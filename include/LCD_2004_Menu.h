#ifndef LCD_2004_MENU
#define LCD_2004_MENU

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <vector>

#include "MenuItem.h"

class LCD_2004_Menu : public LiquidCrystal_I2C, public MenuItem {
public:
	LCD_2004_Menu(uint8_t lcd_Addr);

	virtual ~LCD_2004_Menu();

	bool addMenuItem(MenuItem* menuItem);

	void setCurrentMenuItem(MenuItem* menuItem);

	void draw();

	bool incrementMenuItemSelector();

	void upMenuItemSelector();

	void downMenuItemSelector();

	void invokeSelectedItem() const;

	bool isMenuActive() const;

	void setMenuActive(bool active);

protected:
private:
	std::vector<MenuItem*> m_menuItem;
	MenuItem* m_currentMenuItem;
	size_t m_counterMenuItemSelector;
	bool m_isActive;
};

#endif