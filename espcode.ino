#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <U8g2lib.h>
#include <Wire.h>

// =====================================================
// WIFI
// =====================================================

const char *ssid = "Greenhouse";
const char *password = "12345678";

ESP8266WebServer server(80);

// =====================================================
// OLED SETUP
// =====================================================

U8G2_SSD1306_128X64_NONAME_F_HW_I2C
u8g2(U8G2_R0, U8X8_PIN_NONE);

// =====================================================
// VARIABLES
// =====================================================

String temperature = "--";
String humidity = "--";
String soil = "--";
String lightVal = "--";
String rain = "--";

String pump = "OFF";
String fan = "OFF";
String grow = "OFF";

// =====================================================
// OLED UPDATE FUNCTION
// =====================================================

void updateOLED()
{

    u8g2.clearBuffer();

    // ================= HEADER =================

    u8g2.setFont(u8g2_font_5x8_tf);

    u8g2.drawBox(0, 0, 128, 10);

    u8g2.setDrawColor(0);

    u8g2.drawStr(18, 8, "SMART GREENHOUSE");

    u8g2.setDrawColor(1);

    // ================= TEMPERATURE =================

    char buf[32];

    sprintf(buf, "T:%sC", temperature.c_str());

    u8g2.drawStr(2, 20, buf);

    // ================= HUMIDITY =================

    sprintf(buf, "H:%s%%", humidity.c_str());

    u8g2.drawStr(68, 20, buf);

    // ================= SOIL =================

    if (soil.toInt() > 700)
        sprintf(buf, "SOIL:DRY");
    else
        sprintf(buf, "SOIL:WET");

    u8g2.drawStr(2, 32, buf);

    // ================= LIGHT =================

    if (lightVal.toInt() > 500)
        sprintf(buf, "LDR:DARK");
    else
        sprintf(buf, "LDR:BRIGHT");

    u8g2.drawStr(2, 44, buf);

    // ================= RAIN =================

    if (rain.toInt() < 500)
        sprintf(buf, "RAIN:YES");
    else
        sprintf(buf, "RAIN:NO");

    u8g2.drawStr(2, 56, buf);

    // ================= PUMP =================

    if (pump == "ON")
        sprintf(buf, "PUMP:ON");
    else
        sprintf(buf, "PUMP:OFF");

    u8g2.drawStr(70, 32, buf);

    // ================= FAN =================

    if (fan == "ON")
        sprintf(buf, "FAN:ON");
    else
        sprintf(buf, "FAN:OFF");

    u8g2.drawStr(70, 56, buf);

    // ================= GROW LIGHT =================

    if (grow == "ON")
        sprintf(buf, "LED:ON");
    else
        sprintf(buf, "LED:OFF");

    u8g2.drawStr(70, 44, buf);

    // ================= FRAME =================

    u8g2.drawFrame(0, 10, 128, 54);

    // ================= SEND =================

    u8g2.sendBuffer();
}

// =====================================================
// JSON API
// =====================================================

void handleData()
{

    String json = "{";

    json += "\"temperature\":\"" + temperature + "\",";
    json += "\"humidity\":\"" + humidity + "\",";
    json += "\"soil\":\"" + soil + "\",";
    json += "\"light\":\"" + lightVal + "\",";
    json += "\"rain\":\"" + rain + "\",";
    json += "\"pump\":\"" + pump + "\",";
    json += "\"fan\":\"" + fan + "\",";
    json += "\"grow\":\"" + grow + "\"";

    json += "}";

    server.send(200, "application/json", json);
}

// =====================================================
// DASHBOARD PAGE
// =====================================================

void handleRoot()
{

    String page = R"=====(

<!DOCTYPE html>
<html>

<head>

<meta charset="UTF-8">

<title>Smart Greenhouse</title>

<meta name="viewport" content="width=device-width, initial-scale=1.0">

<style>

*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:Arial;
}

body{
background:#e7dece;
min-height:100vh;
color:#2b2b2b;
padding-bottom:40px;
}

.header{
padding:28px;
font-size:36px;
font-weight:bold;
display:flex;
justify-content:space-between;
align-items:center;
background:#efe6d6;
border-bottom:1px solid rgba(0,0,0,0.08);
}

.live{
background:#1f5134;
color:white;
padding:10px 18px;
border-radius:50px;
font-size:16px;
display:flex;
align-items:center;
gap:10px;
}

.dot{
width:10px;
height:10px;
background:#80ed99;
border-radius:50%;
animation:pulse 1s infinite;
}

@keyframes pulse{
0%{opacity:1;}
50%{opacity:0.3;}
100%{opacity:1;}
}

.container{
display:grid;
grid-template-columns:
repeat(auto-fit,minmax(260px,1fr));
gap:22px;
padding:25px;
}

.card{
background:#efe6d6;
padding:24px;
border-radius:24px;
border:1px solid rgba(0,0,0,0.08);
box-shadow:0 2px 8px rgba(0,0,0,0.06);
transition:0.3s;
}

.card:hover{
transform:translateY(-5px);
}

.label{
font-size:12px;
font-weight:bold;
letter-spacing:2px;
margin-bottom:18px;
color:#6b6b6b;
}

.big{
font-size:36px;
font-weight:bold;
margin-bottom:10px;
}

.small{
font-size:14px;
color:#777;
}

.on{
background:#1f5134;
color:white;
padding:10px 18px;
border-radius:999px;
display:inline-block;
font-weight:bold;
}

.off{
background:#c96b22;
color:white;
padding:10px 18px;
border-radius:999px;
display:inline-block;
font-weight:bold;
}

.insightBox{
margin:25px;
background:#efe6d6;
padding:25px;
border-radius:24px;
border:1px solid rgba(0,0,0,0.08);
}

.insight{
padding:14px;
margin-top:12px;
border-radius:14px;
background:#e7dece;
font-size:16px;
}

</style>

</head>

<body>

<div class="header">

<div>
 Smart Greenhouse
</div>

<div class="live">
<div class="dot"></div>
LIVE
</div>

</div>

<div class="container">

<!-- TEMPERATURE -->

<div class="card">

<div class="label">
TEMPERATURE
</div>

<div class="big" id="temp"
style="color:#c96b22;">
--
</div>

<div class="small">
Greenhouse temperature
</div>

</div>

<!-- HUMIDITY -->

<div class="card">

<div class="label">
HUMIDITY
</div>

<div class="big" id="humidity"
style="color:#3d5a80;">
--
</div>

<div class="small">
Air moisture level
</div>

</div>

<!-- SOIL -->

<div class="card">

<div class="label">
SOIL MOISTURE
</div>

<div class="big" id="soilStatus">
--
</div>

<div class="small">
Value:
<span id="soilValue">--</span>
</div>

</div>

<!-- LIGHT -->

<div class="card">

<div class="label">
LIGHT CONDITION
</div>

<div class="big" id="lightStatus">
--
</div>

<div class="small">
Value:
<span id="lightValue">--</span>
</div>

</div>

<!-- RAIN -->

<div class="card">

<div class="label">
RAIN STATUS
</div>

<div class="big" id="rainStatus">
--
</div>

<div class="small">
Value:
<span id="rainValue">--</span>
</div>

</div>

<!-- PUMP -->

<div class="card">

<div class="label">
WATER PUMP
</div>

<div id="pump" class="off">
OFF
</div>

</div>

<!-- FAN -->

<div class="card">

<div class="label">
COOLING FAN
</div>

<div id="fan" class="off">
OFF
</div>

</div>

<!-- GROW LIGHT -->

<div class="card">

<div class="label">
GROW LIGHT
</div>

<div id="grow" class="off">
OFF
</div>

</div>

</div>

<!-- INSIGHTS -->

<div class="insightBox">

<div class="label">
SMART INSIGHTS
</div>

<div id="insights">

<div class="insight">
Waiting for sensor data...
</div>

</div>

</div>

<script>

async function updateData() {

const response = await fetch('/data');

const data = await response.json();

// ================= TEMPERATURE =================

document.getElementById('temp').innerHTML =
data.temperature + "&#176;C";

// ================= HUMIDITY =================

document.getElementById('humidity').innerHTML =
data.humidity + "%";

// ================= SOIL =================

document.getElementById('soilValue').innerHTML =
data.soil;

let soilStatus =
document.getElementById('soilStatus');

if(parseInt(data.soil) > 700){

soilStatus.innerHTML = "DRY";
soilStatus.style.color = "#d62828";

}else{

soilStatus.innerHTML = "WET";
soilStatus.style.color = "#2a9d8f";
}

// ================= LIGHT =================

document.getElementById('lightValue').innerHTML =
data.light;

let lightStatus =
document.getElementById('lightStatus');

if(parseInt(data.light) > 500){

lightStatus.innerHTML = "DARK";
lightStatus.style.color = "#5e548e";

}else{

lightStatus.innerHTML = "BRIGHT";
lightStatus.style.color = "#f4a261";
}

// ================= RAIN =================

document.getElementById('rainValue').innerHTML =
data.rain;

let rainStatus =
document.getElementById('rainStatus');

if(parseInt(data.rain) < 500){

rainStatus.innerHTML = "RAINING";
rainStatus.style.color = "#0077b6";

}else{

rainStatus.innerHTML = "CLEAR";
rainStatus.style.color = "#bc6c25";
}

// ================= PUMP =================

let pump =
document.getElementById('pump');

pump.innerHTML = data.pump;

if(data.pump == "ON"){

pump.className = "on";

}else{

pump.className = "off";
}

// ================= FAN =================

let fan =
document.getElementById('fan');

fan.innerHTML = data.fan;

if(data.fan == "ON"){

fan.className = "on";

}else{

fan.className = "off";
}

// ================= GROW LIGHT =================

let grow =
document.getElementById('grow');

grow.innerHTML = data.grow;

if(data.grow == "ON"){

grow.className = "on";

}else{

grow.className = "off";
}

// ================= INSIGHTS =================

let insights = "";

if(parseInt(data.soil) > 700){

insights += `
<div class="insight">
 Plants require irrigation
</div>
`;

}else{

insights += `
<div class="insight">
 Soil condition healthy
</div>
`;
}

if(parseFloat(data.temperature) > 32){

insights += `
<div class="insight">
🌡 High temperature detected
</div>
`;
}

if(parseInt(data.rain) < 500){

insights += `
<div class="insight">
 Rainfall detected
</div>
`;
}

if(data.pump == "ON"){

insights += `
<div class="insight">
 Irrigation system active
</div>
`;
}

if(insights == ""){

insights = `
<div class="insight">
 All systems operating normally
</div>
`;
}

document.getElementById('insights').innerHTML =
insights;
}

setInterval(updateData, 2000);

updateData();

</script>

</body>
</html>

)=====";

    server.send(200, "text/html", page);
}

// =====================================================
// SETUP
// =====================================================

void setup()
{

    Serial.begin(9600);

    // ================= OLED =================

    Wire.begin(D2, D1);

    u8g2.begin();

    // ================= WIFI =================

    WiFi.softAP(ssid, password);

    Serial.println();
    Serial.println("================================");
    Serial.println("SMART GREENHOUSE SYSTEM");
    Serial.println("================================");

    Serial.print("WiFi Name: ");
    Serial.println(ssid);

    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    // ================= SERVER =================

    server.on("/", handleRoot);

    server.on("/data", handleData);

    server.begin();

    Serial.println("Web Server Started");

    updateOLED();
}

// =====================================================
// LOOP
// =====================================================

void loop()
{

    server.handleClient();

    if (Serial.available())
    {

        String data = Serial.readStringUntil('\n');

        Serial.println("================================");
        Serial.println("RAW DATA RECEIVED:");
        Serial.println(data);

        int tIndex = data.indexOf("T:");
        int hIndex = data.indexOf(",H:");
        int sIndex = data.indexOf(",S:");
        int lIndex = data.indexOf(",L:");
        int rIndex = data.indexOf(",R:");
        int pIndex = data.indexOf(",P:");
        int fIndex = data.indexOf(",F:");
        int gIndex = data.indexOf(",G:");

        if (tIndex != -1 &&
            hIndex != -1 &&
            sIndex != -1 &&
            lIndex != -1 &&
            rIndex != -1 &&
            pIndex != -1 &&
            fIndex != -1 &&
            gIndex != -1)
        {

            // ================= PARSE =================

            temperature = data.substring(tIndex + 2, hIndex);

            humidity = data.substring(hIndex + 3, sIndex);

            soil = data.substring(sIndex + 3, lIndex);

            lightVal = data.substring(lIndex + 3, rIndex);

            rain = data.substring(rIndex + 3, pIndex);

            pump = data.substring(pIndex + 3, fIndex);

            fan = data.substring(fIndex + 3, gIndex);

            grow = data.substring(gIndex + 3);

            // ================= CLEAN DATA =================

            temperature.trim();
            humidity.trim();
            soil.trim();
            lightVal.trim();
            rain.trim();

            pump.trim();
            fan.trim();
            grow.trim();

            // ================= SERIAL OUTPUT =================

            Serial.println("PARSED VALUES");

            Serial.print("Temperature: ");
            Serial.println(temperature);

            Serial.print("Humidity: ");
            Serial.println(humidity);

            Serial.print("Soil Moisture: ");
            Serial.println(soil);

            Serial.print("Light Intensity: ");
            Serial.println(lightVal);

            Serial.print("Rain Sensor: ");
            Serial.println(rain);

            Serial.print("Pump Status: ");
            Serial.println(pump);

            Serial.print("Fan Status: ");
            Serial.println(fan);

            Serial.print("Grow Light Status: ");
            Serial.println(grow);

            Serial.println("================================");

            // ================= UPDATE OLED =================

            updateOLED();
        }
        else
        {

            Serial.println("DATA PARSE ERROR");
        }
    }
}