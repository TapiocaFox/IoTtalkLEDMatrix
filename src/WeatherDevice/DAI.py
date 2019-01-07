import time, DAN, requests, random
from bs4 import BeautifulSoup    #解析html網頁
from datetime import datetime    #handle timestamp


ServerURL = 'http://140.113.199.189:9999' #with no secure connection
#ServerURL = 'https://DomainName' #with SSL connection
Reg_addr = None #if None, Reg_addr = MAC address

DAN.profile['dm_name']='weather'
DAN.profile['df_list']=['weather-i', 'weather-o']
DAN.profile['d_name']= None # None for autoNaming
DAN.device_registration_with_retry(ServerURL, Reg_addr)

####################################################################
urls = ["https://www.cwb.gov.tw/V7/observe/real/ObsN.htm?", #北部
       #"https://www.cwb.gov.tw/V7/observe/real/ObsC.htm?", #中部
       #"https://www.cwb.gov.tw/V7/observe/real/ObsS.htm?", #南部
       #"https://www.cwb.gov.tw/V7/observe/real/ObsE.htm?", #東部
       #"https://www.cwb.gov.tw/V7/observe/real/ObsI.htm?", #外島
]

Cord = {
    '新竹': {'eng':'Hsinchu'},
    '五峰站': {'eng':'Wufong Station'},
    '梅花': {'eng':'Meihua'},
    '關西': {'eng':'Guanxi'},
    '峨眉': {'eng':'Emei'},
    '打鐵坑': {'eng':'Datiekeng'},
    '橫山': {'eng':'Hengshan'},
    '雪霸': {'eng':'Xueba'},
    '竹東': {'eng':'Zhudong'},
    '寶山': {'eng':'Baoshan'},
    '新豐': {'eng':'Xinfeng'},
    '湖口': {'eng':'Hukou'},
    '香山': {'eng':'Xiangshan'},
    '新竹市東區': {'eng':'Hsinchu City-East Dist.'}
}
def timestamp_handler(timestamp):
    year = '2018'
    month = timestamp.split()[0].split('/')[0]
    day = timestamp.split()[0].split('/')[1]
    hour = timestamp.split()[1].split(':')[0]
    minute = timestamp.split()[1].split(':')[1]
    second = '00'
    return year+'-'+month+'-'+day+' '+hour+':'+minute+':'+second
####################################################################


while True:
    try:
    #Pull data from a device feature called "Dummy_Control"
        value1=DAN.pull('weather-o')
        if value1 != None:
            target = value1[0]
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
                    # [<td style="display: none;">46694</td>,                  *ID 0
                    # <td id="MapID46694"><a href="46694.htm">基隆</a></td>,   *測站名稱  1
                    # <td>11/20 11:20</td>,                                    *觀測時間  2
                    # <td class="temp1">22.7</td>,                             *溫度(攝氏)   3       <need>
                    # <td class="temp2" style="display: none;">72.9</td>,      *溫度(華氏)  4
                    # <td>陰</td>,                                             *天氣  5
                    # <td>東北</td>,                                           *風向 6 
                    # <td>2.5 </td>,                                           *風力(m/s) 7           <need>
                    # <td>2</td>,                                              *風力(級)
                    # <td>-</td>,                                              *陣風(m/s)
                    # <td>-</td>,                                              *陣風(級)  10
                    # <td>16.0</td>,                                           *能見度(公里) 11
                    # <td>64</td>,                                             *相對溼度(%)    12      <need>
                    # <td>1019.8</td>,                                         *海平面氣壓(百帕)  13
                    # <td><font color="black">0.0</font></td>,                 *當日累積雨量(毫米)   14 <need>
                    # <td>0.1</td>]                                            *日照時數
                    #測站名稱, 溫度(攝氏), 風力(m/s), 相對溼度(%), 當日累積雨量(毫米), 觀測時間
                    for row in data_rows:
                        if len(row.find_all('td')) == 16:
                            loc = row.find_all('td')[1].find('a').text
                            if (loc in Cord)and(loc==target):
                                print(Cord[loc]['eng']+" temp:"+row.find_all('td')[3].text+" wind:"+(row.find_all('td')[7].text)[0:len(row.find_all('td')[7].text)-1]+" humidity:"+row.find_all('td')[12].text+" rainfall:"+row.find_all('td')[14].text)
                                DAN.push('weather-i',Cord[loc]['eng']+" temp:"+row.find_all('td')[3].text+" wind:"+(row.find_all('td')[7].text)[0:len(row.find_all('td')[7].text)-1]+" humidity:"+row.find_all('td')[12].text+" rainfall:"+row.find_all('td')[14].text)
	
    #Push data to a device feature called "Dummy_Sensor"
       # test = input("instruction: ")
     #   DAN.push ('weather-I',test)

    except Exception as e:
        print(e)
        if str(e).find('mac_addr not found:') != -1:
            print('Reg_addr is not found. Try to re-register...')
            DAN.device_registration_with_retry(ServerURL, Reg_addr)
        else:
            print('Connection failed due to unknow reasons.')
            time.sleep(1)    

    time.sleep(0.2)

