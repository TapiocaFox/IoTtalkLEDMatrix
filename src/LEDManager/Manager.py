import time, DAN, requests, random, datetime
import sys
from threading import Thread
import re
import json
import traceback

name = ""
light = 0
interval = 50
changeinterval = 10 #s
ledmesseges = {}
ledindex = 0
skip = 0

def sendMessegeToLEDMatrix(msg):
    DAN.push('LEDCommandSender', json.dumps({'model':'LEDMatrix', 'data':[msg]}))

def loop():
    global changeinterval
    global ledindex
    global skip
    while 1:
        if skip == 0:
            now = datetime.datetime.now()
            for index in list(ledmesseges.keys()):
                if index < now:
                    if list(ledmesseges.keys()).index(index) < ledindex:
                        ledindex = ledindex - 1
                    del ledmesseges[index]
            if len(ledmesseges):
                print("ledsent:"+ledmesseges[list(ledmesseges.keys())[ledindex]])
                sendMessegeToLEDMatrix(ledmesseges[list(ledmesseges.keys())[ledindex]]);

            if ledindex < len(ledmesseges)-1:
                ledindex = ledindex+1
            else:
                ledindex = 0
        else:
            skip = 0
        time.sleep(changeinterval)

def push2queue(msg, sec):
    global ledmesseges
    ledmesseges[datetime.datetime.now() + datetime.timedelta(seconds = sec)] = msg

def interrupt(msg):
    global skip
    skip = 1
    DAN.push('LEDCommandSender', json.dumps({'model':'LEDMatrix', 'data':[msg]}))

loopthread = Thread(target=loop)
loopthread.start()

ServerURL = 'http://140.113.199.189:9999' #with no secure connection
#ServerURL = 'https://DomainName' #with SSL connection
Reg_addr = None #if None, Reg_addr = MAC address

DAN.profile['dm_name']='LEDMatrixManager'
DAN.profile['df_list']=['LEDCommandSender', 'LEDManagerReciever']
DAN.profile['d_name']= None # None for autoNaming
DAN.device_registration_with_retry(ServerURL, Reg_addr)

while True:
    try:
        Msg=DAN.pull('LEDManagerReciever')
        if Msg != None:
            msg = json.loads(Msg[0])
            model = msg['model']
            if model == 'TextSender':
                print(msg['data'][0])
                m = re.match("^.*現在(.*)天氣.*$", msg['data'][0])
                if m != None:
                    DAN.push('LEDCommandSender', json.dumps({'model':'weather', 'data':[m.group(1)]}))

                m = re.match("^.*現在(時間|幾點).*$", msg['data'][0])
                if m != None:
                    interrupt('{0:%H:%M}'.format(datetime.datetime.now()))

                m = re.match("^.*今天.*(幾月|幾號|幾日|日期).*$", msg['data'][0])
                if m != None:
                    push2queue('{0:today: %m-%d-%Y}'.format(datetime.datetime.now()), 60)

                m = re.match("^"+name+".*亮度([0-9]*).*$", msg['data'][0])
                if m != None:
                    light = eval(m.group(1))
                    DAN.push('LEDCommandSender', json.dumps({'model':'LEDMatrix', 'data':['!intensity '+str(light)]}))

                m = re.match("^"+name+".*亮.?點.*$", msg['data'][0])
                if m != None:
                    if light < 15:
                        light += 1
                    DAN.push('LEDCommandSender', json.dumps({'model':'LEDMatrix', 'data':['!intensity '+str(light)]}))

                m = re.match("^"+name+".*暗.?點.*$", msg['data'][0])
                if m != None:
                    if light > 0:
                        light -= 1
                    DAN.push('LEDCommandSender', json.dumps({'model':'LEDMatrix', 'data':['!intensity '+str(light)]}))

                m = re.match("^"+name+".*快.?點.*$", msg['data'][0])
                if m != None:
                    interval /= 2
                    DAN.push('LEDCommandSender', json.dumps({'model':'LEDMatrix', 'data':['!interval '+str(interval)]}))

                m = re.match("^"+name+".*慢.?點.*$", msg['data'][0])
                if m != None:
                    interval *= 2
                    DAN.push('LEDCommandSender', json.dumps({'model':'LEDMatrix', 'data':['!interval '+str(interval)]}))

                m = re.match("^"+name+".*間隔.*(久|長).?點.*$", msg['data'][0])
                if m != None:
                    if changeinterval <3:
                        changeinterval += 3

                m = re.match("^"+name+".*間隔.*(短|小).?點.*$", msg['data'][0])
                if m != None:
                    if changeinterval > 3:
                        changeinterval -= 3

                m = re.match("^"+name+".*向左滑.*$", msg['data'][0])
                if m != None:
                    DAN.push('LEDCommandSender', json.dumps({'model':'LEDMatrix', 'data':['!mode scrollLeft']}))

                m = re.match("^"+name+".*向右滑.*$", msg['data'][0])
                if m != None:
                    DAN.push('LEDCommandSender', json.dumps({'model':'LEDMatrix', 'data':['!mode scrollRight']}))

                m = re.match("^.*你.*名字.*$", msg['data'][0])
                if m != None:
                    interrupt(name)

                m = re.match("^"+name+".*插播(.*)$", msg['data'][0])
                if m != None:
                    interrupt(m.group(1))

                m = re.match("^"+name+".*顯示([0-9]*)秒(.*)$", msg['data'][0])
                if m != None:
                    push2queue(m.group(2), eval(m.group(1)))

                m = re.match("^"+name+".*改名.*([a-z]*)$", msg['data'][0])
                if m != None:
                    name = m.group(1)

                m = re.match("^"+name+"笑臉$", msg['data'][0])
                if m != None:
                    interrupt("O wO")

                m = re.match("^"+name+"哭臉$", msg['data'][0])
                if m != None:
                    interrupt("Q wQ")

                m = re.match("^"+name+"眨眼$", msg['data'][0])
                if m != None:
                    interrupt("> w<")

                m = re.match("^(!.*)$", msg['data'][0])
                if m != None:
                    DAN.push('LEDCommandSender', json.dumps({'model':'LEDMatrix', 'data':[m.group(1)]}))
            elif model == 'weather':
                push2queue(msg['data'][0], 60)

    except Exception as e:
        print(e)
        print(traceback.format_exception(None, e, e.__traceback__), file=sys.stderr, flush=True)
        if str(e).find('mac_addr not found:') != -1:
            print('Reg_addr is not found. Try to re-register...')
            DAN.device_registration_with_retry(ServerURL, Reg_addr)
        else:
            print('Connection failed due to unknow reasons.')
            time.sleep(1)

    time.sleep(0.2)
