#ifndef IoTtalll_H_
#define IoTtalll_H_
#define MaxDFSize 10

#include "ESP8266HTTPClient2.h"

class IoTtalkDevice {
  public:
    // IoTtalkDevice() : () { }
    // IoTtalkDevice();
    // ~IoTtalkDevice();
    int push(char *df_name, String value);
    String pull(char *df_name);
    void initialize();
    void loop();
    void setDeviceModelName(const String& str);
    void setDeviceFeatures(String df_list[], int size);

  protected:

    String DefaultIoTtalkServerIP;
    String DM_name;
    String DF_list[MaxDFSize];
    String DF_timestamp[MaxDFSize];
    String DeviceName;
    HTTPClient http;
     //1:AP , 0: STA
    long LEDflashCycle = millis();
    long LEDonCycle = millis();
    int LEDhadFlashed = 0;
    String DeviceURL;

    String removeWhiteSpace(const String& str );
    // void clearEEPROM(int sw=0);

    // void saveNetInfo(char *wifiSSID, char *wifiPASS, char *ServerIP);
    // int readNetInfo(char *wifiSSID, char *wifiPASS, char *ServerIP);
    // String scan_network(void);

    // void handleRoot(int retry);
    // void handleNotFound();
    // void saveInfoAndConnectToWiFi();
    // void startWebServer(void);
    // void setupWifiandIoTtalk(void);
    // void connectToWiFi(char *wifiSSID, char *wifiPASS);
    int registerIoTtalk(void);
    void initDeviceFeaturesTimestamp();
    int returnDFindex(char *df_name);
};

#endif /* IoTtalll_H_ */
