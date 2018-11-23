#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tagreader.h"
#include <QDir>
#include "harmonicplaylistgenerator.h"
#include <QFileDialog>
#include <track.h>
#include "maincontroller.h"
#include <QtWidgets>
#include <QVariant>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settingTable();
    hotKeys();
    createActions();

    QVBoxLayout *vLayout = new QVBoxLayout;
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    QHBoxLayout *topLayout = new QHBoxLayout;

    QPushButton *Browse = new QPushButton("Обзор...");
    QPushButton *Add = new QPushButton("Добавить");
    QPushButton *generate = new QPushButton("Сгенерировать");
    QLabel *labelByTracks = new QLabel("Максимальное количество треков в плейлисте");

    cancel->setEnabled(false);


//    colorWhite = new QRadioButton("Стандартная тема");
//    colorDark = new QRadioButton("Тёмная тема");
//    QVBoxLayout *radio = new QVBoxLayout;
//    QObject::connect(colorDark, SIGNAL(clicked()), SLOT(dark()));
//    QObject::connect(colorWhite, SIGNAL(clicked()), SLOT(white()));

//    colorWhite->setChecked(true);
//    radio->addWidget(colorWhite);
//    radio->addWidget(colorDark);


    //коннекты
    {
        QObject::connect(Browse, SIGNAL(clicked()), this, SLOT(browse()));
        QObject::connect(Add, SIGNAL(clicked()), this, SLOT(add()));
        QObject::connect(cancel, SIGNAL(clicked()), controller, SLOT(restorePrevious()));
        QObject::connect(controller, SIGNAL(canGoBackChanged(bool)), this, SLOT(cancelButton(bool)));
        QObject::connect(generate, SIGNAL(clicked()), SLOT(checking(QList<Track> trackList)));
        QObject::connect(generate, SIGNAL(clicked()), controller, SLOT(generate()));
        QObject::connect(controller, SIGNAL(generated(QList<Track>)), this, SLOT(getPlaylist(QList<Track>)));
        QObject::connect(countOfTrackInPlaylist, SIGNAL(valueChanged(int)), controller, SLOT(setPlaylistSize(int)));
    }

    countOfTrackInPlaylist->setMaximum(1000);
    countOfTrackInPlaylist->setValue(1000);


    //добавление всех виджетов в верхний слой
    {
        topLayout->addWidget(Browse);
        topLayout->addWidget(Add);
        topLayout->addWidget(cancel);
        topLayout->addWidget(generate);
        topLayout->addWidget(labelByTracks);
        topLayout->addWidget(countOfTrackInPlaylist);
        bottomLayout->addWidget(table);
    }

    //объединяем слои и устанавливаем центральным виджетом
    {
        vLayout->addLayout(topLayout);
        vLayout->addLayout(bottomLayout);
        //vLayout->addLayout(radio);
        widget->setLayout(topLayout);
        widget->setLayout(vLayout);
        //widget->setLayout(radio);
        setCentralWidget(widget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::settingTable()
{
    //устанавливаем размер нашей таблицы
    {
        table->setRowCount(0);
        table->setColumnCount(colomn());
    }

    //устанавливаем названия колонок
    {
        QStringList lst;
        lst << "" << "Track" << "Bmp" << "Tone";
        table->setHorizontalHeaderLabels(lst);
    }

    //настраиваем столбцы
    {
        table->verticalHeader()->setVisible(false); //удаляем номера строк
        table->verticalHeader()->setDefaultAlignment(Qt::AlignVCenter);
        table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setShowGrid(false); //отрисовка таблицы
        table->setSelectionBehavior(QAbstractItemView::SelectRows); //выбираем только строки
    }
}

int MainWindow::colomn()
{
    int m = 4;
    return m;
}


void MainWindow::createActions()
{
    addAct = new QAction(tr("Назначить первым"), this);
    QObject::connect(addAct, SIGNAL(triggered()), this, SLOT(setFirstTrack()));
}


void MainWindow::setFirstTrack()
{
    QMessageBox::StandardButton choice = QMessageBox::question(this, "Выбор",
                                                            "Вы действительно хотите установить этот"
                                                            " трек первым?",
                                                               QMessageBox::Yes | QMessageBox::No);


    if(choice == QMessageBox::Yes)
    {
        int rowOfSelectedItem = table->currentRow();
        qDebug() << rowOfSelectedItem;
        controller->setFirstTrack(rowOfSelectedItem);

    }
}


void MainWindow::browse()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выбор папки"));
    controller->setWorkingDirectory(dir);
}


void MainWindow::add()
{
    QString dir = QFileDialog::getOpenFileName(this, tr("Выбор файла"));
    controller->addSingleTrack(dir);
}


void MainWindow::getPlaylist(QList<Track> trackList)
{
    table->setRowCount(trackList.size());

    statusBar()->showMessage("Количество песен в плейлисте: " + QString::number(trackList.size()));

    //добавление элементов в таблицу
    for(int i = 0; i < trackList.size(); i++)
    {

            QTableWidgetItem *checkBox = new QTableWidgetItem;
            checkBox->setCheckState(Qt::CheckState(false));
            table->setItem(i, 0, checkBox);


            if (trackList[i].title != "")
            {
                QTableWidgetItem *track = new QTableWidgetItem(trackList[i].title);
                table->setItem(i, 1, track);
            }
            else
            {
                trackList[i].path = trackList[i].path.split("/").last();
                QTableWidgetItem *track = new QTableWidgetItem(trackList[i].path);
                table->setItem(i, 1, track);
            }

            if(trackList[i].bpm == 0)
            {
                QTableWidgetItem *bmp = new QTableWidgetItem(QString("?"));
                table->setItem(i, 2, bmp);
            }
            else
            {
                QTableWidgetItem *bmp = new QTableWidgetItem(trackList[i].bpmAsString());
                table->setItem(i, 2, bmp);
            }

            if(trackList[i].keyAsString() == "" || trackList[i].num == 0)
            {
                QTableWidgetItem *tone = new QTableWidgetItem(QString("?"));
                table->setItem(i, 3, tone);
            }
            else
            {
                QTableWidgetItem *tone = new QTableWidgetItem(trackList[i].keyAsString());
                table->setItem(i, 3, tone);
            }
    }
    QObject::connect(table, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(boxState(QTableWidgetItem*)));

}


void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(addAct);
    menu.exec(event->globalPos());
}


void MainWindow::hotKeys()
{
    //генерация
    keyCtrlG = new QShortcut(this);
    keyCtrlG->setKey(Qt::CTRL + Qt::Key_G);
    QObject::connect(keyCtrlG, SIGNAL(activated()), controller, SLOT(generate()));

    //открыть для добавления директории
    keyCtrlO = new QShortcut(this);
    keyCtrlO->setKey(Qt::CTRL + Qt::Key_O);
    QObject::connect(keyCtrlO, SIGNAL(activated()), this, SLOT(browse()));

    //открыть для добавления файла
    keyCtrlA = new QShortcut(this);
    keyCtrlA->setKey(Qt::CTRL + Qt::Key_A);
    QObject::connect(keyCtrlA, SIGNAL(activated()), this, SLOT(add()));

    //отмена
    keyCtrlZ = new QShortcut(this);
    keyCtrlZ->setKey(Qt::CTRL + Qt::Key_Z);
    QObject::connect(keyCtrlZ, SIGNAL(activated()), this, SLOT(restorePrevious()));
}


void MainWindow::dark()
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


void MainWindow::white()
{
    qApp->setPalette(qApp->style()->standardPalette());
    qApp->setStyle(QStyleFactory::create("WindowsVista"));
    qApp->setStyleSheet("");
}


void MainWindow::checking(QList<Track> trackList)
{
    for (int i = 0; i < trackList.size(); i++)
    {
        QTableWidget *tableWidget(table);
        QTableWidgetItem  *checkbox(tableWidget->item(i, 0));
        if (checkbox)
        {
            Qt::CheckState state = checkbox->checkState();
            if (state == true)
                controller->setIsTrackIncluded(i, false);
            else
                controller->setIsTrackIncluded(i, true);
        }
    }
}


void MainWindow::cancelButton(bool active)
{
    cancel->setEnabled(active);
}


void MainWindow::boxState(QTableWidgetItem* item)
{
    bool newValue = item->checkState() == Qt::CheckState::Checked ? true : false;
    controller->setIsTrackIncluded(item->row(), newValue);
}
