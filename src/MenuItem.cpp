#include "MenuItem.h"

MenuItem::MenuItem(const char* name) : m_name(name) {
  for (uint8_t i = 0; i < 255; ++i) {
    m_baseMenu[i] = nullptr;
  }
  m_size = 0;
  m_counterBaseMenuSelector = 0;
  while (m_name.length() < 17) {
    m_name += ' ';
  }
}

MenuItem::~MenuItem() {
  for (uint8_t i = 0; i < 255; ++i) {
    delete m_baseMenu[i];
  }
}

bool MenuItem::addMenuItem(BaseMenu* submenuItem) {
  if (m_size == 255) {
    return false;
  }
  m_baseMenu[m_size] = submenuItem;
  ++m_size;
  return true;
}

BaseMenu& MenuItem::getMenuItem(uint8_t pos) {
  return *m_baseMenu[pos];
}

String MenuItem::getName() const {
  return m_name;
}

BaseMenu** MenuItem::getItems() {
  return m_baseMenu;
}

uint8_t MenuItem::getMenuItemsSize() const {
  return m_size;
}

uint8_t MenuItem::getCounter() const {
  return m_counterBaseMenuSelector;
}

void MenuItem::incrementCounterItemSelector() {
  if (m_counterBaseMenuSelector < m_size) {
    m_counterBaseMenuSelector++;
    if (m_counterBaseMenuSelector == m_size) {
      m_counterBaseMenuSelector = 0;
    }
  }
}

void MenuItem::decrementCounterItemSelector() {
  if (m_counterBaseMenuSelector > 0) {
    m_counterBaseMenuSelector--;
  }
  else {
    m_counterBaseMenuSelector = m_size - 1;
  }
}

void MenuItem::setCounterItemSelector(uint8_t i) {
  if (i >= m_size) {
    m_counterBaseMenuSelector = m_size - 1;
  }
  else {
    m_counterBaseMenuSelector = i;
  }
}

void MenuItem::invokeMenuItem() {
  return m_baseMenu[m_counterBaseMenuSelector]->invokeMenuItem();
}
