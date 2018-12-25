#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>

#include "IoTtalkDevice.h"
#define DefaultIoTtalkServerIP "140.113.199.189"
int n_of_DF;

char IoTtalkServerIP[100] = "";
uint8_t isInWiFiMode = 1;
// HTTPClient::HTTPClient() {
//   DefaultIoTtalkServerIP = "140.113.199.189";
// }
ESP8266WebServer HttpServer;
void setupWifiandIoTtalk();

void IoTtalkDevice::setDeviceModelName(const String& str) {
  DM_name = str;
}

void IoTtalkDevice::setDeviceFeatures(String df_list[], int size) {
  int n = size;
  n_of_DF = n;
  for(int i=0; i<n; i++) {
    DF_list[i] = df_list[i];
  }
}

String IoTtalkDevice::removeWhiteSpace(const String& str) {
    String str_no_ws ;
    for( char c : str ) if( !std::isspace(c) ) str_no_ws += c ;
    return str_no_ws ;
}


void connectToWiFi(char *wifiSSID, char *wifiPASS){
  long connecttimeout = millis();

  WiFi.softAPdisconnect(true);
  Serial.println("-----Connect to Wi-Fi-----");
  WiFi.begin(wifiSSID, wifiPASS);

  while (WiFi.status() != WL_CONNECTED && (millis() - connecttimeout < 10000) ) {
      delay(1000);
      Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.println ( "Connected!\n");
    digitalWrite(2,LOW);
    isInWiFiMode = 0;
  }
  else if (millis() - connecttimeout > 10000){
    Serial.println("Connect fail");
    setupWifiandIoTtalk();
  }
}

void clearEEPROM(int sw=0){
    if (!sw){
        Serial.println("Count down 3 seconds to clear EEPROM.");
        digitalWrite(2,LOW);
        delay(3000);
    }
    if( (digitalRead(0) == LOW) || (sw == 1) ){
        for(int addr=0; addr<50; addr++) EEPROM.write(addr,0);   // clear eeprom
        EEPROM.commit();
        Serial.println("Clear EEPROM and reboot.");
        digitalWrite(2,HIGH);
        ESP.reset();
    }
}

void saveNetInfo(char *wifiSSID, char *wifiPASS, char *ServerIP){  //stoage format: [SSID,PASS,ServerIP]
    char *netInfo[3] = {wifiSSID, wifiPASS, ServerIP};
    int addr=0,i=0,j=0;
    EEPROM.write (addr++,'[');  // the code is equal to (EEPROM.write (addr,'[');  addr=addr+1;)
    for (j=0;j<3;j++){
        i=0;
        while(netInfo[j][i] != '\0') EEPROM.write(addr++,netInfo[j][i++]);
        if(j<2) EEPROM.write(addr++,',');
    }
    EEPROM.write (addr++,']');
    EEPROM.commit();
}

int readNetInfo(char *wifiSSID, char *wifiPASS, char *ServerIP){   // storage format: [SSID,PASS,ServerIP]
    char *netInfo[3] = {wifiSSID, wifiPASS, ServerIP};
    String readdata="";
    int addr=0;

    char temp = EEPROM.read(addr++);
    if(temp == '['){
        for (int i=0; i<3; i++){
            readdata ="";
            while(1){
                temp = EEPROM.read(addr++);
                if (temp == ',' || temp == ']') break;
                readdata += temp;
            }
            readdata.toCharArray(netInfo[i],100);
        }

        if (String(ServerIP).length () < 7){
            Serial.println("ServerIP loading failed.");
            return 2;
        }
        else{
            Serial.println("Load setting successfully.");
            return 0;
        }
    }
    else{
        Serial.println("no data in eeprom");
        return 1;
    }
}


String scan_network(void){
    int AP_N,i;  //AP_N: AP number
    String AP_List="<select name=\"SSID\" style=\"width: 150px; font-size:16px; color:blue; \" required>" ;// make ap_name in a string
    AP_List += "<option value=\"\" disabled selected>隢���</option>";

    WiFi.disconnect();
    delay(100);
    AP_N = WiFi.scanNetworks();

    if(AP_N>0) for (i=0;i<AP_N;i++) AP_List += "<option value=\""+WiFi.SSID(i)+"\">" + WiFi.SSID(i) + "</option>";
    else AP_List = "<option value=\"\">NO AP</option>";
    AP_List +="</select><br><br>";
    return(AP_List);
}

void handleRoot(int retry){
  String temp = "<html><title>Wi-Fi Setting</title>";
  temp += "<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>";
  temp += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body bgcolor=\"#F2F2F2\">";
  if (retry) temp += "<font color=\"#FF0000\">Please fill all fields.</font>";
  temp += "<form action=\"setup\"><div>";
  temp += "<center>SSID:<br>";
  temp += scan_network();
  temp += "Password:<br>";
  temp += "<input type=\"password\" name=\"Password\" vplaceholder=\"Password\" style=\"width: 150px; font-size:16px; color:blue; \">";
  temp += "<br><br>IoTtalk Server IP <br>";
  temp += "<input type=\"serverIP\" name=\"serverIP\" value=\"";
  temp += DefaultIoTtalkServerIP;
  temp += "\" style=\"width: 150px; font-size:16px; color:blue;\" required>";
  temp += "<br><br><input style=\"-webkit-border-radius: 11; -moz-border-radius: 11";
  temp += "border-radius: 0px;";
  temp += "text-shadow: 1px 1px 3px #666666;";
  temp += "font-family: Arial;";
  temp += "color: #ffffff;";
  temp += "font-size: 18px;";
  temp += "background: #AAAAAA;";
  temp += "padding: 10px 20px 7px 20px;";
  temp += "text-decoration: none;\"" ;
  temp += "type=\"submit\" value=\"Submit\" on_click=\"javascript:alert('TEST');\"></center>";
  temp += "</div></form><br>";
  temp += "</body></html>";
  HttpServer.send ( 200, "text/html", temp );
}

void handleNotFound() {
  HttpServer.send( 404, "text/html", "Page not found.");
}

void saveInfoAndConnectToWiFi() {
    Serial.println("Get network information.");
    char _SSID_[100]="";
    char _PASS_[100]="";

    if (HttpServer.arg(0) != "" && HttpServer.arg(2) != ""){//arg[0]-> SSID, arg[1]-> password (both string)
        HttpServer.arg(0).toCharArray(_SSID_,100);
        HttpServer.arg(1).toCharArray(_PASS_,100);
        HttpServer.arg(2).toCharArray(IoTtalkServerIP,100);
        HttpServer.send(200, "text/html", "<html><body><center><span style=\" font-size:72px; color:blue; margin:100px; \"> Setup successfully. </span></center></body></html>");
        HttpServer.stop();
        saveNetInfo(_SSID_, _PASS_, IoTtalkServerIP);
        connectToWiFi(_SSID_, _PASS_);
    }
    else {
        handleRoot(1);
    }
}

void startWebServer(void){
    HttpServer.on ( "/", [](){handleRoot(0);} );
    HttpServer.on ( "/setup", saveInfoAndConnectToWiFi);
    HttpServer.onNotFound ( handleNotFound );
    HttpServer.begin();
}


void setupWifiandIoTtalk(void){
    String softapname = "MCU-";
    uint8_t MAC_array[6];
    WiFi.macAddress(MAC_array);
    for (int i=0;i<6;i++){
        if( MAC_array[i]<0x10 ) softapname+="0";
        softapname+= String(MAC_array[i],HEX);      //Append the mac address to DeviceURL string
    }

    IPAddress ip(192,168,0,1);
    IPAddress gateway(192,168,0,1);
    IPAddress subnet(255,255,255,0);
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    WiFi.softAPConfig(ip,gateway,subnet);
    WiFi.softAP(&softapname[0]);
    startWebServer();
    Serial.println ( "Switch to AP mode and start web HttpServer." );
}

int IoTtalkDevice::registerIoTtalk(void){
    DeviceURL = "http://" + String(IoTtalkServerIP) + ":9999/";

    String *df_list = DF_list;

    // int n_of_DF = sizeof(df_list)/sizeof(df_list[0]); // the number of DFs in the DF_list

    String dflist_string = "";
    for (int i=0; i<n_of_DF; i++){
        dflist_string += "\"" + df_list[i] + "\"";
        if (i<n_of_DF-1) dflist_string += ",";
    }

    uint8_t MAC_array[6];
    WiFi.macAddress(MAC_array);//get esp12f mac address
    for (int i=0;i<6;i++){
        if( MAC_array[i]<0x10 ) DeviceURL+="0";
        DeviceURL+= String(MAC_array[i],HEX);      //Append the mac address to DeviceURL string
    }

    //send the register packet
    Serial.println("[HTTP] POST..." + DeviceURL);
    String profile="{\"profile\": {\"d_name\": \"";
    //profile += "MCU.";
    DeviceName = "";
    for (int i=3;i<6;i++){
        // if( MAC_array[i]<0x10 ) profile+="0";
        DeviceName += String(MAC_array[i],HEX);
    }
    DeviceName = DeviceName+DM_name;
    profile += DeviceName;
    profile += "\", \"dm_name\": \"";
    profile += DM_name;
    profile += "\", \"is_sim\": false, \"df_list\": [";
    profile +=  dflist_string;
    profile += "]}}";

    http.begin(DeviceURL);
    http.addHeader("Content-Type","application/json");
    // Serial.println(profile);
    int httpCode = http.POST(profile);

    Serial.println("[HTTP] Register... code: " + (String)httpCode );
    Serial.println(http.getString());
    //http.end();
    DeviceURL +="/";
    return httpCode;
}


void IoTtalkDevice::initDeviceFeaturesTimestamp(){
  for (int i=0; i<MaxDFSize; i++) DF_timestamp[i] = "";
}

int IoTtalkDevice::returnDFindex(char *df_name){
    for (int i=0; i<MaxDFSize; i++){
        if (String(df_name) ==  DF_list[i]) return i;
    }

    return MaxDFSize+1;  // DF_timestamp is full
}

int IoTtalkDevice::push(char *df_name, String value){

    http.begin( DeviceURL + String(df_name));
    http.addHeader("Content-Type","application/json");
    String data = "{\"data\":[" + value + "]}";
    int httpCode = http.PUT(data);
    if (httpCode != 200) Serial.println("[HTTP] PUSH \"" + String(df_name) + "\"... code: " + (String)httpCode + ", retry to register.");
    while (httpCode != 200){
        digitalWrite(4, LOW);
        digitalWrite(2, HIGH);
        httpCode = registerIoTtalk();
        if (httpCode == 200){
            http.PUT(data);
           // if (switchState) digitalWrite(4,HIGH);
        }
        else delay(3000);
    }
    http.end();
    return httpCode;
}

String IoTtalkDevice::pull(char *df_name){

    http.begin( DeviceURL + String(df_name) );
    http.addHeader("Content-Type","application/json");
    int httpCode = http.GET(); //http state code

    if (httpCode != 200) Serial.println("[HTTP] "+DeviceURL + String(df_name)+" PULL \"" + String(df_name) + "\"... code: " + (String)httpCode + ", retry to register.");
    while (httpCode != 200){
        digitalWrite(4, LOW);
        digitalWrite(2, HIGH);
        httpCode = registerIoTtalk();
        if (httpCode == 200){
            http.GET();
            //if (switchState) digitalWrite(4,HIGH);
        }
        else delay(3000);
    }
    String get_ret_str = http.getString();  //After send GET request , store the return string
//    Serial.println

    // Serial.println("output "+String(df_name)+": \n"+get_ret_str);
    http.end();

    get_ret_str = removeWhiteSpace(get_ret_str);
    int string_index = 0;
    string_index = get_ret_str.indexOf("[",string_index);
    String portion = "";  //This portion is used to fetch the timestamp.
    if (get_ret_str[string_index+1] == '[' &&  get_ret_str[string_index+2] == '\"'){
        string_index += 3;
        while (get_ret_str[string_index] != '\"'){
          portion += get_ret_str[string_index];
          string_index+=1;
        }

        if (DF_timestamp[returnDFindex(df_name)] != portion){
            DF_timestamp[returnDFindex(df_name)] = portion;
            string_index = get_ret_str.indexOf("[",string_index);
            string_index += 1;
            portion = ""; //This portion is used to fetch the data.
            while (get_ret_str[string_index] != ']'){
                portion += get_ret_str[string_index];
                string_index+=1;
            }
            return portion;   // return the data.
         }
         else return "___NULL_DATA___";
    }
    else return "___NULL_DATA___";
}

void IoTtalkDevice::initialize() {
  EEPROM.begin(512);
  Serial.begin(115200);

  char wifissid[100]="";
  char wifipass[100]="";
  int statesCode = readNetInfo(wifissid, wifipass, IoTtalkServerIP);
  //for (int k=0; k<50; k++) Serial.printf("%c", EEPROM.read(k) );  //inspect EEPROM data for the debug purpose.

  if (!statesCode)  connectToWiFi(wifissid, wifipass);
  else{
      Serial.println("Laod setting failed! statesCode: " + String(statesCode)); // StatesCode 1=No data, 2=ServerIP with wrong format
      setupWifiandIoTtalk();
  }

  while(isInWiFiMode){
      HttpServer.handleClient(); //waitting for connecting to AP ;
      delay(10);
  }

  statesCode = 0;

  while (statesCode != 200) {
      statesCode = registerIoTtalk();
      if (statesCode != 200){
          Serial.println("Retry to register to the IoTtalk HttpServer. Suspend 3 seconds.");
          if (digitalRead(0) == LOW) clearEEPROM();
          delay(3000);
      }
  }

  initDeviceFeaturesTimestamp();
}


void IoTtalkDevice::loop() {
  if (digitalRead(0) == LOW) clearEEPROM();

  if (millis() - LEDflashCycle > 2000){
      LEDhadFlashed = 0;
      LEDflashCycle = millis();
  }

  if (!LEDhadFlashed){
    digitalWrite(2, 0);
    LEDhadFlashed = 1;
    LEDonCycle = millis();
  }

  if (millis()-LEDonCycle > 1) digitalWrite(2, 1);
}
