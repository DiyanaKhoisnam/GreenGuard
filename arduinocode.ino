#include <DHT.h>
#include <SoftwareSerial.h>

// ================= SERIAL =================
SoftwareSerial espSerial(2, 3);
// D2 <- ESP TX
// D3 -> ESP RX

// ================= DHT11 =================
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// ================= SENSOR PINS =================
#define SOIL_PIN A0
#define LDR_PIN A1
#define RAIN_PIN A2

// ================= RELAY PINS =================
#define PUMP_RELAY 8
#define FAN_RELAY 9
#define LIGHT_RELAY 10

void setup()
{

    Serial.begin(9600);

    espSerial.begin(9600);

    dht.begin();

    pinMode(PUMP_RELAY, OUTPUT);
    pinMode(FAN_RELAY, OUTPUT);
    pinMode(LIGHT_RELAY, OUTPUT);

    // Relays OFF initially
    digitalWrite(PUMP_RELAY, HIGH);
    digitalWrite(FAN_RELAY, HIGH);
    digitalWrite(LIGHT_RELAY, HIGH);

    Serial.println("================================");
    Serial.println("SMART GREENHOUSE SYSTEM STARTED");
    Serial.println("================================");
}

void loop()
{

    // ================= READ DHT11 =================
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity))
    {

        Serial.println("DHT SENSOR ERROR");

        delay(2000);

        return;
    }

    // ================= READ SENSORS =================
    int soilValue = analogRead(SOIL_PIN);

    int lightValue = analogRead(LDR_PIN);

    int rainValue = analogRead(RAIN_PIN);

    // ================= STATUS VARIABLES =================
    String pumpStatus = "OFF";

    String fanStatus = "OFF";

    String lightStatus = "OFF";

    // ================= PUMP CONTROL =================
    // Pump ON only if soil dry and no rain

    if (soilValue > 700 && rainValue > 500)
    {

        digitalWrite(PUMP_RELAY, LOW);

        pumpStatus = "ON";
    }
    else
    {

        digitalWrite(PUMP_RELAY, HIGH);
    }

    // ================= FAN CONTROL =================

    if (temperature > 25)
    {

        digitalWrite(FAN_RELAY, LOW);

        fanStatus = "ON";
    }
    else
    {

        digitalWrite(FAN_RELAY, HIGH);
    }

    // ================= LIGHT CONTROL =================

    if (lightValue > 500)
    {

        digitalWrite(LIGHT_RELAY, LOW);

        lightStatus = "ON";
    }
    else
    {

        digitalWrite(LIGHT_RELAY, HIGH);
    }

    // ================= SERIAL MONITOR OUTPUT =================

    Serial.println("================================");

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Soil Moisture: ");
    Serial.println(soilValue);

    Serial.print("Light Intensity: ");
    Serial.println(lightValue);

    Serial.print("Rain Sensor Value: ");
    Serial.println(rainValue);

    Serial.print("Pump Status: ");
    Serial.println(pumpStatus);

    Serial.print("Fan Status: ");
    Serial.println(fanStatus);

    Serial.print("Grow Light Status: ");
    Serial.println(lightStatus);

    // ================= SEND DATA TO ESP =================

    espSerial.print("T:");
    espSerial.print(temperature);

    espSerial.print(",H:");
    espSerial.print(humidity);

    espSerial.print(",S:");
    espSerial.print(soilValue);

    espSerial.print(",L:");
    espSerial.print(lightValue);

    espSerial.print(",R:");
    espSerial.print(rainValue);

    espSerial.print(",P:");
    espSerial.print(pumpStatus);

    espSerial.print(",F:");
    espSerial.print(fanStatus);

    espSerial.print(",G:");
    espSerial.println(lightStatus);

    espSerial.flush();

    Serial.println("DATA SENT TO ESP8266");

    Serial.println("================================");

    delay(2000);
}