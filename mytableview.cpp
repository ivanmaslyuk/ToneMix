#include "mytableview.h"

MyTableView::MyTableView(int row, int column, QWidget *parent) : QTableView (parent)
{
    tableViewModel->setRowCount(row);
    tableViewModel->setColumnCount(column);
    tableViewModel->setHorizontalHeaderLabels(QStringList() << "" << "Трек" << "BPM" << "Тон");

    this->setModel(tableViewModel);

    this->verticalHeader()->setVisible(false); //удаляем номера строк
    this->verticalHeader()->setDefaultAlignment(Qt::AlignVCenter);
    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setShowGrid(false); //отрисовка таблицы
    this->setSelectionBehavior(QAbstractItemView::SelectRows); //выбираем только строки
    this->setStyle(new NoFocusProxyStyle()); //удаление пунктира у выбранных строк
}

void MyTableView::fillLine(int rowIndex, QColor rowColor)
{
    for (int i = 0; i < this->tableViewModel->columnCount(); i++) {
        this->tableViewModel->item(rowIndex, i)->setBackground(rowColor);
    }
}


