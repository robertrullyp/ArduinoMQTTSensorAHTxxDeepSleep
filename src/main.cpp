#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <AHTxx.h>
#if defined(ESP32)
  #include <WiFi.h>
  #include <ESPmDNS.h>
#else defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <DNSServer.h>
#endif

unsigned long long LastPublishMillis = 0;
const long publishinterval = 3000;
unsigned long long LastSleepMillis = 0;
unsigned long long sleepinterval = 5;
unsigned int sleeptime = 60;
bool sleepmode = true;

const char* ssid = "wifi_ssid";
const char* password = "wifi_password";
const char* user = "user_mqtt";
const char* passwd = "password_mqtt";
const char* mqtt_server = "server_mqtt";
// const char* mqtt_server = "iotsmarthome.my.id";
const char* mqtt_port = "8883";

String PubTopic = "user_mqtt/state";
String SubTopic = "user_mqtt/set";


#if defined(ESP32)
  const static char* test_root_ca PROGMEM = \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n" \
  "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
  "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n" \
  "WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n" \
  "ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n" \
  "MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n" \
  "h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n" \
  "0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n" \
  "A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n" \
  "T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n" \
  "B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n" \
  "B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n" \
  "KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n" \
  "OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n" \
  "jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n" \
  "qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n" \
  "rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n" \
  "HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n" \
  "hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n" \
  "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n" \
  "3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n" \
  "NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n" \
  "ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n" \
  "TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n" \
  "jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n" \
  "oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n" \
  "4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n" \
  "mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n" \
  "emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n" \
  "-----END CERTIFICATE-----\n";
#else defined(ESP8266)
  // const static char fingerprint[] PROGMEM = "AD 76 EC F9 DD 41 1F FC 0F 6E DC A9 06 D3 0D C4 85 B7 24 2F"; //SHA1 FINGERPRINT
  const static char fingerprint[] PROGMEM = "AA D4 06 67 05 F2 D3 2E DD 91 76 6F BE D5 FB EC 0A 34 C3 BE"; //SHA1 HiveMQ
#endif

WiFiClientSecure  espClient;
PubSubClient client(espClient);
AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print(F("Message arrived on topic: "));
  Serial.print(topic);
  Serial.print(F(". Message: "));
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  Serial.println(messageTemp);

  if (String(topic) == (SubTopic + "/sleepmode")) {
    sleepmode = messageTemp.toInt();
    Serial.print(F("Sleep Mode Set to : "));
    Serial.println(sleepmode);
  }
  if (String(topic) == (SubTopic + "/sleepinterval")) {
    if (messageTemp.toInt() <= 5 ) sleepinterval = 5;
    else sleepinterval = messageTemp.toInt();
    Serial.print(F("Sleep Interval Set to : "));
    Serial.print(sleepinterval);
    Serial.println(F(" Seconds"));
  }
  if (String(topic) == (SubTopic + "/sleeptime")) {
    sleeptime = messageTemp.toInt();
    Serial.print(F("Sleep Time Set to : "));
    Serial.print(sleeptime);
    Serial.println(F(" Seconds"));
  }
  if (String(topic) == (SubTopic + "/cmd")) {
    messageTemp.toUpperCase();
    if (messageTemp == "RESET" || messageTemp == "REBOOT")
#if ESP8266
    ESP.reset();
#else
    ESP.restart();
#endif
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    // Attempt to connect
    if (client.connect("ESP8266Client", user, passwd, (String(user) + "/status").c_str(), 0, true, "offline")) {
      Serial.println(F("MQTT Connected"));
      // Subscribe
      client.subscribe((SubTopic + "/#").c_str());
      // client.subscribe("usertest/#");
      LastSleepMillis = millis();
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  client.publish((String(user) + "/status").c_str(), "online");
}


void setup() {
  Serial.begin(115200);
  Serial.println(F("UART ready"));
  // while (aht10.begin() != true) { //for ESP-01 use aht10.begin(0, 2);
  while (aht10.begin(0, 2) != true) {
    Serial.println(F("AHT1x not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free
    delay(5000);
  }
  Serial.println(F("AHT10 OK"));
#if defined(ESP32)
  if (mqtt_server=="192.168.1.93") espClient.setInsecure();
  else espClient.setCACert(test_root_ca);
  Serial.print(test_root_ca);
#else defined(ESP8266)
  // espClient.setInsecure();
  espClient.setFingerprint(fingerprint);
  Serial.print(fingerprint);
#endif
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(F("."));
  }
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(callback);
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println(F("Auth Failed"));
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println(F("Begin Failed"));
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println(F("Connect Failed"));
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println(F("Receive Failed"));
    } else if (error == OTA_END_ERROR) {
      Serial.println(F("End Failed"));
    }
  });
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  if (!client.connected()) {
    reconnect();
  }
  else client.publish((String(user) + "/status").c_str(), "online");
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  client.loop();
  if (millis() - LastPublishMillis >= publishinterval) {
    LastPublishMillis = millis();
    client.publish((PubTopic + "/sensors/temperature").c_str(), String(aht10.readTemperature()).c_str());
    client.publish((PubTopic + "/sensors/humidity").c_str(), String(aht10.readHumidity()).c_str());
    client.publish((PubTopic + "/sleepmode").c_str(), String(sleepmode).c_str());
    client.publish((PubTopic + "/sleepinterval").c_str(), String(sleepinterval).c_str());
    client.publish((PubTopic + "/sleeptime").c_str(), String(sleeptime).c_str());
    Serial.println(F("Sensor State Published"));
  }
  if (sleepmode == true) {
    if (millis() - LastSleepMillis >= (sleepinterval*1000)) {
      LastSleepMillis = millis();
      Serial.print(F("Sleeping Now for "));
      Serial.print(sleeptime);
      Serial.println(F(" Seconds..."));
      ESP.deepSleep(sleeptime*1000000);
    }
  }
}
