#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tagreader.h"
#include <QDir>
#include "harmonicplaylistgenerator.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDir directory = QFileDialog::getExistingDirectory(this, tr("select directo"));
    TagReader reader(directory.path(), directory.entryList(QStringList() << "*.mp3" << "*.MP3", QDir::Files));

    QList<Track> tracks = reader.read();
    HarmonicPlaylistGenerator generator;
    QList<Track> sorted = generator.harmonicSort(tracks);
    int s = sorted.size();
    int i = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
