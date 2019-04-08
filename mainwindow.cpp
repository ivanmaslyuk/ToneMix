#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tagreader.h"
#include "harmonicplaylistgenerator.h"
#include "maincontroller.h"
#include "nofocusproxystyle.h"
#include "track.h"
#include <QtWidgets>
#include "tonenotationtranslator.h"
#include "thememanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    hotKeys();

    QVBoxLayout *vLayout = new QVBoxLayout;
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    QHBoxLayout *topLayout = new QHBoxLayout;

    QPushButton *Browse = new QPushButton("Обзор...");
    QPushButton *Add = new QPushButton("Добавить");
    QLabel *labelByTracks = new QLabel("Максимальное количество треков в плейлисте");

    //коннекты
    connect(Browse, SIGNAL(clicked()), this, SLOT(browse()));
    connect(Add, SIGNAL(clicked()), this, SLOT(add()));
    connect(cancel, SIGNAL(clicked()), controller, SLOT(restorePrevious()));
    connect(controller, SIGNAL(canGoBackChanged(bool)), this, SLOT(cancelButton(bool)));
    connect(generate, SIGNAL(clicked()), SLOT(checking(QList<Track> trackList)));
    connect(generate, SIGNAL(clicked()), controller, SLOT(generate()));
    connect(controller, SIGNAL(generated(QList<Track>)), this, SLOT(getPlaylist(QList<Track>)));
    connect(countOfTrackInPlaylist, SIGNAL(valueChanged(int)), controller, SLOT(setPlaylistSize(int)));
    connect(controller, SIGNAL(scanningFiles()), this, SLOT(scanning()));
    connect(controller, SIGNAL(canGenerateChanged()), this, SLOT(canGenerateChanged()));

    //добавление всех виджетов в верхний слой
    topLayout->addWidget(Browse);
    topLayout->addWidget(Add);
    topLayout->addWidget(cancel);
    topLayout->addWidget(generate);
    topLayout->addWidget(labelByTracks);
    topLayout->addWidget(countOfTrackInPlaylist);

    QHBoxLayout *bottom = new QHBoxLayout;
    bottom->addWidget(tableView);

    //объединяем слои и устанавливаем центральным виджетом
    vLayout->addLayout(topLayout);
    vLayout->addLayout(bottomLayout);
    vLayout->addLayout(bottom);
    widget->setLayout(topLayout);
    widget->setLayout(vLayout);
    setCentralWidget(widget);

    //настройка спинбокса
    countOfTrackInPlaylist->setMaximum(1000);
    countOfTrackInPlaylist->setValue(1000);

    //делаем кнопки отмены и генерации неактивными
    cancel->setEnabled(false);
    generate->setEnabled(false);

    //удаление пунктира у выбранных строк
    table->setStyle(new NoFocusProxyStyle());

    statusBar()->showMessage("Пусто. Выберите треки, нажав \"Обзор\" или \"Добавить\".");

    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::column()
{
    int m = 4;
    return m;
}

void MainWindow::setFirstTrack()
{
    int rowOfSelectedItemTableView = tableView->selectionModel()->currentIndex().row();
    qDebug() << rowOfSelectedItemTableView;

    if (tableView->tableViewModel->item(rowOfSelectedItemTableView,  0)->checkState() == Qt::Checked) {
        QMessageBox::warning(this, "Ошибка", "Невозможно установить этот трек первым, так как он исключен "
                                             "из генерации");
        return;
    }

    QMessageBox::StandardButton choice = QMessageBox::question(this, "Выбор",
                                                            "Вы действительно хотите установить этот"
                                                            " трек первым?",
                                                               QMessageBox::Yes | QMessageBox::No);


    if(choice == QMessageBox::Yes)
    {
        controller->setFirstTrack(rowOfSelectedItemTableView);

    }
}

void MainWindow::browse()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выбор папки"));
    if (dir.isEmpty())
        return;
    controller->setWorkingDirectory(dir);
}

void MainWindow::add()
{
    QString dir = QFileDialog::getOpenFileName(this, tr("Выбор файла"), "", tr("Tracks (*.mp3)"));
    if (dir.isEmpty())
        return;

    controller->addSingleTrack(dir);
}

void MainWindow::getPlaylist(QList<Track> trackList)
{
    tableView->tableViewModel->setRowCount(trackList.size());
    statusBar()->showMessage("Количество песен в плейлисте: " + QString::number(trackList.size()));

    //добавление элементов в таблицу
    for(int i = 0; i < trackList.size(); i++) {
//            tableView->setRowHeight(i, 15); // херово робит

            QStandardItem *checkBoxTableView = new QStandardItem;
            if (trackList[i].excluded) {
                checkBoxTableView->setCheckable(true);
                checkBoxTableView->setCheckState(Qt::Checked);
            }
            else {
                checkBoxTableView->setCheckable(true);
                checkBoxTableView->setCheckState(Qt::CheckState(false));
            }
            tableView->tableViewModel->setItem(i, 0, checkBoxTableView);

            if (trackList[i].title != "") {
                QStandardItem *trackTableView = new QStandardItem(trackList[i].title);
                tableView->tableViewModel->setItem(i, 1, trackTableView);
            }
            else {
                trackList[i].path = trackList[i].path.split("/").last();
                QStandardItem *trackTableView = new QStandardItem(trackList[i].path);
                tableView->tableViewModel->setItem(i, 1, trackTableView);
            }

            if(trackList[i].bpm == 0) {
                QStandardItem *bmpTableView = new QStandardItem(QString("?"));
                tableView->tableViewModel->setItem(i, 2, bmpTableView);
            }
            else {
                QStandardItem *bmpTableView = new QStandardItem(trackList[i].bpmAsString());
                tableView->tableViewModel->setItem(i, 2, bmpTableView);
            }

            ToneNotationTranslator translator;

            QStandardItem *toneTableView = new QStandardItem(translator.toCamelot(trackList[i].toneRaw));
            tableView->tableViewModel->setItem(i, 3, toneTableView);

            if (trackList[i].repeatedInPlaylist)
                tableView->fillLine(i, "#ffa7a7");
            }

    tableView->setModel(tableView->tableViewModel);
    tableView->resizeColumnsToContents();
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    connect(tableView->tableViewModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(boxState(QStandardItem *)));
}

void MainWindow::hotKeys()
{
    //генерация
    keyCtrlG = new QShortcut(this);
    keyCtrlG->setKey(Qt::CTRL + Qt::Key_G);
    connect(keyCtrlG, SIGNAL(activated()), controller, SLOT(generate()));

    //открыть для добавления директории
    keyCtrlO = new QShortcut(this);
    keyCtrlO->setKey(Qt::CTRL + Qt::Key_O);
    connect(keyCtrlO, SIGNAL(activated()), this, SLOT(browse()));

    //открыть для добавления файла
    keyCtrlA = new QShortcut(this);
    keyCtrlA->setKey(Qt::CTRL + Qt::Key_A);
    connect(keyCtrlA, SIGNAL(activated()), this, SLOT(add()));

    //отмена
    keyCtrlZ = new QShortcut(this);
    keyCtrlZ->setKey(Qt::CTRL + Qt::Key_Z);
    connect(keyCtrlZ, SIGNAL(activated()), this, SLOT(restorePrevious()));
}

void MainWindow::checking(QList<Track> trackList)
{
    for (int i = 0; i < trackList.size(); i++)
    {
        QStandardItem  *checkbox(tableView->tableViewModel->item(i, 0));

        if (checkbox) {
            Qt::CheckState state = checkbox->checkState();
            controller->setIsTrackIncluded(i, !state);
        }
    }
}

void MainWindow::cancelButton(bool active)
{
    cancel->setEnabled(active);
}

void MainWindow::boxState(QStandardItem *item)
{
    if (tableView->tableViewModel->item(item->row(), 0) == item) {
        bool newValue = item->checkState() == Qt::CheckState::Checked ? false : true;
        controller->setIsTrackIncluded(item->row(), newValue);
        qDebug() << item << ", " << tableView->tableViewModel->item(item->row(), 0) << ", " << item->checkState() << ", " << newValue << ", " << item->row();
    }
}

void MainWindow::scanning()
{
    statusBar()->showMessage("Подождите, идёт загрузка!");
}

void MainWindow::canGenerateChanged()
{
    generate->setEnabled(controller->canGenerate());
}

void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *setFirstTrack = new QAction(tr("Назначить первым"), this);
    connect(setFirstTrack, SIGNAL(triggered()), this, SLOT(setFirstTrack()));
    menu->addAction(setFirstTrack);
    menu->popup(tableView->viewport()->mapToGlobal(pos));
}
