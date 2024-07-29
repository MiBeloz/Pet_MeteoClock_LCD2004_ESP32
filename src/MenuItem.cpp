#include "MenuItem.h"

MenuItem::MenuItem(const char* name) : m_name(name), m_function(nullptr) {
  m_counterSelector = 0;
  while (m_name.length() < 17) {
    m_name += ' ';
  }
}

MenuItem::~MenuItem() {
  for (size_t i = 0; i < m_menuItem.size(); ++i) {
    delete m_menuItem[i];
  }
}

bool MenuItem::addMenuItem(MenuItem* menuItem) {
  if (m_menuItem.size() == m_menuItem.max_size()) {
    return false;
  }
  m_menuItem.push_back(menuItem);
  return true;
}

// MenuItem& MenuItem::getMenuItem(size_t pos) {
//   return *m_menuItem[pos];
// }

String MenuItem::getMenuName() const {
  return m_name;
}

std::vector<MenuItem*> MenuItem::getMenuItems() {
  return m_menuItem;
}

size_t MenuItem::getMenuItemSize() const {
  return m_menuItem.size();
}

size_t MenuItem::getCounter() const {
  return m_counterSelector;
}

void MenuItem::incrementMenuItemSelector() {
  if (m_counterSelector < m_menuItem.size()) {
    m_counterSelector++;
    if (m_counterSelector == m_menuItem.size()) {
      m_counterSelector = 0;
    }
  }
}

void MenuItem::decrementMenuItemSelector() {
  if (m_counterSelector > 0) {
    m_counterSelector--;
  }
  else {
    m_counterSelector = m_menuItem.size() - 1;
  }
}

void MenuItem::setMenuItemSelector(size_t i) {
  if (i >= m_menuItem.size()) {
    m_counterSelector = m_menuItem.size() - 1;
  }
  else {
    m_counterSelector = i;
  }
}

void MenuItem::setMenuItemFunction(std::function<void()> function) {
    m_function = function;
}

void MenuItem::invokeMenuItem() const {
  if (m_function) {
    m_function();
  }
}

MenuItem::MenuItem() : MenuItem("LCD Menu") {}
