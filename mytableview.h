#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include <QtWidgets>
#include "nofocusproxystyle.h"

class MyTableView : public QTableView
{
    Q_OBJECT

public:
    MyTableView(int row, int column, QWidget *parent = nullptr);
    QStandardItemModel *tableViewModel = new QStandardItemModel;

    void fillLine(int rowIndex, QColor rowColor);
};

#endif // MYTABLEVIEW_H
