#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESP32Servo.h>
#include <DHT.h>

// ✅ WiFi Credentials (Replace with your own network details)
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ✅ Web server and WebSocket
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// ✅ Device Definitions
#define FAN_PIN 22
#define LDRPIN 2
#define DHTPIN 27
#define MAGNETIC_SENSOR_door_PIN 4
#define MAGNETIC_SENSOR_CURTAIN_PIN 5
#define LED_PIN1 26  // Bathroom
#define LED_PIN2 25  // Kitchen
#define LED_PIN3 33  // Room
#define LED_PIN4 32  // Living Room 1
#define LED_PIN5 0   // Living Room 2
#define LED_PIN6 12  // Extra Living Room Light 1
#define LED_PIN7 13  // Extra Living Room Light 2

DHT my_sensor(DHTPIN, DHT22);
Servo servo1;
int motorA1 = 19;
int motorA2 = 21;

// ✅ State Variables                
int light_kitchen = 0;
int fan_kitchen = 0;
int light_room = 0;
int curtains_room = 0;
int light1_livingRoom = 0;
int light2_livingRoom = 0;
int light3_livingRoom = 0;
int light4_livingRoom = 0;
int main_door_livingRoom = 0;
int light_bathroom = 0;

// ✅ Curtain motor control variables without delay
bool curtainMoving = false;
unsigned long curtainStartTime = 0;
const unsigned long curtainDuration = 3000; // Curtain motor duration (3 seconds)
int curtainDirection = 0; // 1 = open, -1 = close, 0 = stop

void notifyClients() {
    DynamicJsonDocument json(512);
    json["temperature"] = my_sensor.readTemperature();
    json["humidity"] = my_sensor.readHumidity();
    json["ldr"] = analogRead(LDRPIN);
    json["door"] = digitalRead(MAGNETIC_SENSOR_door_PIN);
    json["curtain"] = digitalRead(MAGNETIC_SENSOR_CURTAIN_PIN);
    json["light_kitchen"] = digitalRead(LED_PIN2);
    json["fan_kitchen"] = digitalRead(FAN_PIN);
    json["light_room"] = digitalRead(LED_PIN3);
    json["curtains_room"] = digitalRead(MAGNETIC_SENSOR_CURTAIN_PIN);
    json["light1_livingRoom"] = light1_livingRoom;
    json["light2_livingRoom"] = light2_livingRoom;
    json["light3_livingRoom"] = digitalRead(LED_PIN6);
    json["light4_livingRoom"] = digitalRead(LED_PIN7);
    json["main_door_livingRoom"] = digitalRead(MAGNETIC_SENSOR_door_PIN);
    json["light_bathroom"] = digitalRead(LED_PIN1);

    String message;
    serializeJson(json, message);
    ws.textAll(message);
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                      void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("📡 WebSocket client %u connected\n", client->id());
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("📴 WebSocket client %u disconnected\n", client->id());
    }
}

// ✅ Function to execute multiple commands
void executeCommands(String commands) {
    while (commands.length() > 0) {
        int commaIndex = commands.indexOf(",");
        String command;
        if (commaIndex == -1) {
            command = commands;
            commands = "";
        } else {
            command = commands.substring(0, commaIndex);
            commands = commands.substring(commaIndex + 1);
        }
        command.trim();

        if (command == "open fan" || command == "open fan kitchen" ) {
            digitalWrite(FAN_PIN, HIGH);
            fan_kitchen = 1;
            Serial.println("🌬 Fan ON");
        } else if (command == "close fan" || command == "close fan kitchen" ) {
            digitalWrite(FAN_PIN, LOW);
            fan_kitchen = 0;
            Serial.println("🌬 Fan OFF");
        } else if (command == "open light bathroom" ) {
            digitalWrite(LED_PIN1, HIGH);
            light_bathroom = 1;
            Serial.println("💡 Bathroom Light ON");
        } else if (command == "close light bathroom" ) {
            digitalWrite(LED_PIN1, LOW);
            light_bathroom = 0;
            Serial.println("💡 Bathroom Light OFF");
        } else if (command == "open light kitchen" ) {
            digitalWrite(LED_PIN2, HIGH);
            light_kitchen = 1;
            Serial.println("💡 Kitchen Light ON");
        } else if (command == "close light kitchen" ) {
            digitalWrite(LED_PIN2, LOW);
            light_kitchen = 0;
            Serial.println("💡 Kitchen Light OFF");
        } else if (command == "open light room" ) {
            digitalWrite(LED_PIN3, HIGH);
            light_room = 1;
            Serial.println("💡 Room Light ON");
        } else if (command == "close light room" ) {
            digitalWrite(LED_PIN3, LOW);
            light_room = 0;
            Serial.println("💡 Room Light OFF");
        } else if (command == "open light living room" ) {
            digitalWrite(LED_PIN4, HIGH);
            digitalWrite(LED_PIN5, HIGH);
            light1_livingRoom = 1;
            light2_livingRoom = 1;
            Serial.println("💡 Living Room Light ON");
        } else if (command == "close light living room" ) {
            digitalWrite(LED_PIN4, LOW);
            digitalWrite(LED_PIN5, LOW);
            light1_livingRoom = 0;
            light2_livingRoom = 0;
            Serial.println("💡 Living Room Light OFF");
        } else if (command == "increase light living room" ) {
            digitalWrite(LED_PIN6, HIGH);
            digitalWrite(LED_PIN7, HIGH);
            light3_livingRoom = 1;
            light4_livingRoom = 1;
            Serial.println("💡 Living Room Brightness Increased");
        } else if (command == "decrease light living room" ) {
            digitalWrite(LED_PIN6, LOW);
            digitalWrite(LED_PIN7, LOW);
            light3_livingRoom = 0;
            light4_livingRoom = 0;
            Serial.println("💡 Living Room Brightness Decreased");
        } else if (command == "open curtain" || command == "open curtain room" ) {
            if (!curtainMoving) {
                digitalWrite(motorA1, HIGH);
                digitalWrite(motorA2, LOW);
                curtainMoving = true;
                curtainStartTime = millis();
                curtainDirection = 1;
                Serial.println("🪟 Curtain Opening");
            }
        } else if (command == "close curtain room" || command == "close curtain" ) {
            if (!curtainMoving) {
                digitalWrite(motorA1, LOW);
                digitalWrite(motorA2, HIGH);
                curtainMoving = true;
                curtainStartTime = millis();
                curtainDirection = -1;
                Serial.println("🪟 Curtain Closing");
            }
        } else if (command == "open door" ||command == "open door living Room" ) {
            servo1.write(250);
            main_door_livingRoom = 1;
            Serial.println("🚪 Door Opening");
        } else if (command == "close door living Room" ||command == "close door" ) {
            servo1.write(0);
            main_door_livingRoom = 0;
            Serial.println("🚪 Door Closing");
        }
    }
    notifyClients(); // Send updates after command execution
}

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n✅ WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    servo1.attach(14);
    my_sensor.begin();

    pinMode(FAN_PIN, OUTPUT);
    pinMode(MAGNETIC_SENSOR_door_PIN, INPUT);
    pinMode(MAGNETIC_SENSOR_CURTAIN_PIN, INPUT);
    pinMode(motorA1, OUTPUT);
    pinMode(motorA2, OUTPUT);
    pinMode(LED_PIN1, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
    pinMode(LED_PIN3, OUTPUT);
    pinMode(LED_PIN4, OUTPUT);
    pinMode(LED_PIN5, OUTPUT);
    pinMode(LED_PIN6, OUTPUT);
    pinMode(LED_PIN7, OUTPUT);
    
    ws.onEvent(onWebSocketEvent);
    server.addHandler(&ws);

    server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("command")) {
            String command = request->getParam("command")->value();
            executeCommands(command);
            request->send(200, "text/plain", "Command executed");
        } else {
            request->send(400, "text/plain", "Invalid request");
        }
    });

    server.begin();
}

void loop() {
    // Curtain state check
    if (curtainMoving && (millis() - curtainStartTime >= curtainDuration)) {
        digitalWrite(motorA1, LOW);
        digitalWrite(motorA2, LOW);
        curtainMoving = false;
        curtainDirection = 0;
        Serial.println("🪟 Curtain Stopped");
        notifyClients();
    }

    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        executeCommands(command);
    }
    notifyClients();
    delay(1000); // Small delay to improve responsiveness
}