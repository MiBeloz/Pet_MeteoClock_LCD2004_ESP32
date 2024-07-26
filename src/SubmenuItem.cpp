#include "SubmenuItem.h"

SubmenuItem::SubmenuItem(const char* name) : m_name(name) {
    while (m_name.length() < 17) {
        m_name += ' ';
    }
    m_function = nullptr;
}

void SubmenuItem::setSubmenuItemFunction(void(*function)()) {
    m_function = function;
}

String SubmenuItem::getSubmenuItemName() const {
    return m_name;
}

void SubmenuItem::invokeSubmenuItem() {
    if (m_function) {
        m_function();
    }
}
