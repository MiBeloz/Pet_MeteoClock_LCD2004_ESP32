#include "MenuItem.h"

MenuItem::MenuItem(const char* name) : m_name(name) {
  for (uint8_t i = 0; i < 255; ++i) {
    m_submenuItem[i] = nullptr;
  }
  m_size = 0;
  m_counterSubmenuItemSelector = 0;
  while (m_name.length() < 17) {
    m_name += ' ';
  }
}

MenuItem::~MenuItem() {
  for (uint8_t i = 0; i < 255; ++i) {
    delete m_submenuItem[i];
  }
}

bool MenuItem::addSubmenuItem(SubmenuItem* submenuItem) {
  if (m_size == 255) {
    return false;
  }
  m_submenuItem[m_size] = submenuItem;
  ++m_size;
  return true;
}

SubmenuItem& MenuItem::getSubmenuItem(uint8_t pos) {
  return *m_submenuItem[pos];
}

String MenuItem::getName() const {
  return m_name;
}

SubmenuItem** MenuItem::getItems() {
  return m_submenuItem;
}

uint8_t MenuItem::getMenuItemsSize() const {
  return m_size;
}

uint8_t MenuItem::getCounter() const {
  return m_counterSubmenuItemSelector;
}

void MenuItem::incrementCounterItemSelector() {
  if (m_counterSubmenuItemSelector < m_size) {
    m_counterSubmenuItemSelector++;
    if (m_counterSubmenuItemSelector == m_size) {
      m_counterSubmenuItemSelector = 0;
    }
  }
}

void MenuItem::decrementCounterItemSelector() {
  if (m_counterSubmenuItemSelector > 0) {
    m_counterSubmenuItemSelector--;
  }
  else {
    m_counterSubmenuItemSelector = m_size - 1;
  }
}

void MenuItem::setCounterItemSelector(uint8_t i) {
  if (i >= m_size) {
    m_counterSubmenuItemSelector = m_size - 1;
  }
  else {
    m_counterSubmenuItemSelector = i;
  }
}

void MenuItem::invokeMenuItem() {
  return m_submenuItem[m_counterSubmenuItemSelector]->invokeSubmenuItem();
}
