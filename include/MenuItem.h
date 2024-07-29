#ifndef MENUITEM_H
#define MENUITEM_H

#include <Arduino.h>
#include <vector>
#include <functional>

class MenuItem {
public:
  MenuItem(const char* name);

  ~MenuItem();

  bool addMenuItem(MenuItem* menu);

  // MenuItem& getMenuItem(size_t pos);

  String getMenuName() const;

  std::vector<MenuItem*> getMenuItems();

  size_t getMenuItemSize() const;

  size_t getCounter() const;

  void incrementMenuItemSelector();

  void decrementMenuItemSelector();

  void setMenuItemSelector(size_t i);

  void setMenuItemFunction(std::function<void()> function);

  void invokeMenuItem() const;

protected:
  MenuItem();

private:
  String m_name;
  std::vector<MenuItem*> m_menuItem;
  size_t m_counterSelector;
  std::function<void()> m_function;
};

#endif
