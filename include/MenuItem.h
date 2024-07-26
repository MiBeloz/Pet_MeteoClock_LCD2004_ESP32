#ifndef MENUITEM_H
#define MENUITEM_H

#include "SubmenuItem.h"

class MenuItem {
public:
  MenuItem(const char* name);
  ~MenuItem();

  bool addSubmenuItem(SubmenuItem* submenuItem);
  SubmenuItem& getSubmenuItem(uint8_t pos);
  String getName() const;
  SubmenuItem** getItems();
  uint8_t getMenuItemsSize() const;
  uint8_t getCounter() const;
  void incrementCounterItemSelector();
  void decrementCounterItemSelector();
  void setCounterItemSelector(uint8_t i);
  void invokeMenuItem();

protected:
  String m_name;
  SubmenuItem* m_submenuItem[255];
  uint8_t m_size;
  int8_t m_counterSubmenuItemSelector;
};

#endif
