#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <track.h>
#include <QtWidgets>
#include "maincontroller.h"
#include "track.h"
#include "mytableview.h"


#define ORGANIZATION_NAME "WorkDj"
#define ORGANIZATION_DOMAIN "www.workdj.ru"
#define APPLICATION_NAME "ToneMix"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QTableWidget *table = new QTableWidget;
    QWidget *widget = new QWidget;
    MainController *controller = new MainController;
    QSpinBox *countOfTrackInPlaylist = new QSpinBox();
    QPushButton *cancel = new QPushButton("Назад");
    QPushButton *generate = new QPushButton("Сгенерировать");
    MyTableView *tableView = new MyTableView(0, 4);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<Track> tracks;
    QListWidget *l2;
    QAction *addAct;

    int column();
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
    void checking(QList<Track> trackList);
    void cancelButton(bool);
    void boxState(QStandardItem *);
    void scanning();
    void canGenerateChanged();
    void slotCustomMenuRequested(QPoint pos);
};

#endif // MAINWINDOW_H
