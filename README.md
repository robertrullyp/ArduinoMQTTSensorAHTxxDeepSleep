
# Arduino MQTT Sensor (Temperature & Humidity) 

Didesign buat jadi sensor Temperature & Humidity yang memungkinkan buat rechargeable atau dipakein powerbank dengan mengimplemtasi fitur deep sleep di mcu esp dan dapat diintegrasi dengan dashboard home automation Home Assistant.

cek sketch arduinonya disini [Arduino MQTT Sensor](https://github.com/robertrullyp/ArduinoMQTTSensorAHTxxDeepSleep/blob/main/src/main.cpp)
## Device Components & Wiring

- ESP-01s
- USB Adapter for ESP-01
- Sensor Suhu & Kelembaban (AHT10)

Buat Wiringnya segera diupdate......
## Library

Buat library tambahan yang perlu diinstal :

- [PubSubClient](https://github.com/knolleary/pubsubclient)
- [AHTxx](https://github.com/enjoyneering/AHTxx)


## Custom Parameter

rubah sendiri sesuaikn parameternya dengan parameter kalian :

    const char* ssid = "wifi_ssid";
    const char* password = "wifi_password";
    const char* user = "user_mqtt";
    const char* passwd = "password_mqtt";
    const char* mqtt_server = "server_mqtt";
    const char* mqtt_port = "8883";
    
    String PubTopic = "user_mqtt/state";
    String SubTopic = "user_mqtt/set";

ssid dan password isi dengan ssid dan password buat device connect ke wifi. Untuk yg lainnya adalah parameter buat connect ke Server MQTT. udah ditest pake server [hivemq cloud](https://console.hivemq.cloud) juga kok

PubTopic adalah topic buat kalian liat status sensor dan parameter kayak interval buat deep sleep, aktifin atau matiin mode deep sleep & waktu deep sleepnya.

SubTopic adalah topic buat ngerubah parameter dan ngirim command reset.

detail struktur topic nya cek di bagian struktur topic dibawah.
## Struktur Topic


Kalau mau ngerubah parameter deep sleep bisa kirim topic ke sini :

    [SubTopic]/sleeptime
    [SubTopic]/sleepmode
    [SubTopic]/sleepinterval 

[SubTopic] ini berdasarkan custom parameter yang kalian isi

diset pake retain on yaa biar setelah perangkat booting bisa ngebaca custom settingnya yg kalian kirim.
kalau buat [SubTopic]/cmd jangan di retain, kalau di retain nanti device bakalan ngereboot mulu setelah selesai booting

buat nilai sensornya ada di topic [PubTopic]/sensors/temperature dan [PubTopic]/sensors/humidity

kalau parameter deep sleep yang lagi running di device nanti ada di topic:
[PubTopic]/sleeptime
[PubTopic]/sleepmode
[PubTopic]/sleepinterval

buat topic availabilitynya kalau ada di [PubTopic]/status payloadnya akan ngirim status perangkat online/offline jadi kalau lagi di beroperasi device bakalan ngirim status online, ketika offline artinya device lagi deep sleep atau mungkin device kehilangan koneksi kalau lebih dari interval yang tentukan.

![Capture](https://github.com/robertrullyp/ArduinoMQTTSensorAHTxxDeepSleep/assets/12167355/bc355c2a-be5a-4b66-8620-9dc19ea86a47)


## Demo

Posted Soon....
## Authors

- [Robert Rully Pernando](https://github.com/robertrullyp)

