#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <RTClib.h>

#include "LCD_2004_Menu.h"
#include "MeteoSensors.h"
#include "RemoteController.hpp"

#define LCD_ADDRESS   0x27
#define IRRECEIVEPIN  0

enum StatisticsAll {
  statisticsOff,
  statisticsTemperature,
  statisticsHumidity,
  statisticsPressure
};

uint16_t showMessageTime = 3000;

const char* dayOfTheWeekStr[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

std::string ssid = "iPhone (Михаил)";
std::string password = "q6d1nzngyzfuz";
std::string endpoint = "http://worldtimeapi.org/api/timezone/Asia/Omsk";

uint8_t timeoutWiFi = 10;

StatisticsAll statisticsAll = StatisticsAll::statisticsOff;

LCD_2004_Menu lcd(LCD_ADDRESS);
MeteoSensors meteoSensors;
RTC_DS3231 rtc;

// int addressDefaultAllValues = 0;
// int addressDateTemperatureMinimum = 10;
// int addressDateTemperatureMaximum = 14;
// int addressDateHumidityMinimum = 18;
// int addressDateHumidityMaximum = 22;
// int addressDatePressureMinimum = 26;
// int addressDatePressureMaximum = 30;
// int addressTemperatureMinimum = 34;
// int addressTemperatureMaximum = 38;
// int addressHumidityMinimum = 42;
// int addressHumidityMaximum = 46;
// int addressPressureMinimum = 50;
// int addressPressureMaximum = 54;

uint8_t defaultAllValues = 0;
uint32_t dateTemperatureMinimum = 0;
uint32_t dateTemperatureMaximum = 0;
uint32_t dateHumidityMinimum = 0;
uint32_t dateHumidityMaximum = 0;
uint32_t datePressureMinimum = 0;
uint32_t datePressureMaximum = 0;
float temperatureMinimum = 100.0f;
float temperatureMaximum = -100.0f;
float humidityMinimum = 100.0f;
float humidityMaximum = -100.0;
float pressureMinimum = 2000.0f;
float pressureMaximum = -2000.0f;

void connectionToWiFi();
void disconnectWiFi();
void handleReceivedMessage(String message);
void printLCDMessage(const char* str1, const char* str2 = nullptr, const char* str3 = nullptr, const char* str4 = nullptr);
void printFullDate(uint8_t col, uint8_t row, uint8_t day, uint8_t month, uint16_t year);
void printFullTime(uint8_t col, uint8_t row, uint8_t hour, uint8_t minute, uint8_t second);
void printDayOfTheWeek(uint8_t col, uint8_t row, uint8_t day);
void printSensorValue(uint8_t col, uint8_t row, float value, const char* measure = nullptr, const char* measure_2 = nullptr);
void updateEEPROM();
void home();
void setTime();
void setDate();
template<typename T>
T setDateTimeValue(T currentVal, T minVal, T maxVal, uint8_t position);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  meteoSensors.begin();

  rtc.begin();
  if (rtc.now() < DateTime(2024, 1, 1)) {
    rtc.adjust(DateTime(2024, 1, 1));
  }

  IrReceiver.begin(IRRECEIVEPIN);

  MenuItem* clockMenu = new MenuItem("Clock Menu");
  MenuItem* setTimeSubmenu = new MenuItem("Set Time");
  MenuItem* setDateSubmenu = new MenuItem("Set Date");
  clockMenu->addMenuItem(setTimeSubmenu);
  clockMenu->addMenuItem(setDateSubmenu);
  setTimeSubmenu->setFunction(setTime);
  setDateSubmenu->setFunction(setDate);
  
  MenuItem* alarmMenu = new MenuItem("Alarm Menu");
  MenuItem* alarmsSubmenu = new MenuItem("Alarms");
  MenuItem* addAlarmSubmenu = new MenuItem("Add Alarm");
  alarmMenu->addMenuItem(alarmsSubmenu);
  alarmMenu->addMenuItem(addAlarmSubmenu);

  MenuItem* displayMenu = new MenuItem("Display Menu");
  MenuItem* onOffBacklightSubmenu = new MenuItem("On/Off Backlight");
  displayMenu->addMenuItem(onOffBacklightSubmenu);

  MenuItem* wifiMenu = new MenuItem("WiFi Menu");
  MenuItem* connectWifiSubmenu = new MenuItem("Connect");
  MenuItem* disconnectWifiSubmenu = new MenuItem("Disconnect");
  MenuItem* aboutWifiSubmenu = new MenuItem("About Network");
  MenuItem* networksWifiSubmenu = new MenuItem("Networks");
  MenuItem* settingsWifiSubmenu = new MenuItem("WiFi Settings");
  MenuItem* timeoutWifiSubmenu = new MenuItem("Timeout");
  MenuItem* autoconnectStartupWifiSubmenu = new MenuItem("Autoconnect");
  MenuItem* timeUpdateRateWifiSubmenu = new MenuItem("Time update rate");
  MenuItem* setTimeWifiSubmenu = new MenuItem("Set time");
  MenuItem* setRateWifiSubmenu = new MenuItem("Set rate");
  MenuItem* rateEveryHourWifiSubmenu = new MenuItem("Every hour");
  MenuItem* rateEveryDayWifiSubmenu = new MenuItem("Every day");
  MenuItem* rateEveryWeekWifiSubmenu = new MenuItem("Every week");
  MenuItem* rateEveryMonthWifiSubmenu = new MenuItem("Every month");
  wifiMenu->addMenuItem(connectWifiSubmenu);
  wifiMenu->addMenuItem(disconnectWifiSubmenu);
  wifiMenu->addMenuItem(aboutWifiSubmenu);
  wifiMenu->addMenuItem(networksWifiSubmenu);
  wifiMenu->addMenuItem(settingsWifiSubmenu);
  settingsWifiSubmenu->addMenuItem(timeoutWifiSubmenu);
  settingsWifiSubmenu->addMenuItem(autoconnectStartupWifiSubmenu);
  settingsWifiSubmenu->addMenuItem(timeUpdateRateWifiSubmenu);
  timeUpdateRateWifiSubmenu->addMenuItem(setTimeWifiSubmenu);
  timeUpdateRateWifiSubmenu->addMenuItem(setRateWifiSubmenu);
  setRateWifiSubmenu->addMenuItem(rateEveryHourWifiSubmenu);
  setRateWifiSubmenu->addMenuItem(rateEveryDayWifiSubmenu);
  setRateWifiSubmenu->addMenuItem(rateEveryWeekWifiSubmenu);
  setRateWifiSubmenu->addMenuItem(rateEveryMonthWifiSubmenu);

  lcd.addMenuItem(clockMenu);
  lcd.addMenuItem(alarmMenu);
  lcd.addMenuItem(displayMenu);
  lcd.addMenuItem(wifiMenu);

  connectionToWiFi();
}

void loop() {
  static uint64_t readSensorTimer = 0;
  if (millis() > readSensorTimer + 2000) {
    readSensorTimer = millis();
    meteoSensors.read();
  }

  IRRawDataType button_click = getIrReceiverCommand();
  if (lcd.isActive()) {
    if (button_click == button_asterisk) {
      lcd.nextMenuItem();
    }
    else if (button_click == button_up) {
      lcd.upCounter();
    }
    else if (button_click == button_down) {
      lcd.downCounter();
    }
    else if (button_click == button_ok) {
      lcd.invokeMenuItem();
    }
    else if (button_click == button_hash) {
      lcd.setActive(false);
    }
  }
  else {
    home();
    if (button_click == button_asterisk) {
      lcd.setActive(true);
    }
    else if (button_click == button_ok) {
      lcd.clear();
      if (statisticsAll == StatisticsAll::statisticsOff) {
        statisticsAll = StatisticsAll::statisticsTemperature;
      }
      else if (statisticsAll == StatisticsAll::statisticsTemperature) {
        statisticsAll = StatisticsAll::statisticsHumidity;
      }
      else if (statisticsAll == StatisticsAll::statisticsHumidity) {
        statisticsAll = StatisticsAll::statisticsPressure;
      }
      else if (statisticsAll == StatisticsAll::statisticsPressure) {
        statisticsAll = StatisticsAll::statisticsOff;
      }
      else {
        statisticsAll = StatisticsAll::statisticsOff;
      }
    }
  }
}

void connectionToWiFi() {
  lcd.clear();
  lcd.print("Connection to");
  lcd.setCursor(0, 1);
  lcd.print("WiFi...");
  delay(1000);
  WiFi.mode(wifi_mode_t::WIFI_MODE_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  bool connected = false;
  for (int i = 0; i < timeoutWiFi; ++i) {
    if (WiFi.status() != WL_CONNECTED) {
      delay(1000);
    }
    else {
      connected = true;
      printLCDMessage("WiFi connection", "successful!");
      lcd.clear();
      break;
    }
  }

  if (connected) {
    HTTPClient http;
    http.begin(String(endpoint.c_str()));
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      handleReceivedMessage(payload);
    }
    else {
      printLCDMessage("Error HTTP!", "Date and Time will", "be set by default.");
      disconnectWiFi();
      lcd.clear();
    }
    http.end();
  }
  else {
    printLCDMessage("Error connection!", "Date and Time will", "be set by default.");
    disconnectWiFi();
    lcd.clear();
  }
}

void disconnectWiFi() {
  WiFi.disconnect();
}

void handleReceivedMessage(String message) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    printLCDMessage("Deserialize error!", "Date and Time will", "be set by default.");
    lcd.clear();
    return;
  }
  uint32_t unixtime = doc["unixtime"];
  int raw_offset = doc["raw_offset"];
  unixtime += raw_offset;
  rtc.adjust(DateTime(unixtime));
  printLCDMessage("The date and time", "were successfully", "received!");
  lcd.clear();
}

void printLCDMessage(const char* str1, const char* str2, const char* str3, const char* str4) {
  lcd.clear();
  lcd.print(str1);
  if (str2) {
    lcd.setCursor(0, 1);
    lcd.print(str2);
    if (str3) {
      lcd.setCursor(0, 2);
      lcd.print(str3);
      if (str4) {
        lcd.setCursor(0, 3);
        lcd.print(str4);
      }
    }
  }
  delay(showMessageTime);
}

void printFullDate(uint8_t col, uint8_t row, uint8_t day, uint8_t month, uint16_t year) {
  lcd.setCursor(col, row);
  if (day < 10) { lcd.print("0"); }
  lcd.print(day);
  lcd.print(".");
  if (month < 10) { lcd.print("0"); }
  lcd.print(month);
  lcd.print(".");
  lcd.print(year);
}

void printFullTime(uint8_t col, uint8_t row, uint8_t hour, uint8_t minute, uint8_t second) {
  lcd.setCursor(col, row);
  if (hour < 10) { lcd.print("0"); }
  lcd.print(hour);
  lcd.print(":");
  if (minute < 10) { lcd.print("0"); }
  lcd.print(minute);
  lcd.print(":");
  if (second < 10) { lcd.print("0"); }
  lcd.print(second);
}

void printDayOfTheWeek(uint8_t col, uint8_t row, uint8_t day) {
  lcd.setCursor(col, row);
  lcd.print(dayOfTheWeekStr[day]);
}

void printSensorValue(uint8_t col, uint8_t row, float value, const char* measure, const char* measure_2) {
  lcd.setCursor(col, row);
  lcd.print(value, 1);
  if (measure) { lcd.print(measure); }
  if (measure_2) { lcd.print(measure_2); }
}

void updateEEPROM() {
  if (temperatureMinimum > meteoSensors.getTemperature()) {
    DateTime now_temperature_minimum(rtc.now());
    temperatureMinimum = meteoSensors.getTemperature();
    dateTemperatureMinimum = now_temperature_minimum.unixtime();
    // EEPROM.put(addressDateTemperatureMinimum, dateTemperatureMinimum);
    // EEPROM.put(addressTemperatureMinimum, temperatureMinimum);
    if (!defaultAllValues) {
      // EEPROM.put(addressDefaultAllValues, true);
    }
  }
  if (temperatureMaximum < meteoSensors.getTemperature()) {
    DateTime now_temperature_maximum(rtc.now());
    temperatureMaximum = meteoSensors.getTemperature();
    dateTemperatureMaximum = now_temperature_maximum.unixtime();
    // EEPROM.put(addressDateTemperatureMaximum, dateTemperatureMaximum);
    // EEPROM.put(addressTemperatureMaximum, temperatureMaximum);
    if (!defaultAllValues) {
      // EEPROM.put(addressDefaultAllValues, true);
    }
  }
  if (humidityMinimum > meteoSensors.getHumidity()) {
    DateTime now_humidity_minimum(rtc.now());
    humidityMinimum = meteoSensors.getHumidity();
    dateHumidityMinimum = now_humidity_minimum.unixtime();
    // EEPROM.put(addressDateHumidityMinimum, dateHumidityMinimum);
    // EEPROM.put(addressHumidityMinimum, humidityMinimum);
    if (!defaultAllValues) {
      // EEPROM.put(addressDefaultAllValues, true);
    }
  }
  if (humidityMaximum < meteoSensors.getHumidity()) {
    DateTime now_humidity_maximum(rtc.now());
    humidityMaximum = meteoSensors.getHumidity();
    dateHumidityMaximum = now_humidity_maximum.unixtime();
    // EEPROM.put(addressDateHumidityMaximum, dateHumidityMaximum);
    // EEPROM.put(addressHumidityMaximum, humidityMaximum);
    if (!defaultAllValues) {
      // EEPROM.put(addressDefaultAllValues, true);
    }
  }
  if (pressureMinimum > meteoSensors.getPressure()) {
    DateTime now_pressure_minimum(rtc.now());
    pressureMinimum = meteoSensors.getPressure();
    datePressureMinimum = now_pressure_minimum.unixtime();
    // EEPROM.put(addressDatePressureMinimum, datePressureMinimum);
    // EEPROM.put(addressPressureMinimum, pressureMinimum);
    if (!defaultAllValues) {
      // EEPROM.put(addressDefaultAllValues, true);
    }
  }
  if (pressureMaximum < meteoSensors.getPressure()) {
    DateTime now_pressure_maximum(rtc.now());
    pressureMaximum = meteoSensors.getPressure();
    datePressureMaximum = now_pressure_maximum.unixtime();
    // EEPROM.put(addressDatePressureMaximum, datePressureMaximum);
    // EEPROM.put(addressPressureMaximum, pressureMaximum);
    if (!defaultAllValues) {
      // EEPROM.put(addressDefaultAllValues, true);
    }
  }
}

void home() {
  lcd.setCursor(0, 0);
  if (statisticsAll == StatisticsAll::statisticsTemperature) {
    lcd.print("TMin:");
    lcd.setCursor(0, 1);
    lcd.print(temperatureMinimum, 1);
    DateTime dt_temperature_minimum(dateTemperatureMinimum);
    printFullTime(0, 2, dt_temperature_minimum.hour(), dt_temperature_minimum.minute(), dt_temperature_minimum.second());
    printFullDate(0, 3, dt_temperature_minimum.day(), dt_temperature_minimum.month(), dt_temperature_minimum.year() % 100);
    lcd.setCursor(10, 0);
    lcd.print("TMax:");
    lcd.setCursor(10, 1);
    lcd.print(temperatureMaximum, 1);
    DateTime dt_temperature_maximum(dateTemperatureMaximum);
    printFullTime(10, 2, dt_temperature_maximum.hour(), dt_temperature_maximum.minute(), dt_temperature_maximum.second());
    printFullDate(10, 3, dt_temperature_maximum.day(), dt_temperature_maximum.month(), dt_temperature_maximum.year() % 100);
  }
  else if (statisticsAll == StatisticsAll::statisticsHumidity) {
    lcd.print("HMin:");
    lcd.setCursor(0, 1);
    lcd.print(humidityMinimum, 1);
    DateTime dt_humidity_minimum(dateHumidityMinimum);
    printFullTime(0, 2, dt_humidity_minimum.hour(), dt_humidity_minimum.minute(), dt_humidity_minimum.second());
    printFullDate(0, 3, dt_humidity_minimum.day(), dt_humidity_minimum.month(), dt_humidity_minimum.year() % 100);
    lcd.setCursor(10, 0);
    lcd.print("HMax:");
    lcd.setCursor(10, 1);
    lcd.print(humidityMaximum, 1);
    DateTime dt_humidity_maximum(dateHumidityMaximum);
    printFullTime(10, 2, dt_humidity_maximum.hour(), dt_humidity_maximum.minute(), dt_humidity_maximum.second());
    printFullDate(10, 3, dt_humidity_maximum.day(), dt_humidity_maximum.month(), dt_humidity_maximum.year() % 100);
  }
  else if (statisticsAll == StatisticsAll::statisticsPressure) {
    lcd.print("PMin:");
    lcd.setCursor(0, 1);
    lcd.print(pressureMinimum, 1);
    DateTime dt_pressure_minimum(datePressureMinimum);
    printFullTime(0, 2, dt_pressure_minimum.hour(), dt_pressure_minimum.minute(), dt_pressure_minimum.second());
    printFullDate(0, 3, dt_pressure_minimum.day(), dt_pressure_minimum.month(), dt_pressure_minimum.year() % 100);
    lcd.setCursor(10, 0);
    lcd.print("PMax:");
    lcd.setCursor(10, 1);
    lcd.print(pressureMaximum, 1);
    DateTime dt_pressure_maximum(datePressureMaximum);
    printFullTime(10, 2, dt_pressure_maximum.hour(), dt_pressure_maximum.minute(), dt_pressure_maximum.second());
    printFullDate(10, 3, dt_pressure_maximum.day(), dt_pressure_maximum.month(), dt_pressure_maximum.year() % 100);
  }
  else {
    DateTime now = rtc.now();
    printFullTime(0, 0, now.hour(), now.minute(), now.second());
    printDayOfTheWeek(10, 0, now.dayOfTheWeek());
    printFullDate(0, 1, now.day(), now.month(), now.year());
    printSensorValue(0, 3, meteoSensors.getTemperature(), "\xDF", "C ");
    printSensorValue(10, 2, meteoSensors.getHumidity(), "% ");
    printSensorValue(10, 3, meteoSensors.getPressure(), "mmHg ");

    static uint64_t statusWiFiTimer = 0;
    int rssi = WiFi.RSSI();
    

    //Serial.printf("RSSI: %d\n", rssi);
    lcd.setCursor(19, 0);
    if ((rssi <= 0 && rssi > -25) || rssi < -90) {
      lcd.print('X');
    }
    else if (rssi <= -25 && rssi >= -50) {
      lcd.print(static_cast<char>(5));
    }
    else if (rssi < -50 && rssi >= -60) {
      lcd.print(static_cast<char>(4));
    }
    else if (rssi < -60 && rssi >= -70) {
      lcd.print(static_cast<char>(3));
    }
    else if (rssi < -70 && rssi >= -80) {
      lcd.print(static_cast<char>(2));
    }
    else if (rssi < -80 && rssi >= -90) {
      lcd.print(static_cast<char>(1));
    }
    else {
      lcd.print(static_cast<char>(0));
    }
  }
}

void setTime() {
  const DateTime dt = rtc.now();
  uint8_t hour = dt.hour();
  uint8_t minute = dt.minute();
  uint8_t second = dt.second();

  lcd.clear();
  lcd.setCursor(0, 2);

  if (hour < 10) {
    lcd.print(0);
  }
  lcd.print(hour);
  lcd.print(":");
  if (minute < 10) {
    lcd.print(0);
  }
  lcd.print(minute);
  lcd.print(":");
  if (second < 10) {
    lcd.print(0);
  }
  lcd.print(second);

  lcd.setCursor(0, 0);
  lcd.print("Enter hours:        ");
  hour = setDateTimeValue<uint8_t>(hour, 0, 23, 0);

  lcd.setCursor(0, 0);
  lcd.print("Enter minutes:      ");
  minute = setDateTimeValue<uint8_t>(minute, 0, 59, 3);

  lcd.setCursor(0, 0);
  lcd.print("Enter seconds:      ");
  second = setDateTimeValue<uint8_t>(second, 0, 59, 6);

  rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), hour, minute, second));

  printLCDMessage("Date has been set!");
}

void setDate() {
  bool dateIsCorrect = false;
  DateTime dt = rtc.now();
  uint8_t day = 0;
  uint8_t month = 0;
  uint16_t year = 0;

  do {
    lcd.clear();
    lcd.setCursor(0, 2);

    if (dt.day() < 10) {
      lcd.print(0);
    }
    lcd.print(dt.day());
    lcd.print(".");
    if (dt.month() < 10) {
      lcd.print(0);
    }
    lcd.print(dt.month());
    lcd.print(".");
    lcd.print(dt.year());

    lcd.setCursor(0, 0);
    lcd.print("Enter day:         ");
    day = dt.day();
    day = setDateTimeValue<uint8_t>(day, 1, 31, 0);

    lcd.setCursor(0, 0);
    lcd.print("Enter month:       ");
    month = dt.month();
    month = setDateTimeValue<uint8_t>(month, 1, 12, 3);

    lcd.setCursor(0, 0);
    lcd.print("Enter year:        ");
    year = dt.year();
    year = setDateTimeValue<uint16_t>(year, 2000, 2099, 6);

    DateTime current(year, month, day, rtc.now().hour(), rtc.now().minute(), rtc.now().second());
    if (current.isValid()) {
      dateIsCorrect = true;
    }
    else {
      printLCDMessage("The date is not", "compatible with the", "month or the", "year!");
      lcd.clear();
      lcd.print("Press 'OK' to");
      lcd.setCursor(0, 1);
      lcd.print("autocorrect or");
      lcd.setCursor(0, 2);
      lcd.print("'#' to enter a");
      lcd.setCursor(0, 3);
      lcd.print("different date.");
      while (true) {
        if (IrReceiver.decode()) {
          if (IrReceiver.decodedIRData.protocol != UNKNOWN) {
            if (IrReceiver.decodedIRData.command != 0) {
              if (IrReceiver.decodedIRData.decodedRawData == button_ok) {
                while (!current.isValid()) {
                  day--;
                  current = DateTime(year, month, day, rtc.now().hour(), rtc.now().minute(), rtc.now().second());
                }
                dateIsCorrect = true;
                printLCDMessage("Autocorrect!");
                break;
              }
              else if (IrReceiver.decodedIRData.decodedRawData == button_hash) {
                break;
              }
            }
            IrReceiver.resume();
          }
        }
      }
    }
  }
  while (!dateIsCorrect);

  dt = rtc.now();
  rtc.adjust(DateTime(year, month, day, dt.hour(), dt.minute(), dt.second()));

  printLCDMessage("Date has been set!");
}

template<typename T>
T setDateTimeValue(T currentVal, T minVal, T maxVal, uint8_t position) {
  lcd.setCursor(position, 2);
  if (currentVal < 10) {
    lcd.print(0);
  }
  lcd.print(currentVal);

  while (true) {
    if (IrReceiver.decode()) {
      if (IrReceiver.decodedIRData.protocol != UNKNOWN) {
        if (IrReceiver.decodedIRData.command != 0) {
          if (IrReceiver.decodedIRData.decodedRawData == button_ok) {
            IrReceiver.resume();
            return currentVal;
          }
          else if (IrReceiver.decodedIRData.decodedRawData == button_up) {
            if (currentVal >= maxVal) {
              currentVal = minVal;
            }
            else {
              currentVal++;
            }
          }
          else if (IrReceiver.decodedIRData.decodedRawData == button_down) {
            if (currentVal <= minVal) {
              currentVal = maxVal;
            }
            else {
              currentVal--;
            }
          }
          IrReceiver.decodedIRData.command = 0;
          lcd.setCursor(position, 2);
          if (currentVal < 10) {
            lcd.print(0);
          }
          lcd.print(currentVal);
        }
        IrReceiver.resume();
      }
    }
  }
}
