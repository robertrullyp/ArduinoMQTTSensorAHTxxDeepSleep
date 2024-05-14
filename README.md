
# Arduino MQTT Sensor (Temperature & Humidity) 

Didesign buat jadi sensor Temperature & Humidity yang memungkinkan buat rechargeable atau dipakein powerbank dengan mengimplemtasi fitur deep sleep di mcu esp dan dapat diintegrasi dengan dashboard home automation Home Assistant.

cek sketch arduinonya disini [Arduino MQTT Sensor](https://github.com/robertrullyp/ArduinoMQTTSensorAHTxxDeepSleep/blob/main/src/main.cpp)
## Device Components & Wiring

- ESP-01s

![ESP-01s](https://www.makerstore.com.au/wp-content/uploads/2017/09/ELEC-ESP01S-01-324x324.jpg)
- USB Adapter for ESP-01
![ESP-01 with USB Adapter](https://m.media-amazon.com/images/I/61bh6KPmE+L._AC_UF894,1000_QL80_.jpg)
- Sensor Suhu & Kelembaban (AHT10)

![AHT10](https://ezmation.com/101-medium_default/aht10-temperature-and-humidity-sensor-i2c.jpg)

Perlu sedikit modifikasi di esp01s biar fitur deep sleep bisa berjalan lancar (biar bisa bangun dari tidurnya), pastiin di esp01s Pin RESET ini disambung dengan Pin GPIO 16 biar nanti ketika udah habis waktu deep sleepnya esp01s bisa bangun terus ngirim data sensor lagi lalu masuk mode tidur lagi (deep sleep)
![image2](https://github.com/robertrullyp/ArduinoMQTTSensorAHTxxDeepSleep/assets/12167355/f6bc99c0-6d6c-42e1-a3a7-b7baf7d3ec35) 

Buat Wiring keseluruhannya gini : 

![image](https://github.com/robertrullyp/ArduinoMQTTSensorAHTxxDeepSleep/assets/12167355/c16bbd88-71eb-4100-9b8f-4f677045fa3a)


jadinya kira" gini sebelum masuk case 3d print :

![photo_2024-05-12_14-48-42](https://github.com/robertrullyp/ArduinoMQTTSensorAHTxxDeepSleep/assets/12167355/4401051e-c555-4c1e-ae8a-24bbe9a0a27d)


## 3D Print Case Model

Lagi di design, Update Soon....

## Library

Buat library tambahan yang perlu diinstal :

- [PubSubClient](https://github.com/knolleary/pubsubclient)
- [AHTxx](https://github.com/enjoyneering/AHTxx)


## Custom Parameter

rubah sendiri sesuaikn parameternya dengan parameter kalian :

    #define USE_SSL/TLS
    const char* ssid = "wifi_ssid";
    const char* password = "wifi_password";
    const char* user = "user_mqtt";
    const char* passwd = "password_mqtt";
    const char* mqtt_server = "server_mqtt";
    const char* mqtt_port = "8883";
    
    String PubTopic = "user_mqtt/state";
    String SubTopic = "user_mqtt/set";

di bagian #define USE_SSL/TLS jadiin comment aja kalau koneksi gak pengen pake SSL/TLS, misal mau pake buat konek ke server mqtt lokal punya sendiri.

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

Integrasi dengan Home Assistant :

configuration yaml kira" contohnya gini :

    - sensor:
      - name: "esp01s temperature"
        unique_id: esp01s_mqtt_temperature
        state_topic: "usertest/state/sensors/temperature"
        # availability_topic : "usertest/status"
        state_class: measurement
        device_class: "temperature"
      
      - name: "esp01s humidity"
        unique_id: esp01s_mqtt_humidity
        state_topic: "usertest/state/sensors/humidity"
        # availability_topic : "usertest/status"
        state_class: measurement
        device_class: "humidity"

![photo_2024-05-12_15-09-02](https://github.com/robertrullyp/ArduinoMQTTSensorAHTxxDeepSleep/assets/12167355/f78976de-d4cd-4d7a-a4a0-25eeff3e9049)



## Authors

- [Robert Rully Pernando](https://github.com/robertrullyp)

