#include "LCD_2004_Menu.h"

LCD_2004_Menu::LCD_2004_Menu(uint8_t lcd_Addr)
: LiquidCrystal_I2C(lcd_Addr, 20, 4), m_counter(0), m_isActive(false), m_menuItem(nullptr), m_currentMenuItem(nullptr), m_showMessageTime(3000) {}

LCD_2004_Menu::~LCD_2004_Menu() {
  delete m_menuItem;
}

void LCD_2004_Menu::init() {
  LiquidCrystal_I2C::init();
  for (uint8_t i = 0; i < sizeof(m_wifiStatus) / sizeof(m_wifiStatus[0]); ++i) {
    createChar(i, m_wifiStatus[i]);
  }
}

void LCD_2004_Menu::setMenu(MenuItem* menuItem) {
  if (m_menuItem) {
    delete m_menuItem;
  }
  m_menuItem = menuItem;
}

void LCD_2004_Menu::returnToPreviousMenu() {
  if (!m_prevMenuItem.empty()) {
    setCurrentMenuItem(m_prevMenuItem.top().first, m_prevMenuItem.top().second);
    m_prevMenuItem.pop();
    clear();
    draw();
  }
  else {
    setActive(false);
  }
}

void LCD_2004_Menu::upCounter() {
  if (m_currentMenuItem) {
    m_currentMenuItem->decrementCounter();
    draw();
  }
}

void LCD_2004_Menu::downCounter() {
  if (m_currentMenuItem) {
    m_currentMenuItem->incrementCounter();
    draw();
  }
}

void LCD_2004_Menu::invokeMenuItem() {
  if (m_currentMenuItem) {
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
}

bool LCD_2004_Menu::isActive() const {
  return m_isActive;
}

void LCD_2004_Menu::setActive(bool active) {
  m_isActive = active;
  clear();
  if (m_isActive) {
    if (m_menuItem) {
      setCurrentMenuItem(m_menuItem, 0);
    }
    draw();
  }
  else {
    while (!m_prevMenuItem.empty()) {
      m_prevMenuItem.pop();
    }
    m_counter = 0;
    m_currentMenuItem = nullptr;
  }
}

void LCD_2004_Menu::clearAndPrintMessage(MessageTime mt, const char* str1, const char* str2, const char* str3, const char* str4) {
  //if (!m_isActive) {
    setCursor(0, 0);
    print(alignmentStr(str1));
    setCursor(0, 1);
    print(alignmentStr(str2));
    setCursor(0, 2);
    print(alignmentStr(str3));
    setCursor(0, 3);
    print(alignmentStr(str4));
    if (mt == MessageTime::DEFAULT_PAUSE) {
      delay(m_showMessageTime);
    }
  //}
}

void LCD_2004_Menu::printMessage(uint8_t col, uint8_t row, const char* str) {
  //if (!m_isActive) {
    setCursor(col, row);
    print(alignmentStr(str));
  //}
}

void LCD_2004_Menu::printDate(uint8_t col, uint8_t row, uint8_t day, uint8_t month, uint16_t year) {
  if (!m_isActive) {
    setCursor(col, row);
    if (day < 10) { print("0"); }
    print(day);
    print(".");
    if (month < 10) { print("0"); }
    print(month);
    print(".");
    print(year);
  }
}

void LCD_2004_Menu::printTime(uint8_t col, uint8_t row, uint8_t hour, uint8_t minute, uint8_t second) {
  if (!m_isActive) {
    setCursor(col, row);
    if (hour < 10) { print("0"); }
    print(hour);
    print(":");
    if (minute < 10) { print("0"); }
    print(minute);
    print(":");
    if (second < 10) { print("0"); }
    print(second);
  }
}

/**************************************************************************/
/*!
    @brief  Print the day of the week.
    @return Day of week as an integer from 0 (Sunday) to 6 (Saturday).
*/
/**************************************************************************/
void LCD_2004_Menu::printDayOfTheWeek(uint8_t col, uint8_t row, uint8_t day) {
  if (!m_isActive) {
    setCursor(col, row);
    print(m_dayOfTheWeek[day]);
  }
}

void LCD_2004_Menu::printSensorValue(uint8_t col, uint8_t row, float value, const char* measure, const char* measure_2) {
  if (!m_isActive) {
    setCursor(col, row);
    print(value, 1);
    if (measure) { print(measure); }
    if (measure_2) { print(measure_2); }
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
  else {
    setCursor(0, 1);
    print("      NO MENU      ");
  }
}

void LCD_2004_Menu::setCurrentMenuItem(MenuItem* menuItem, size_t posCounter) {
  m_currentMenuItem = menuItem;
  m_currentMenuItem->setCounter(posCounter);
}

const char* LCD_2004_Menu::alignmentStr(const char* str) {
  if (strlen(str) == 20) {
    return str;
  }
  std::string currentStr = str;
  currentStr.resize(20, ' ');
  return currentStr.c_str();
}
