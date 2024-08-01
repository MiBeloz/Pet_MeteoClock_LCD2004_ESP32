#include "MenuItem.h"

MenuItem::MenuItem(const char* name)
: m_name(name), m_counter(0), m_function(nullptr) {
  if (m_name.length() > m_maxNameLength) {
    m_name = m_name.substr(0, m_maxNameLength);
  }
  while (m_name.length() < m_maxNameLength) {
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

MenuItem* MenuItem::getMenuItem(size_t pos) const {
  return m_menuItem[pos];
}

std::string MenuItem::getMenuName() const {
  return m_name;
}

size_t MenuItem::getMenuItemCount() const {
  return m_menuItem.size();
}

size_t MenuItem::getCounter() const {
  return m_counter;
}

void MenuItem::incrementCounter() {
  if (m_counter < m_menuItem.size()) {
    m_counter++;
    if (m_counter == m_menuItem.size()) {
      m_counter = 0;
    }
  }
}

void MenuItem::decrementCounter() {
  if (m_counter > 0) {
    m_counter--;
  }
  else {
    m_counter = m_menuItem.size() - 1;
  }
}

bool MenuItem::setCounter(size_t i) {
  if (i >= m_menuItem.size()) {
    m_counter = m_menuItem.size() - 1;
    return false;
  }
  m_counter = i;
  return true;
}

void MenuItem::setFunction(std::function<void()> function) {
    m_function = function;
}

void MenuItem::invoke() const {
  if (m_function) {
    m_function();
  }
}
