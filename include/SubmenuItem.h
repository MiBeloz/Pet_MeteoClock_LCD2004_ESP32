#ifndef SUBMENUITEM_H
#define SUBMENUITEM_H

#include <Arduino.h>

class SubmenuItem {
public:
  SubmenuItem(const char* name);
  void setSubmenuItemFunction(void(*function)());
  String getSubmenuItemName() const;
  void invokeSubmenuItem();

private:
  String m_name;
  void(*m_function)();
};

#endif
