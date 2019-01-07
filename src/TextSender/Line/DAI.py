import time, DAN, requests, random
import threading
from flask import Flask, request, abort
from linebot import LineBotApi, WebhookHandler
from linebot.exceptions import InvalidSignatureError 
from linebot.models import MessageEvent, TextMessage, TextSendMessage
import queue
import pandas as pd
from bs4 import BeautifulSoup
from io import open                 #向網站發出request
from datetime import datetime    #handle timestamp

ServerURL = 'http://140.113.199.189:9999' #with no secure connection
#ServerURL = 'https://test.iottalk.tw' #with SSL connection
Reg_addr = '2' #if None, Reg_addr = MAC address

#DAN.profile['dm_name']='Line_0416043'
DAN.profile['dm_name']='TextSender'
DAN.profile['df_list']=['TextSenderIDF']
DAN.profile['d_name']= None # None for autoNaming
DAN.device_registration_with_retry(ServerURL, Reg_addr)

line_bot_api = LineBotApi('QwKhnBc5In5OV/vZ9DmBjEy6Ji/cpcIEwKvBwU4gmmfzondCEsD99UGG3Tm0TAvmu63XWwbSxTkAbnbqBkCCXRbLrJlMDhTj6UqWdsb+495UWx7NC0TVK9I/lCLmz/vFewO8nFKOaG9wLgvjbe8PxQdB04t89/1O/w1cDnyilFU=') #LineBot's Channel access token
handler = WebhookHandler('a429f45f21c8b9d5e972482ad3659faf')        #LineBot's Channel secret
user_id_set=set()                                         #LineBot's Friend's user id 
app = Flask(__name__)

def timestamp_handler(timestamp):
    year = '2018'
    month = timestamp.split()[0].split('/')[0]
    day = timestamp.split()[0].split('/')[1]
    hour = timestamp.split()[1].split(':')[0]
    minute = timestamp.split()[1].split(':')[1]
    second = '00'
    return year+'-'+month+'-'+day+' '+hour+':'+minute+':'+second

def loadUserId():
    try:
        idFile = open('idfile', 'r')
        idList = idFile.readlines()
        idFile.close()
        idList = idList[0].split(';')
        idList.pop()
        return idList
    except Exception as e:
        print(e)
        return None


def saveUserId(userId):
        idFile = open('idfile', 'a')
        idFile.write(userId+';')
        idFile.close()


@app.route("/", methods=['GET'])
def hello():
    return "HTTPS Test OK."

@app.route("/", methods=['POST'])
def callback():
    signature = request.headers['X-Line-Signature']    # get X-Line-Signature header value
    body = request.get_data(as_text=True)              # get request body as text
    print("Request body: " + body, "Signature: " + signature)
    try:
        handler.handle(body, signature)                # handle webhook body
    except InvalidSignatureError:
        abort(400)
    return 'OK'


@handler.add(MessageEvent, message=TextMessage)
def handle_message(event):
    Msg = event.message.text
    if Msg == 'Hello, world': return
    print('GotMsg:{}'.format(Msg))
    DAN.push ('TextSenderIDF', Msg)
    #line_bot_api.reply_message(event.reply_token,TextSendMessage(text=value1[0]))   # Reply API example
    userId = event.source.user_id
    if not userId in user_id_set:
        user_id_set.add(userId)
        saveUserId(userId)

#def g():
#   while True:
#    
#    try:
#       value1=DAN.pull('Acceleration-O')
#       if value1 != None:
#           for userId in user_id_set:
#              line_bot_api.push_message(userId,TextSendMessage(text=value1[0]))   # Reply API example
#       print(value1)
#    #Pull data from a device feature called "Dummy_Control"
#        #value1=DAN.pull('msg_o_0416043')
#                
#    
#    #Push data to a device feature called "Dummy_Sensor"
#       #value2=time.time()
#        #value2=random.uniform(1, 10)
#        #DAN.push ('msg_i_0416043', value2,  value2)
#    except Exception as e:
#        print(e)
#        if str(e).find('mac_addr not found:') != -1:
#            print('Reg_addr is not found. Try to re-register...')
#            DAN.device_registration_with_retry(ServerURL, Reg_addr)
#        else:
#            print('Connection failed due to unknow reasons.')
#            time.sleep(1)    
#
#    time.sleep(0.2)

if __name__ == "__main__":

    idList = loadUserId()
    if idList: user_id_set = set(idList)

    try:
        for userId in user_id_set:
            line_bot_api.push_message(userId, TextSendMessage(text='LineBot is ready for you.'))  # Push API example
    except Exception as e:
        print(e)
    #t = threading.Thread(target=g, args=())
    #t.daemon = True     # this ensures thread ends when main process ends
    #t.start()
    app.run('127.0.0.1', port=32768, threaded=True, use_reloader=False)



