#include "SubmenuItem.h"

SubmenuItem::SubmenuItem(const char* name) : m_name(name) {
    while (m_name.length() < 17) {
        m_name += ' ';
    }
    m_function = nullptr;
}

void SubmenuItem::setMenuItemFunction(std::function<void()> function) {
    m_function = function;
}

String SubmenuItem::getName() const {
    return m_name;
}

void SubmenuItem::invokeMenuItem() {
    if (m_function) {
        m_function();
    }
}
