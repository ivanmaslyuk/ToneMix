#include "thememanager.h"

ThemeManager::ThemeManager() {}

void ThemeManager::dark()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette p = qApp->palette();
    p.setColor(QPalette::Window, QColor(53, 53, 53));
    p.setColor(QPalette::WindowText, QColor(255, 255, 255));
    p.setColor(QPalette::Base, QColor(15, 15, 15));
    p.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    p.setColor(QPalette::ToolTipBase , QColor(255, 255, 255));
    p.setColor(QPalette::ToolTipText, QColor(255, 255, 255));
    p.setColor(QPalette::Button, QColor(53, 53, 53));
    p.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    p.setColor(QPalette::BrightText, Qt::red);
    p.setColor(QPalette::Highlight, QColor(47, 79, 79).lighter());
    p.setColor(QPalette::HighlightedText, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

    qApp->setPalette(p);
}

void ThemeManager::standard()
{
    qApp->setPalette(qApp->style()->standardPalette());
    qApp->setStyle(QStyleFactory::create("WindowsVista"));
    qApp->setStyleSheet("");
}
