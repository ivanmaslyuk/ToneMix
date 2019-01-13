#ifndef NOFOCUSPROXYSTYLE_H
#define NOFOCUSPROXYSTYLE_H

#include <QProxyStyle>

class NoFocusProxyStyle : public QProxyStyle {
public:

    NoFocusProxyStyle(QStyle *baseStyle = 0) : QProxyStyle(baseStyle) {}

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
        if(element == QStyle::PE_FrameFocusRect) {
            return;
        }
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }

};

#endif // NOFOCUSPROXYSTYLE_H
