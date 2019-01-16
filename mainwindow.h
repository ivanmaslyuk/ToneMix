#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <track.h>
#include <QtWidgets>
#include "maincontroller.h"
#include "track.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QTableWidget *table = new QTableWidget;
    QWidget *widget = new QWidget;
    MainController *controller = new MainController;
    QSpinBox *countOfTrackInPlaylist = new QSpinBox();
    QPushButton *cancel = new QPushButton("Назад");
    QPushButton *generate = new QPushButton("Сгенерировать");
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<Track> tracks;
    QListWidget *l2;
    QAction *addAct;

    int column();
    void settingTable();
    void createActions();
    void hotKeys();

    QShortcut *keyCtrlG;
    QShortcut *keyCtrlZ;
    QShortcut *keyCtrlO;
    QShortcut *keyCtrlA;

    QRadioButton *colorWhite;
    QRadioButton *colorDark;

private slots:
    void setFirstTrack();
    void browse();
    void add();
    void getPlaylist(QList<Track>);
    void white();
    void dark();
    void checking(QList<Track> trackList);
    void cancelButton(bool);
    void boxState(QTableWidgetItem*);
    void scanning();
    void canGenerateChanged();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // MAINWINDOW_H
