// SmartTouch Attendance Logger 

#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <RTClib.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Wi-Fi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";


// Firebase Realtime Database URL
const String firebase_host = "YOUR-FIREBASE-HOST";

// LCD 16x2 I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RTC Module
RTC_DS3231 rtc;

// Pin configuration
#define TOUCH_PIN 15
#define BUZZER_PIN 12
#define GREEN_LED 13

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {4, 27, 26, 25};
byte colPins[COLS] = {33, 32, 19, 18};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ID to name mapping
struct Student {
String id;
String name;
};
Student students[] = {
{"TP085799", "Mohammad"},
{"TP012345", "USER"},
};

String currentID = "";
bool waitingForTouch = false;

void setup() {
Serial.begin(115200);
pinMode(TOUCH_PIN, INPUT);
pinMode(BUZZER_PIN, OUTPUT);
pinMode(GREEN_LED, OUTPUT);

Wire.begin(21, 22);
lcd.init();
lcd.backlight();

WiFi.begin(ssid, password);
lcd.setCursor(0, 0);
lcd.print("Connecting WiFi");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

lcd.clear();
lcd.print("WiFi Connected");

if (!rtc.begin(&Wire)) {
lcd.clear();
lcd.print("RTC ERROR");
while (true);
}
// SET RTC TIME ONCE (then comment this out)
rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));


lcd.clear();
lcd.print("Enter ID:");
}

void loop() {
char key = keypad.getKey();
if (key && !waitingForTouch) {
if (key == '#') {
waitingForTouch = true;
lcd.clear();
lcd.print("Touch to confirm");
} else if (key == '*') {
currentID = "";
lcd.clear();
lcd.print("Enter ID:");
} else if (isDigit(key)) {
currentID += key;
lcd.setCursor(0, 1);
lcd.print(currentID);
}
}

if (waitingForTouch && digitalRead(TOUCH_PIN) == HIGH) {
String fullID = "TP" + currentID;
logAttendance(fullID);
currentID = "";
waitingForTouch = false;
delay(1000);
lcd.clear();
lcd.print("Enter ID:");
}
}

void logAttendance(String id) {
DateTime now = rtc.now();
String today = String(now.year()) + "-" + (now.month() < 10 ? "0" : "") + String(now.month()) + "-" + (now.day() < 10 ? "0" : "") + String(now.day());
String timeStr = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

String name = "";
bool found = false;

for (Student s : students) {
if (s.id == id) {
name = s.name;
found = true;
break;
}
}

if (!found) {
lcd.clear();
lcd.print("ID Not Found");
tone(BUZZER_PIN, 300, 600);
delay(2000);
return;
}

String path = "/attendance/" + id + "/" + today + ".json";
String url = firebase_host + path;

HTTPClient http;
http.begin(url);
int httpCode = http.GET();
if (httpCode == 200 && http.getString().indexOf("time") > -1) {
lcd.clear();
lcd.print("Already Checked In");
tone(BUZZER_PIN, 500, 500);
http.end();
delay(2000);
return;
}
http.end();

http.begin(url);
http.addHeader("Content-Type", "application/json");

DynamicJsonDocument doc(256);
doc["name"] = name;
doc["time"] = timeStr;
doc["status"] = "Checked In";

String requestBody;
serializeJson(doc, requestBody);
httpCode = http.PUT(requestBody);

if (httpCode == 200 || httpCode == 204) {
lcd.clear();
lcd.print("Attendance Done");
lcd.setCursor(0, 1);
lcd.print("Welcome " + name);
digitalWrite(GREEN_LED, HIGH);
tone(BUZZER_PIN, 1000, 300);
delay(2000);
digitalWrite(GREEN_LED, LOW);
} else {
lcd.clear();
lcd.print("Log Failed!");
tone(BUZZER_PIN, 100, 100);
delay(2000);
}

http.end();
}