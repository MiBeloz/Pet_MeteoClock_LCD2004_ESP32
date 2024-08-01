#include "LCD_2004_Menu.h"

LCD_2004_Menu::LCD_2004_Menu(uint8_t lcd_Addr)
: LiquidCrystal_I2C(lcd_Addr, 20, 4), m_counter(0), m_isActive(false), m_currentMenuItem(nullptr) {
  
}

LCD_2004_Menu::~LCD_2004_Menu() {
  for (size_t i = 0; i < m_menuItem.size(); ++i) {
    delete m_menuItem[i];
  }
}

bool LCD_2004_Menu::addMenuItem(MenuItem* menuItem) {
  if (m_menuItem.size() == m_menuItem.max_size()) {
    return false;
  }
  m_menuItem.push_back(menuItem);
  if (!m_currentMenuItem) {
    setCurrentMenuItem(m_menuItem[0], 0);
  }
  return true;
}

void LCD_2004_Menu::nextMenuItem() {
  if (!m_prevMenuItem.empty()) {
    setCurrentMenuItem(m_prevMenuItem.top().first, m_prevMenuItem.top().second);
    m_prevMenuItem.pop();
    clear();
    draw();
  }
  else {
    ++m_counter;
    if (m_counter < m_menuItem.size()) {
      setCurrentMenuItem(m_menuItem[m_counter], 0);
      clear();
      draw();
    }
    else {
      setActive(false);
    }
  }
}

void LCD_2004_Menu::upCounter() {
  m_currentMenuItem->decrementCounter();
  draw();
}

void LCD_2004_Menu::downCounter() {
  m_currentMenuItem->incrementCounter();
  draw();
}

void LCD_2004_Menu::invokeMenuItem() {
  if (m_currentMenuItem->getMenuItem(m_currentMenuItem->getCounter())->getMenuItemCount() == 0) {
    m_currentMenuItem->getMenuItem(m_currentMenuItem->getCounter())->invoke();
    setActive(false);
  }
  else {
    m_prevMenuItem.push(std::pair(m_currentMenuItem, m_currentMenuItem->getCounter()));
    setCurrentMenuItem(m_currentMenuItem->getMenuItem(m_currentMenuItem->getCounter()), 0);
    clear();
    draw();
  }
}

bool LCD_2004_Menu::isActive() const {
  return m_isActive;
}

void LCD_2004_Menu::setActive(bool active) {
  m_isActive = active;
  clear();
  if (m_isActive) {
    draw();
  }
  else {
    while (!m_prevMenuItem.empty()) {
      m_prevMenuItem.pop();
    }
    m_counter = 0;
    if (!m_menuItem.empty()) {
      setCurrentMenuItem(m_menuItem[0], 0);
    }
    else {
      m_currentMenuItem = nullptr;
    }
  }
}

void LCD_2004_Menu::draw() {
  if (m_currentMenuItem) {
    setCursor(0, 0);
    std::string menuName = m_currentMenuItem->getMenuName();
    while (menuName.length() < 20) {
      menuName += ' ';
    }
    print(menuName.c_str());
    size_t i = 0;
    if (m_currentMenuItem->getCounter() > 2) {
      i = m_currentMenuItem->getCounter() - 2;
    }
    for (int j = 0; i < m_currentMenuItem->getMenuItemCount(); ++i, ++j) {
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
      print(m_currentMenuItem->getMenuItem(i)->getMenuName().c_str());
    }
  }
}

void LCD_2004_Menu::setCurrentMenuItem(MenuItem* menuItem, size_t posCounter) {
  m_currentMenuItem = menuItem;
  m_currentMenuItem->setCounter(posCounter);
}
