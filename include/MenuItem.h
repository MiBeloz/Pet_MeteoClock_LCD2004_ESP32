#ifndef MENUITEM_H
#define MENUITEM_H

#include "BaseMenu.h"

class MenuItem : public BaseMenu {
public:
  MenuItem(const char* name);
  ~MenuItem();

  bool addMenuItem(BaseMenu* menu);
  BaseMenu& getMenuItem(uint8_t pos);
  String getName() const override;
  BaseMenu** getItems();
  uint8_t getMenuItemsSize() const;
  uint8_t getCounter() const;
  void incrementCounterItemSelector();
  void decrementCounterItemSelector();
  void setCounterItemSelector(uint8_t i);
  void invokeMenuItem();
  void setMenuItemFunction(void(*function)()) override {}

protected:
  String m_name;
  BaseMenu* m_baseMenu[255];
  uint8_t m_size;
  int8_t m_counterBaseMenuSelector;
};

#endif
