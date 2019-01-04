import time, DAN, requests, random

ServerURL = 'http://140.113.199.189:9999' #with no secure connection
#ServerURL = 'https://DomainName' #with SSL connection
Reg_addr = str(random.randint(1, 1024)) #if None, Reg_addr = MAC address

DAN.profile['dm_name']='TextSender'
DAN.profile['df_list']=['TextSenderIDF']
DAN.profile['d_name']= None # None for autoNaming
DAN.device_registration_with_retry(ServerURL, Reg_addr)

while True:
    try:
        DAN.push ('TextSenderIDF', input('(sendtext)>>>'))
    except Exception as e:
        print(e)
        if str(e).find('mac_addr not found:') != -1:
            print('Reg_addr is not found. Try to re-register...')
            DAN.device_registration_with_retry(ServerURL, Reg_addr)
        else:
            print('Connection failed due to unknow reasons.')
            time.sleep(1)

    time.sleep(0.2)
