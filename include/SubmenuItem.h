#ifndef SUBMENUITEM_H
#define SUBMENUITEM_H

#include <Arduino.h>

class SubmenuItem {
public:
  SubmenuItem(const char* name);

  void setMenuItemFunction(std::function<void()> function);

  String getName() const;

  void invokeMenuItem();

private:
  String m_name;
  std::function<void()> m_function;
};

#endif
