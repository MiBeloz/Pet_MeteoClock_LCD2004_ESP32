#include "LCD_2004_Menu.h"

LCD_2004_Menu::LCD_2004_Menu(uint8_t lcd_Addr) : LiquidCrystal_I2C(lcd_Addr, 20, 4) {
  m_currentMenuItem = nullptr;
  m_counterMenuItemSelector = 0;
  m_isActive = false;
}

LCD_2004_Menu::~LCD_2004_Menu() {
  delete m_currentMenuItem;
  for (size_t i = 0; i < m_menuItem.size(); ++i) {
    delete m_menuItem[i];
  }
}

bool LCD_2004_Menu::addMenuItem(MenuItem* menuItem) {
  if (m_menuItem.size() == m_menuItem.max_size()) {
    return false;
  }
  m_menuItem.push_back(menuItem);
  return true;
}

void LCD_2004_Menu::setCurrentMenuItem(MenuItem* menuItem) {
  m_currentMenuItem = menuItem;
}

void LCD_2004_Menu::draw() {
  if (!m_currentMenuItem) {
    return;
  }

  setCursor(0, 0);
  print(m_currentMenuItem->getMenuName());
  int i = 0;
  if (m_currentMenuItem->getCounter() > 2) {
    i = m_currentMenuItem->getCounter() - 2;
  }
  for (int j = 0; i < m_currentMenuItem->getMenuItemSize(); ++i, ++j) {
    if (j > 2) {
      break;
    }
    setCursor(0, (1 + j));
    if (m_currentMenuItem->getCounter() == i) {
      print("-> ");
    }
    else {
      print("   ");
    }
    print(m_currentMenuItem->getMenuItems()[i]->getMenuName());
  }
}

bool LCD_2004_Menu::incrementMenuItemSelector() {
  ++m_counterMenuItemSelector;
  if (m_counterMenuItemSelector < m_menuItem.size()) {
    m_currentMenuItem = m_menuItem[m_counterMenuItemSelector];
    m_currentMenuItem->setMenuItemSelector(0);
    return true;
  }
  else {
    m_counterMenuItemSelector = 0;
    m_currentMenuItem = m_menuItem[m_counterMenuItemSelector];
    m_currentMenuItem->setMenuItemSelector(0);
    return false;
  }
}

void LCD_2004_Menu::upMenuItemSelector() {
  m_currentMenuItem->decrementMenuItemSelector();
}

void LCD_2004_Menu::downMenuItemSelector() {
  m_currentMenuItem->incrementMenuItemSelector();
}

void LCD_2004_Menu::invokeSelectedItem() const {
  m_currentMenuItem->getMenuItems()[m_counterMenuItemSelector]->invokeMenuItem();
}

bool LCD_2004_Menu::isMenuActive() const {
  return m_isActive;
}

void LCD_2004_Menu::setMenuActive(bool active) {
  if (!active && !m_menuItem.empty()) {
      m_currentMenuItem = m_menuItem[0];
  }
  m_isActive = active;
}
