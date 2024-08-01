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

class LCD_2004_Menu : public LiquidCrystal_I2C {
public:
	LCD_2004_Menu(uint8_t lcd_Addr);

	virtual ~LCD_2004_Menu();

	bool addMenuItem(MenuItem* menuItem);

	void nextMenuItem();

	void upCounter();

	void downCounter();

	void invokeMenuItem();

	bool isActive() const;

	void setActive(bool active);

protected:
	void draw();

	void setCurrentMenuItem(MenuItem* menuItem, size_t posCounter);

private:
	size_t m_counter;
	bool m_isActive;
	MenuItem* m_currentMenuItem;
	std::vector<MenuItem*> m_menuItem;
	std::stack<std::pair<MenuItem*, size_t>> m_prevMenuItem;

	// inline static std::vector<unsigned char*> m_wifiStatus = {
	// 	{ B10001, B10001, B01010, B00100, B01010, B10001, B10001 },
	// 	{ B00000, B00000, B00000, B00000, B00000, B10000, B10000 },
	// 	{ B00000, B00000, B00000, B00000, B01000, B11000, B11000 },
	// 	{ B00000, B00000, B00000, B00100, B01100, B11100, B11100 },
	// 	{ B00000, B00000, B00010, B00110, B01110, B11110, B11110 },
	// 	{ B00000, B00001, B00011, B00111, B01111, B11111, B11111 }
	// };
};


// class fff {
// 	template<typename T>
// 	fff(T) = delete;
// };

#endif