#ifndef LCD_2004_MENU
#define LCD_2004_MENU

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstddef>
#endif
#include <LiquidCrystal_I2C.h>
#include <vector>
#include <stack>

#include "MenuItem.h"

#define EMPTY_LINE "                   "

enum class MessageTime {
	NO_PAUSE,
	DEFAULT_PAUSE
};

class LCD_2004_Menu : public LiquidCrystal_I2C {
public:
	LCD_2004_Menu(uint8_t lcd_Addr);

	virtual ~LCD_2004_Menu();

	void init();

	void setMenu(MenuItem* menuItem);

	void returnToPreviousMenu();

	void upCounter();

	void downCounter();

	void invokeMenuItem();

	bool isActive() const;

	void setActive(bool active);

	void clearAndPrintMessage(MessageTime mt,
					const char* str1,
					const char* str2 = EMPTY_LINE,
					const char* str3 = EMPTY_LINE,
					const char* str4 = EMPTY_LINE);

	void printMessage(uint8_t col, uint8_t row, const char* str);

	void printDate(uint8_t col, uint8_t row, uint8_t day, uint8_t month, uint16_t year);

	void printTime(uint8_t col, uint8_t row, uint8_t hour, uint8_t minute, uint8_t second);

	void printDayOfTheWeek(uint8_t col, uint8_t row, uint8_t day);

	void printSensorValue(uint8_t col, uint8_t row, float value, const char* measure = nullptr, const char* measure_2 = nullptr);

private:
	size_t m_counter;
	bool m_isActive;
	MenuItem* m_menuItem;
	MenuItem* m_currentMenuItem;
	std::stack<std::pair<MenuItem*, size_t>> m_prevMenuItem;
	uint16_t m_showMessageTime;
	inline static const char* m_dayOfTheWeek[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	inline static uint8_t m_wifiStatus[6][8] {
		{ B10001, B10001, B01010, B00100, B01010, B10001, B10001 },
		{ B00000, B00000, B00000, B00000, B00000, B10000, B10000 },
		{ B00000, B00000, B00000, B00000, B01000, B11000, B11000 },
		{ B00000, B00000, B00000, B00100, B01100, B11100, B11100 },
		{ B00000, B00000, B00010, B00110, B01110, B11110, B11110 },
		{ B00000, B00001, B00011, B00111, B01111, B11111, B11111 }
	};

	void draw();

	void setCurrentMenuItem(MenuItem* menuItem, size_t posCounter);

	const char* alignmentStr(const char* str);
};

#endif