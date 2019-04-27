#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QtWidgets>

// Управляет оформлением программы.
class ThemeManager
{
public:
    ThemeManager();

    // Применяет тёмную тему оформаления.
    void dark();

    // Применяет светлую тему оформаления.
    void standard();
};

#endif // THEMEMANAGER_H
