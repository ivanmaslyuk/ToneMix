#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tagreader.h"
#include <QDir>
#include "harmonicplaylistgenerator.h"
#include <QFileDialog>

#include <QListWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDir directory = QFileDialog::getExistingDirectory(this, tr("select directo"));
    // читаем файлы
    TagReader reader(directory.path(), directory.entryList(QStringList() << "*.mp3" << "*.MP3", QDir::Files)); // конструктор TagReader нужно переделать, чтобы он принимал полные пути
    QList<Track> tracks = reader.read();
    // сортируем
    HarmonicPlaylistGenerator generator;
    QList<Track> compatible = generator.listCompatible(tracks);
    QList<Track> sorted = generator.harmonicSort(tracks);
    QList<Track> compatSorted = generator.harmonicSort(compatible);
    //выводим
    QListWidget *l1 = new QListWidget(this);
    foreach (Track track, tracks) {
        l1->addItem(track.keyAsString() + "\t" + track.bpmAsString() + "\t" + track.artist + "\t\t\t" + track.title);
    }
    ui->horizontalLayout->addWidget(l1);
    QListWidget *l2 = new QListWidget(this);
    foreach(Track track, sorted) {
        l2->addItem(track.keyAsString() + "\t" + track.bpmAsString() + "\t" + track.artist + "\t\t\t" + track.title);
    }
    ui->horizontalLayout->addWidget(l2);
    QListWidget *l3 = new QListWidget(this);
    foreach(Track track, compatSorted) {
        l3->addItem(track.keyAsString() + "\t" + track.bpmAsString() + "\t" + track.artist + "\t\t\t" + track.title);
    }
    ui->horizontalLayout->addWidget(l3);
}

MainWindow::~MainWindow()
{
    delete ui;
}
