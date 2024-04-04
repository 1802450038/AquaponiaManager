// Import required libraries
#include <Arduino.h>
// Web And WIFI
#include "WiFi.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>

// File and Memory
#include <FS.h>
#include <SD.h>
#include <SPI.h>

// Display
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <OneWire.h>

// Rotary And Button
#include <OneButton.h>
#include <AiEsp32RotaryEncoder.h>

// Sensors
#include <DallasTemperature.h>
#include <NewPing.h>

// MULTI CORE
TaskHandle_t SendToSQL;

// Display And Menu
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ENCODER
int portsEncoder[4] = {13, 14, 27, 5};

// Ports Reference on array
// portsEncoder[0] => 13 => BTN
// portsEncoder[1] => 14 => DT
// portsEncoder[2] => 27 => CLK
// portsEncoder[3] => 5 => Number of steps
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(portsEncoder[1], portsEncoder[2], portsEncoder[0], -1, portsEncoder[3]);

// Control BUTTON ROTARY ENCODER
OneButton button(portsEncoder[0], true);

// ENCODER - END

// WIFI MANAGER AND SERVER VARIABLES

const unsigned char pix[] PROGMEM = {0x00, 0xff, 0x00, 0x7e, 0x00, 0x18, 0x00, 0x00}; // Wifi ico
const char *PARAM_INPUT_1 = "ssid";
const char *PARAM_INPUT_2 = "pass";
String ssid;
String pass;
bool isWifiCon = false;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char *serverName = "http://177.36.44.91/aquatest/sender.php";
String apiKeyValue = "tPmAT5Ab3j7F9";

const char *ssidPath = "/wifi/ssid.txt";
const char *passPath = "/wifi/pass.txt";
const char *preferences = "/preferences_board.txt";

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000; // interval to wait for Wi-Fi connection (milliseconds)

// WIFI MANAGER AND SERVER VARIABLES - END

// SENSORS VARIABLES

// TEMP
int portsRED[2] = {25};
// SONAR
int portsBLUE[2] = {32, 33};
// PH
int portsGREEN[1] = {34};
// RELAY
int portsPURPLE[1] = {12};

// Pinout COLOR reference SERIAL PORT
// PORTS USED
// Relay     => 12
// Ph        => 25
// Sonar     => 32 33
// Temp      => 34
// Encoder   => 13,14,27,5
// SD Reader => 5,23,19,18
//  ****************************
//  *   ( ) ( ) ( ) ( ) ( )   *
//   *   5   4   3   2   1   *
//    *   ( ) ( ) ( ) ( )   *
//     *   6   7   8   9   *
//      ******************* <-- ( ) 10

// Ultrassonic variables
// Blue
const int echoPin = portsBLUE[0];
const int trigPin = portsBLUE[1];
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
long duration;
float distanceCm;

// Relay variables
// Purple
bool relayState = false;
String relayStateText;

// PH Sensor variables
// Green
// float ph;
float calibration_value = 20.24 - 0.7; // 21.34 - 0.7
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;
float ph_act;

// Temperature variables
// Red
OneWire oneWire(portsRED[0]);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

// SENSORS VARIABLES - END

// MENU VARIABLES
int menuPos = 1;
int menuPage = 1;
int itemsPerPage = 4;
int activeLang = 0;
int subMenuPos = 0;
boolean subMenuActive = false;
unsigned long millisTarefa1 = millis();
unsigned long millisTarefa2 = millis();

// Menu text prompts
String menus[3][8] = {
    {"Readings", "Temperature", "Relay", "Water level", "Ph", "WiFi configuration", "Select language", "About"},
    {"Leituras", "Temperatura", "Rele", "Nivel da agua", "Ph", "Configuracao Wi-fi", "Selecionar idioma", "Sobre"},
    {"Lecturas", "Temperatura", "Rele", "Nivel de agua", "Ph", "Configuracion WiFi", "Seleccione idioma", "Acerca"}};

String menusTitle[3][8] = {
    {"Readings", "Temp", "Relay", "Level", "Ph", "WiFi conf", "Sel language", "About"},
    {"Leituras", "Temp", "Rele", "Nivel", "Ph", "Conf Wi-fi", "Sel idioma", "Sobre"},
    {"Lecturas", "Temp", "Relé", "Nivel", "Ph", "Conf WiFi", "Sel el idioma", "Acerca de"}};

String menuTipMessage[3] = {"<- Hold to return", "<- Segure p/ voltar", "<- Mantener p/ regresar"};

String wifiCleanMessage[3] = {
    "Cleaning WiFi",
    "Limpando WiFi",
    "Limpieza WiFi",
};

String resetMessage[3] = {"Restarting", "Reiniciando", "Reiniciando"};

String menuActionTipMessage[3] = {"<- 2x clicks to action", "<- 2x cliques p acao", "<- 2x clics p la accion"};

String menuStateMessage[2][3] = {{"ON", "Ligado", "Activo"}, {"OFF", "Desligado", "Apagado"}};

String readingsPage[3][4] = {{"Temp", "PH", "Level", "Relay"}, {"Temp", "PH", "Nivel", "Rele"}, {"Temp", "PH", "Nivel", "Rele"}};

// MENU VARIABLES - END

// =======================
// = Functions Interface =
// =======================

// Multi Core functions
void SendToSQLCode(void *pvParameters);

// Display and Menu Functions
void initLcd();
void printLine(int, String, bool);
void showTip(String);
void controlPage(int);
String getMenuItem(int, String);
void toggleLanguage();
void menu(int, int);
void fillGraph(int, int, int);
void generateGraph(int, int, String, int);
void actions(int);
void updateDisplay();
void updateData();

// Wifi Functions
bool initWifi();
void printWifiStatus(bool);
void clearWifi();

// Rotaty and Button Functions
void initRotary();
void initButton();
void btnClick();
void btnDoubleClick();
void btnLongPress();
void rotaryLoop();
void IRAM_ATTR readEncoderISR();

// Sensors Functions
void initPorts();
float getPH(bool);
float getTemperature();
float getDistance();
void toggleRelay(String);

// WebServer Functions
String getRelayState();
String processorRelay();
String processorTemp();
String processorPh();
String processorDistance();
void sendToServer();

// SD and Files Functions
void initSDCard();
void writeFile(fs::FS &fs, const char *path, const char *message);
String readFile(fs::FS &fs, const char *path, bool singleLine);
void loadVariablesFromSDCard();

// Setup Functions
void setupWifiAndServer();

// ======================MULTI CORE PROCESSOR======================

void SendToSQLCode(void *pvParameters)
{
  for (;;)
  {
    sendToServer();
    delay(60000);
  }
}

// ======================DISPLAY AND MENU======================

void initLcd()
{
  if (!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    for (;;)
      ;
  }
}

void printLine(int line, String text, bool selected)
{
  lcd.setTextColor(WHITE);
  if (line == 0)
  {
    lcd.fillRect(0, 0, 128, 16, BLACK);
    lcd.fillRect(0, 0, 128, 16, WHITE);
    lcd.setTextColor(BLACK);
    lcd.setTextSize(2);
    lcd.setCursor(2, 1);
    lcd.println(text);
  }
  else if (line == 1)
  {
    lcd.fillRect(0, 16, 128, 12, BLACK);
    lcd.setTextSize(1);
    lcd.setCursor(2, 18);
    lcd.println(text);
    if (selected == true)
    {
      lcd.drawRect(0, 16, 128, 12, WHITE);
    }
  }
  else if (line == 2)
  {
    lcd.fillRect(0, 28, 128, 12, BLACK);
    lcd.setTextSize(1);
    lcd.setCursor(2, 30);
    lcd.println(text);
    if (selected == true)
    {
      lcd.drawRect(0, 28, 128, 12, WHITE);
    }
  }
  else if (line == 3)
  {
    lcd.fillRect(0, 40, 128, 12, BLACK);
    lcd.setTextSize(1);
    lcd.setCursor(2, 42);
    lcd.println(text);
    if (selected == true)
    {
      lcd.drawRect(0, 40, 128, 12, WHITE);
    }
  }
  else if (line == 4)
  {
    lcd.fillRect(0, 52, 128, 12, BLACK);
    lcd.setTextSize(1);
    lcd.setCursor(2, 54);
    lcd.println(text);
    if (selected == true)
    {
      lcd.drawRect(0, 52, 128, 12, WHITE);
    }
  }
  // lcd.display();
}

void showTip(String text)
{
  lcd.fillRect(0, 52, 128, 12, BLACK);
  lcd.fillRect(0, 52, 128, 12, WHITE);
  lcd.setTextColor(BLACK);
  lcd.setCursor(2, 54);
  lcd.println(text);
}

void controlPage(int pos)
{

  // Serial.println("Control Page ");
  // Serial.println("");
  // Serial.println("Pos -> " + (String)pos);
  // Serial.println("Menu Page Before -> " + (String)menuPage);
  if (pos > menuPage * itemsPerPage)
  {
    menuPage += 1;
  }
  if (pos + itemsPerPage - 1 < menuPage * itemsPerPage)
  {
    if (menuPage > 1)
    {
      menuPage -= 1;
    }
  }
  // Serial.println("Menu Page After -> " + (String)menuPage);
  // Serial.println("");
}

String getMenuItem(int pos, String text)
{
  // return String(pos) + " " + text;
  return text;
}

void toggleLanguage()
{
  activeLang += 1;
  if (activeLang > 2)
  {
    activeLang = 0;
  }
}

void menu(int pos, int menuPage)
{
  int lineMenuToPrint = 1;

  for (size_t j = (menuPage * itemsPerPage) - itemsPerPage + 1; j < menuPage * itemsPerPage + 1; j++)
  {
    if (j == pos)
    {
      printLine(lineMenuToPrint, getMenuItem(j, menus[activeLang][j - 1]), true);
    }
    else
    {
      printLine(lineMenuToPrint, getMenuItem(j, menus[activeLang][j - 1]), false);
    }
    lineMenuToPrint += 1;
  }
  Serial.println("");
}

void fillGraph(int val, int valMin, int valMax)
{
  lcd.fillRect(22, 20, 84, 14, BLACK);
  if (val > valMax)
  {
    val = valMax;
  }

  lcd.fillRect(22, 20, map(val, valMin, valMax, valMin, 85), 14, WHITE);
  lcd.display();
}

void generateGraph(int valMin, int valMax, String unit, int currentValue)
{
  lcd.setTextSize(1);
  lcd.setTextColor(WHITE);
  lcd.setCursor(1, 24);
  lcd.print(valMin);
  lcd.setCursor(110, 24);
  lcd.print(valMax);
  lcd.drawRect(21, 19, 86, 16, WHITE);

  lcd.fillRect(52, 38, 40, 10, BLACK);
  lcd.setCursor(52, 38);
  if (currentValue > valMax)
  {
    lcd.print("Max");
  }
  else
  {
    lcd.print(currentValue + unit);
  }

  lcd.display();
  fillGraph(currentValue, valMin, valMax);
}

void actions(int menuPos)
{

  switch (menuPos)
  {
  case 1:
    printLine(0, menusTitle[activeLang][0], false);
    printLine(1, readingsPage[activeLang][0] + "     = " + (String)getTemperature(), false);
    printLine(2, readingsPage[activeLang][1] + "     = " + (String)getPH(false), false);
    printLine(3, readingsPage[activeLang][2] + "     = " + (String)getDistance(), false);
    printLine(4, readingsPage[activeLang][3] + "     = " + (String)getRelayState(), false);
    break;
  case 2:
    printLine(0, menusTitle[activeLang][1], false);
    generateGraph(0, 100, "C", (int)getTemperature());
    showTip(menuTipMessage[activeLang]);
    break;
  case 3:
    printLine(0, menusTitle[activeLang][2], false);
    if (relayState)
    {
      printLine(2, "" + menuStateMessage[1][activeLang] + " | > " + menuStateMessage[0][activeLang] + "", false);
      showTip(menuTipMessage[activeLang]);
    }
    else
    {
      printLine(2, "> " + menuStateMessage[1][activeLang] + " |  " + menuStateMessage[0][activeLang] + "", false);
      showTip(menuTipMessage[activeLang]);
    }
    break;
  case 4:
    printLine(0, menusTitle[activeLang][3], false);
    generateGraph(0, 250, "cm", (int)getDistance());
    showTip(menuTipMessage[activeLang]);
    break;
  case 5:
    printLine(0, menusTitle[activeLang][4], false);
    generateGraph(0, 14, "Ph", (int)getPH(false));
    showTip(menuTipMessage[activeLang]);
    break;
  case 6:
    printLine(0, menusTitle[activeLang][5], false);
    if (isWifiCon)
    {
      printLine(1, "IP -> " + WiFi.localIP().toString(), false);
      printLine(2, "Clear/Limpar/Limpiar", true);
      printLine(3, menuActionTipMessage[activeLang], false);
      showTip(menuTipMessage[activeLang]);
    }
    else
    {
      printLine(1, "CON -> ESP-MANAGER ", false);
      IPAddress IP = WiFi.softAPIP();
      printLine(2, "Acess", false);
      printLine(3, WiFi.softAPIP().toString(), false);
      showTip(menuTipMessage[activeLang]);
    }

    break;
  case 7:
    printLine(0, menusTitle[activeLang][6], false);
    printLine(1, menuActionTipMessage[activeLang], false);
    if (activeLang == 0)
    {
      printLine(2, "> EN | PT | SP", false);
    }
    else if (activeLang == 1)
    {
      printLine(2, "EN | > PT | SP", false);
    }
    else if (activeLang == 2)
    {
      printLine(2, "EN | PT | > SP", false);
    }
    else
    {
      showTip("Error");
    }
    showTip(menuTipMessage[activeLang]);
    break;
  case 8:
    printLine(0, menusTitle[activeLang][7], false);
    printLine(2, "Projeto de aquaponia", false);
    printLine(3, "Gabriel Bellagamba", false);
    showTip(menuTipMessage[activeLang]);
    break;

  default:
    lcd.clearDisplay();
    printLine(0, "Error !", false);
    break;
  }
}

void updateDisplay()
{
  if ((millis() - millisTarefa1) > 2000)
  {
    if (subMenuActive)
    {
      actions(menuPos);
      lcd.display();
    }
    else
    {
      menu(menuPos, menuPage);
    }
    getPH(false);
    getDistance();
    getTemperature();
    getRelayState();
    millisTarefa1 = millis();
  }
}

// ======================WIFI======================

bool initWiFi()
{
  if (ssid == "")
  {
    isWifiCon = false;
    return false;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  // while (WiFi.status() != WL_CONNECTED)
  while (WiFi.status() != WL_CONNECTED)
  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      isWifiCon = false;
      return false;
    }
  }

  isWifiCon = true;
  return true;
}

void printWifiStatus(bool wifiStatus)
{
  lcd.drawBitmap(115, 5, pix, 8, 8, BLACK);
  if (wifiStatus == false)
  {
    lcd.drawLine(113, 12, 123, 2, BLACK);
  }
  lcd.display();
}

void clearWifi()
{
  lcd.clearDisplay();
  printLine(0, wifiCleanMessage[activeLang], false);
  for (size_t i = 1; i < 4; i++)
  {
    generateGraph(1, 3, "S", i);
    lcd.display();
    delay(1000);
  }
  lcd.clearDisplay();
  printLine(2, "      " + resetMessage[activeLang], false);
  lcd.display();
  writeFile(SD, ssidPath, " ");
  writeFile(SD, passPath, " ");
  delay(1000);
  ESP.restart();
}

// ======================ROTARY AND BUTTON======================

void initRotary()
{
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  bool circleValues = false;
  rotaryEncoder.setBoundaries(1, 8, circleValues);
  rotaryEncoder.setAcceleration(10);
}

void initButton()
{
  button.attachClick(btnClick);
  button.attachDoubleClick(btnDoubleClick);
  button.attachLongPressStart(btnLongPress);
}

void btnClick()
{
  lcd.clearDisplay();
  actions(menuPos);
  subMenuActive = true;
  lcd.display();
}

void btnDoubleClick()
{
  if (menuPos == 3)
  {
    if (relayState == false)
    {
      toggleRelay("ON");
      actions(menuPos);
      lcd.display();
    }
    else
    {
      toggleRelay("OFF");
      actions(menuPos);
      lcd.display();
    }
  }
  else if (menuPos == 7)
  {
    toggleLanguage();
    actions(menuPos);
    lcd.display();
  }
  else if (menuPos == 6)
  {
    clearWifi();
  }
}

void btnLongPress()
{
  subMenuActive = false;
  printLine(0, "Menu", false);
  menu(menuPos, menuPage);
  printWifiStatus(isWifiCon);
  lcd.display();
}

void rotaryLoop()
{
  if (rotaryEncoder.encoderChanged())
  {
    if (!subMenuActive)
    {
      menuPos = rotaryEncoder.readEncoder();
      printLine(0, "Menu ", false);
      printWifiStatus(isWifiCon);
      controlPage(menuPos);
      menu(menuPos, menuPage);
    }
    else
    {
      actions(menuPos);
    }
    lcd.display();
  }
}

void IRAM_ATTR readEncoderISR()
{
  rotaryEncoder.readEncoder_ISR();
}

// ======================SENSORS======================

void initPorts()
{
  sensors.begin();
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(portsPURPLE[0], OUTPUT);
}

// TESTING
float getPH(bool debugPh)
{
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(portsGREEN[0]);
    // delay(1);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  // float volt = (float)avgval * 5.0/1024/6;
  float volt = (float)avgval * 3.3 / 4096.0 / 6;

  ph_act = -5.70 * volt + calibration_value;
  if (debugPh == true)
  {
    Serial.print("Voltage: ");
    Serial.print(volt);

    Serial.print("\tpH Val: ");
    Serial.print(ph_act);

    Serial.print("\tpH Val RD: ");
    Serial.print(round(ph_act));

    Serial.print("\tAnalog: \t");
    Serial.println(analogRead(A0));
  }
  // delay(200);

  return round(ph_act);
}

// WORKING
float getTemperature()
{
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  return tempC;
}

// WORKING
float getDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  return (float)distanceCm;
}

// WORKING
void toggleRelay(String state)
{
  if (state == "ON")
  {
    digitalWrite(portsPURPLE[0], HIGH);
    relayState = true;
  }
  else
  {
    digitalWrite(portsPURPLE[0], LOW);
    relayState = false;
  }
}

// ======================WEB SERVER======================

String getRelayState()
{
  if (relayState == false)
  {
    return "0";
  }
  else
  {
    return "1";
  }
}

String processorRelay(String state)
{
  if (state == "1")
  {
    toggleRelay("1");
  }
  else
  {
    toggleRelay("0");
  }
  return getRelayState();
}

String processorTemp()
{
  return (String)getTemperature();
}

String processorPh()
{
  return (String)getPH(false);
}

String processorDistance()
{
  return (String)getDistance();
}

void sendToServer()
{
  if (isWifiCon == true)
  {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName);

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "api_key=tPmAT5Ab3j7F9&board_id=1&ph_value= " + processorPh() + "&temp_value=" + processorTemp() + "&level_value=" + processorDistance() + "&relay_value=" + getRelayState();
    // Serial.print("httpRequestData: ");
    // Serial.println(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);
    if (httpResponseCode > 0)
    {
      // Serial.print("HTTP Response code: ");
      // Serial.println(httpResponseCode);
    }
    else
    {
      // Serial.print("Error code: ");
      // Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}

// ======================FILE READER======================

void initSDCard()
{
  if (!SD.begin())
  {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.println("MMC");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("SDHC");
  }
  else
  {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    return;
  }
  file.print(message);

  file.close();
}

String readFile(fs::FS &fs, const char *path, bool singleLine)
{
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return String();
  }

  String fileContent;
  while (file.available())
  {
    if (singleLine == true)
    {
      fileContent = file.readStringUntil('\n');
      break;
    }
    else
    {
      fileContent = file.read();
      break;
    }
  }
  file.close();
  // Serial.println("File Content -> "+fileContent);
  return fileContent;
}

void loadVariablesFromSDCard()
{
  ssid = readFile(SD, ssidPath, true);
  pass = readFile(SD, passPath, true);
}

// ======================SETUP FUNCTIONS======================

void setupWifiAndServer()
{
  if (initWiFi())
  {
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/index.html", String(), false); });
    // Route to load style.css file
    server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/index.css", "text/css"); });
    // Route to load style.css file
    server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/index.js", "text/javascript"); });
    // Route to set GPIO to HIGH
    server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    Serial.println("Getting TEMP");
    request->send(200, "text/plain", processorTemp()); });
    // Route to set GPIO to LOW
    server.on("/ph", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    Serial.println("Getting PH");
    request->send(200, "text/plain", processorPh()); });
    // Route to set GPIO to LOW
    server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    Serial.println("Getting Distance");
    request->send(200, "text/plain", processorDistance()); });
    // Route to set GPIO to LOW
    server.on("/relayOn", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    Serial.println("Getting Relay ON");
    request->send(200, "text/plain", processorRelay("ON")); });
    // Route to set GPIO to LOW
    server.on("/realyOff", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    Serial.println("Getting Relay OFF");
     request->send(200, "text/plain", processorRelay("OFF")); });
    // Start server
    server.begin();
  }
  else
  {
    // NULL sets an open Access Point
    WiFi.softAP("ESP-MANAGER", NULL);
    IPAddress IP = WiFi.softAPIP();
    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/wifi/wifimanager.html", "text/html"); });

    // server.serveStatic("/", SD, "/");

    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
              {
      int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) {
            ssid = p->value().c_str();
            writeFile(SD, ssidPath, ssid.c_str());
          }
          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) {
            pass = p->value().c_str();
            // Write file to save value
            writeFile(SD, passPath, pass.c_str());
          }
        }
      }
      request->send(200, "text/plain", "Done. ESP will restart, connect to your router");
      delay(3000);
      ESP.restart(); });
    server.begin();
  }
}

void setup()
{
  Serial.begin(115200);
  initLcd();
  lcd.clearDisplay();
  initRotary();
  initButton();
  initPorts();
  initSDCard();
  loadVariablesFromSDCard();
  setupWifiAndServer();
  xTaskCreatePinnedToCore(
      SendToSQLCode, /* Task function. */
      "SendToSQL",   /* name of task. */
      10000,         /* Stack size of task */
      NULL,          /* parameter of the task */
      1,             /* priority of the task */
      &SendToSQL,    /* Task handle to keep track of created task */
      0);            /* pin task to core 0 */
  delay(500);
}

void loop()
{
  button.tick();
  rotaryLoop();
  updateDisplay();
}