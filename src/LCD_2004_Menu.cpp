#include "LCD_2004_Menu.h"

LCD_2004_Menu::LCD_2004_Menu(uint8_t lcd_Addr) : LiquidCrystal_I2C(lcd_Addr, 20, 4) {
  for (uint8_t i = 0; i < 255; ++i) {
    m_menuItem[i] = nullptr;
  }
  m_size = 0;
  m_counterMenuItemSelector = 0;
  m_isActive = false;
}

LCD_2004_Menu::~LCD_2004_Menu() {
  for (uint8_t i = 0; i < 255; ++i) {
    delete m_menuItem[i];
  }
}

bool LCD_2004_Menu::addMenuItem(MenuItem* menuItem) {
  if (m_size == 255) {
    return false;
  }
  m_menuItem[m_size] = menuItem;
  ++m_size;
  return true;
}

MenuItem& LCD_2004_Menu::getMenuItem(uint8_t pos) {
  return *m_menuItem[pos];
}

void LCD_2004_Menu::draw() {
  setCursor(0, 0);
  print(m_menuItem[m_counterMenuItemSelector]->getName());
  int i = 0;
  if (m_menuItem[m_counterMenuItemSelector]->getCounter() > 2) {
    i = m_menuItem[m_counterMenuItemSelector]->getCounter() - 2;
  }
  for (int j = 0; i < m_menuItem[m_counterMenuItemSelector]->getMenuItemsSize(); ++i, ++j) {
    if (j > 2) {
      break;
    }
    setCursor(0, (1 + j));
    if (m_menuItem[m_counterMenuItemSelector]->getCounter() == i) {
      print("-> ");
    }
    else {
      print("   ");
    }
    print(m_menuItem[m_counterMenuItemSelector]->getItems()[i]->getName());
  }
}

bool LCD_2004_Menu::incrementMenuItemSelector() {
  ++m_counterMenuItemSelector;
  if (m_counterMenuItemSelector < m_size) {
    m_menuItem[m_counterMenuItemSelector]->setCounterItemSelector(0);
    return true;
  }
  else {
    m_counterMenuItemSelector = 0;
    m_menuItem[m_counterMenuItemSelector]->setCounterItemSelector(0);
    return false;
  }
}

void LCD_2004_Menu::upMenuItemSelector() {
  m_menuItem[m_counterMenuItemSelector]->decrementCounterItemSelector();
}

void LCD_2004_Menu::downMenuItemSelector() {
  m_menuItem[m_counterMenuItemSelector]->incrementCounterItemSelector();
}

void LCD_2004_Menu::invokeSelectedItem() const {
  return m_menuItem[m_counterMenuItemSelector]->invokeMenuItem();
}

bool LCD_2004_Menu::isMenuActive() const {
  return m_isActive;
}

void LCD_2004_Menu::setMenuActive(bool active) {
  m_isActive = active;
}
