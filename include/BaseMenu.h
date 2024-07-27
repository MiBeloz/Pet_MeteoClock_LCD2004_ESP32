#ifndef BASEMENU
#define BASEMENU

#include <Arduino.h>

class BaseMenu {
public:
    virtual String getName() const = 0;
    virtual bool addMenuItem(BaseMenu* menu) = 0;
    virtual BaseMenu& getMenuItem(uint8_t pos) = 0;
    virtual void invokeMenuItem() = 0;
    virtual void setMenuItemFunction(void(*function)()) = 0;
};

#endif