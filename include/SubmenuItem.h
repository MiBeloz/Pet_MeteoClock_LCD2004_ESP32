#ifndef SUBMENUITEM_H
#define SUBMENUITEM_H

#include <Arduino.h>

#include "BaseMenu.h"

class SubmenuItem : public BaseMenu {
public:
  SubmenuItem(const char* name);
  bool addMenuItem(BaseMenu* menu) override {}
  BaseMenu& getMenuItem(uint8_t pos) override {}
  void setMenuItemFunction(void(*function)()) override;
  String getName() const override;
  void invokeMenuItem() override;

private:
  String m_name;
  void(*m_function)();
};

#endif
