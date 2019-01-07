# -*- coding: utf-8 -*-
import time, DAN, requests, random
import time
import pandas as pd
from bs4 import BeautifulSoup
from io import open
import requests                  #向網站發出request
from datetime import datetime    #handle timestamp



#ServerURL = 'https://140.113.199.187:9999' #with no secure connection
ServerURL = 'https://test.iottalk.tw' #with SSL connection
#Reg_addr = 'cool85864fdsfsdf6465' #if None, Reg_addr = MAC address
Reg_addr = '2'
#DAN.profile['dm_name']='Map_0416205'
DAN.profile['dm_name']='weather_0416043'
DAN.profile['df_list']=['Humidity_0416205','Rainfall_0416205','Temperature_0416205','Wind_0416205', 'weather_0416043', 'Acceleration-O']
#DAN.profile['df_list']=['weather','Acceleration-O']
DAN.profile['d_name']= None # None for autoNaming
DAN.device_registration_with_retry(ServerURL, Reg_addr)

urls = [#"https://www.cwb.gov.tw/V7/observe/real/ObsN.htm?", #北部
       #"https://www.cwb.gov.tw/V7/observe/real/ObsC.htm?", #中部
       "https://www.cwb.gov.tw/V7/observe/real/ObsS.htm?", #南部
       #"https://www.cwb.gov.tw/V7/observe/real/ObsE.htm?", #東部
       #"https://www.cwb.gov.tw/V7/observe/real/ObsI.htm?", #外島
]

Cord = {
	
    '岡山': {'lat':'22.797078', 'lng':'120.294958', 'Humidity':'0', 'Temp':'0'},
    '茄萣': {'lat':'22.906642', 'lng':'120.182608', 'Humidity':'0', 'Temp':'0'},
	'湖內': {'lat':'22.906642', 'lng':'120.182608', 'Humidity':'0', 'Temp':'0'},
	'彌陀': {'lat':'22.783806', 'lng':'120.246361', 'Humidity':'0', 'Temp':'0'},
	'岡山': {'lat':'22.797078', 'lng':'120.294958', 'Humidity':'0', 'Temp':'0'},
	'楠梓': {'lat':'22.71975', 'lng':'120.286', 'Humidity':'0', 'Temp':'0'},
	'仁武': {'lat':'22.701083', 'lng':'120.347722', 'Humidity':'0', 'Temp':'0'},
	'鼓山': {'lat':'22.623325', 'lng':'120.275575', 'Humidity':'0', 'Temp':'0'},
	'三民': {'lat':'22.645017', 'lng':'120.311233', 'Humidity':'0', 'Temp':'0'},
	'苓雅': {'lat':'22.622742', 'lng':'120.330669', 'Humidity':'0', 'Temp':'0'},
	'林園': {'lat':'22.507747', 'lng':'120.394511', 'Humidity':'0', 'Temp':'0'},
	'大寮': {'lat':'22.605614', 'lng':'120.395717', 'Humidity':'0', 'Temp':'0'},
	'旗山': {'lat':'22.88861400', 'lng':'120.48359700', 'Humidity':'0', 'Temp':'0'},
	'路竹': {'lat':'22.854997', 'lng':'120.259439', 'Humidity':'0', 'Temp':'0'},
	'橋頭': {'lat':'22.757806', 'lng':'120.30575', 'Humidity':'0', 'Temp':'0'},
	'大社': {'lat':'22.730306', 'lng':'120.346639', 'Humidity':'0', 'Temp':'0'},
	
}
def timestamp_handler(timestamp):
    year = '2018'
    month = timestamp.split()[0].split('/')[0]
    day = timestamp.split()[0].split('/')[1]
    hour = timestamp.split()[1].split(':')[0]
    minute = timestamp.split()[1].split(':')[1]
    second = '00'
    return year+'-'+month+'-'+day+' '+hour+':'+minute+':'+second
	

while True:
	#向網站發出request要html資料
    for url in urls:
        res = requests.get(url)
        res.encoding = 'utf-8'
        #確認是否回傳成功
        if  res.status_code == requests.codes.ok:
            #成功的話開始解析網頁
            soup = BeautifulSoup(res.text, 'html.parser')
            data_rows = soup.find_all('tr')
            # print(data_rows[1].find_all('td'))
            # 一筆資料
            # [<td style="display: none;">46694</td>,                  *ID
            # <td id="MapID46694"><a href="46694.htm">基隆</a></td>,   *測站名稱
            # <td>11/20 11:20</td>,                                    *觀測時間
            # <td class="temp1">22.7</td>,                             *溫度(攝氏)          <need>
            # <td class="temp2" style="display: none;">72.9</td>,      *溫度(華氏)
            # <td>陰</td>,                                             *天氣
            # <td>東北</td>,                                           *風向
            # <td>2.5 </td>,                                           *風力(m/s)           <need>
            # <td>2</td>,                                              *風力(級)
            # <td>-</td>,                                              *陣風(m/s)
            # <td>-</td>,                                              *陣風(級)
            # <td>16.0</td>,                                           *能見度(公里)
            # <td>64</td>,                                             *相對溼度(%)         <need>
            # <td>1019.8</td>,                                         *海平面氣壓(百帕)
            # <td><font color="black">0.0</font></td>,                 *當日累積雨量(毫米)  <need>
            # <td>0.1</td>]                                            *日照時數
            for row in data_rows:
                if len(row.find_all('td')) == 16:
                    #測站名稱, 溫度(攝氏), 風力(m/s), 相對溼度(%), 當日累積雨量(毫米), 觀測時間
                    # print(row.find_all('td')[1].find('a').text, row.find_all('td')[3].text, row.find_all('td')[7].text, row.find_all('td')[12].text, row.find_all('td')[14].find('font').text, timestamp_handler(row.find_all('td')[2].text))
                    loc = row.find_all('td')[1].find('a').text
                    if loc in Cord:
                            print('Temperature-I', Cord[loc]['lat'], Cord[loc]['lng'],  row.find_all('td')[1].find('a').text, row.find_all('td')[3].text, timestamp_handler(row.find_all('td')[2].text))
                            print('Temperature-I', Cord[loc]['lat'], Cord[loc]['lng'],  loc, row.find_all('td')[3].text, timestamp_handler(row.find_all('td')[2].text))
                            print('WindSpeed-I', Cord[loc]['lat'], Cord[loc]['lng'],  row.find_all('td')[1].find('a').text, row.find_all('td')[7].text, timestamp_handler(row.find_all('td')[2].text))
                            print('RainMeter-I', Cord[loc]['lat'], Cord[loc]['lng'],  row.find_all('td')[1].find('a').text, row.find_all('td')[14].text, timestamp_handler(row.find_all('td')[2].text))
                            print('Humidity-I', Cord[loc]['lat'], Cord[loc]['lng'],  row.find_all('td')[1].find('a').text, row.find_all('td')[12].text, timestamp_handler(row.find_all('td')[2].text))
                            #DAN.push ('Humidity_0416205',Cord[loc]['lat'], Cord[loc]['lng'],  row.find_all('td')[1].find('a').text, row.find_all('td')[12].text, timestamp_handler(row.find_all('td')[2].text) )
                            #DAN.push ('Rainfall_0416205', Cord[loc]['lat'], Cord[loc]['lng'],  row.find_all('td')[1].find('a').text, row.find_all('td')[14].text, timestamp_handler(row.find_all('td')[2].text))
                            #DAN.push ('Temperature_0416205', Cord[loc]['lat'], Cord[loc]['lng'],  row.find_all('td')[1].find('a').text, row.find_all('td')[3].text, timestamp_handler(row.find_all('td')[2].text))
                            #DAN.push ('Wind_0416205', Cord[loc]['lat'], Cord[loc]['lng'],  row.find_all('td')[1].find('a').text, row.find_all('td')[7].text, timestamp_handler(row.find_all('td')[2].text))
                            Cord[loc]['Humidity']=row.find_all('td')[3].text
                            Cord[loc]['Temp']=row.find_all('td')[12].text
                            #DAN.push('Temperature_0416205',Cord[loc]['lat'], Cord[loc]['lng'],row.find_all('td')[1].find('a').text, row.find_all('td')[3].text, row.find_all('td')[12].text, row.find_all('td')[7].text)
                            #print("")    #time.sleep(600)
    #Pull data from a device feature called "Dummy_Control"
    #    value1=DAN.pull('Acceleration-O')
    #    value2=DAN.pull('yaha')		
    #    if value1 != None:
    #        print (value1)
    #        print (value2)

    #Push data to a device feature called "Dummy_Sensor"
    #    value2=df_tmp[0][1]
    #    value3=df_tmp[0][8]
    #    DAN.push ('Humidity_I', value2)
    #    DAN.push ('temperature', value3)


    #except Exception as e:
    #    print(e)
    #    if str(e).find('mac_addr not found:') != -1:
    #        print('Reg_addr is not found. Try to re-register...')
    #        DAN.device_registration_with_retry(ServerURL, Reg_addr)
    #    else:
    #        print('Connection failed due to unknow reasons.')
    #        time.sleep(1)    

    time.sleep(0.2)

