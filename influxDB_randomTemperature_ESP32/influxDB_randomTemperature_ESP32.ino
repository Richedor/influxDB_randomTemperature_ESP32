#include <WiFi.h>
#include <InfluxDbClient.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "PUCbT1zgHeE5HjUs7vFs2bCMo9KduUl26emitlEL7ZOQbDjxOiJNykoZwIAL-JAZaeEuMUMs-37MvLfpLC9Byw=="
#define INFLUXDB_ORG "24355fe20890a9f8"
#define INFLUXDB_BUCKET "ESP32"

#define TZ_INFO "UTC1"

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

Point temperature("temperature");

void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println("Connected to WiFi");

    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

    if (client.validateConnection()) {
        Serial.print("Connected to InfluxDB: ");
        Serial.println(client.getServerUrl());
    } else {
        Serial.print("InfluxDB connection failed: ");
        Serial.println(client.getLastErrorMessage());
    }
}

void loop() {
    // Lire la température à partir du capteur
    float temperatureValue = lireTemperature();

    if (!isnan(temperatureValue)) {
        // Envoi de la température à InfluxDB avec le label "temperature"
        temperature.clearFields();
        temperature.addField("value", temperatureValue);
        client.writePoint(temperature);
        Serial.println("Temperature sent to InfluxDB");
    } else {
        Serial.println("Error reading temperature");
    }

    delay(1000); // Attendre une seconde avant de prendre la prochaine mesure
}

float lireTemperature() {
    // Générer une valeur de température aléatoire entre -10°C et 40°C
    return random(-100, 401) / 10.0;
}
