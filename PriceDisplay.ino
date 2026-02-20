#include <WiFi.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>
#include <ArduinoJson.h>

// ============== CONFIGURE THESE ==============
const char* WIFI_SSID = "Phone";
const char* WIFI_PASSWORD = "EV1234567";
const char* SERVER_IP = "172.20.10.2";
// const char* WIFI_SSID = "SwayamAarav-deco";
// const char* WIFI_PASSWORD = "SwayamAarav@2016";
// const char* SERVER_IP = "192.168.68.62";
const int SERVER_PORT = 3000;
const int DISPLAY_ID = 1;  // Change this for each display (1-10)
const int UPDATE_INTERVAL = 30000;  // Check for updates every 30 seconds
// =============================================

TFT_eSPI tft = TFT_eSPI();

String currentName = "";
float currentPrice = 0.0;

void setup() {
    Serial.begin(115200);
    
    // Initialize display
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_WHITE);
    
    // Show connecting message
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("Connecting to WiFi...");
    
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        
        tft.fillScreen(TFT_WHITE);
        tft.setCursor(10, 10);
        tft.println("WiFi Connected!");
        delay(1000);
    } else {
        tft.fillScreen(TFT_RED);
        tft.setTextColor(TFT_WHITE, TFT_RED);
        tft.setCursor(10, 10);
        tft.println("WiFi Failed!");
        while(1) delay(1000);
    }
    
    // Fetch initial price
    fetchPrice();
}

void loop() {
    fetchPrice();
    delay(UPDATE_INTERVAL);
}

void fetchPrice() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected, reconnecting...");
        WiFi.reconnect();
        delay(5000);
        return;
    }
    
    HTTPClient http;
    String url = "http://" + String(SERVER_IP) + ":" + String(SERVER_PORT) + "/api/displays/" + String(DISPLAY_ID);
    
    Serial.print("Fetching: ");
    Serial.println(url);
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode == 200) {
        String payload = http.getString();
        Serial.println("Response: " + payload);
        
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);
        
        if (!error) {
            String newName = doc["name"].as<String>();
            float newPrice = doc["price"].as<float>();
            
            // Only update display if something changed
            if (newName != currentName || newPrice != currentPrice) {
                currentName = newName;
                currentPrice = newPrice;
                updateDisplay();
            }
        } else {
            Serial.print("JSON parse error: ");
            Serial.println(error.c_str());
        }
    } else {
        Serial.print("HTTP error: ");
        Serial.println(httpCode);
    }
    
    http.end();
}

void updateDisplay() {
    tft.fillScreen(TFT_WHITE);
    
    // Draw item name
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextSize(3);
    
    // Center the name
    int nameWidth = currentName.length() * 18;
    int nameX = (tft.width() - nameWidth) / 2;
    if (nameX < 5) nameX = 5;
    
    tft.setCursor(nameX, 30);
    tft.println(currentName);
    
    // Draw price
    tft.setTextColor(TFT_DARKGREEN, TFT_WHITE);
    tft.setTextSize(5);
    
    String priceStr = "$" + String(currentPrice, 2);
    int priceWidth = priceStr.length() * 30;
    int priceX = (tft.width() - priceWidth) / 2;
    if (priceX < 5) priceX = 5;
    
    tft.setCursor(priceX, 80);
    tft.println(priceStr);
}
