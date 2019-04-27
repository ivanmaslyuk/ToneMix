#ifndef NOFOCUSPROXYSTYLE_H
#define NOFOCUSPROXYSTYLE_H

#include <QProxyStyle>

class NoFocusProxyStyle : public QProxyStyle {
    Q_OBJECT
public:

    NoFocusProxyStyle(QStyle *baseStyle = nullptr) : QProxyStyle(baseStyle) {}

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
        // Отменяем рисовку стандартных рамок Windows вокруг выбранных элементов.
        if(element == QStyle::PE_FrameFocusRect) {
            return;
        }
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }

};

#endif // NOFOCUSPROXYSTYLE_H
