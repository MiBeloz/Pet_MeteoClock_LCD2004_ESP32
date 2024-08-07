#ifndef MENUITEM_H
#define MENUITEM_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstddef>
#endif
#include <vector>
#include <functional>

class MenuItem {
public:
  MenuItem(const char* name);

  virtual ~MenuItem();

  bool addMenuItem(MenuItem* menu);

  MenuItem* getMenuItem(size_t pos) const;

  std::string getMenuName() const;

  size_t getMenuItemCount() const;

  size_t getCounter() const;

  void incrementCounter();

  void decrementCounter();

  bool setCounter(size_t i);

  void setFunction(std::function<void()> function);

  void invoke() const;

private:
  inline static uint8_t m_maxNameLength = 17;
  std::string m_name;
  size_t m_counter;
  std::function<void()> m_function;
  std::vector<MenuItem*> m_menuItem;
};

#endif
