#ifndef LCD_2004_MENU
#define LCD_2004_MENU

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "MenuItem.h"

class LCD_2004_Menu : public LiquidCrystal_I2C {
public:
	LCD_2004_Menu(uint8_t lcd_Addr);
	virtual ~LCD_2004_Menu();

	bool addMenuItem(MenuItem* menuItem);
	MenuItem& getMenuItem(uint8_t pos);
	void draw();
	bool incrementMenuItemSelector();
	void upMenuItemSelector();
	void downMenuItemSelector();
	void invokeSelectedItem() const;
	bool isMenuActive() const;
	void setMenuActive(bool active);

protected:
private:
	MenuItem* m_menuItem[255];
	uint8_t m_size;
	uint8_t m_counterMenuItemSelector;
	bool m_isActive;
};

#endif