#include "mainwindow.h"
#include <QApplication>

void load(QSplashScreen *splash)
{
    QTime time;
    time.start();

    for (int i = 0; i < 100;) {
        if (time.elapsed() > 10) {
            time.start();
            ++i;
        }

    splash->showMessage("Load modules: " + QString::number(i) + "%",
                        Qt::AlignCenter | Qt::AlignBottom, Qt::white);
    qApp->processEvents();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    QApplication a(argc, argv);

    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    settings.setValue(APPLICATION_NAME, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
    settings.sync();

    QString dir = QCoreApplication::applicationDirPath() + "/123.jpg";
    QSplashScreen splash(QPixmap(dir).scaled(700, 500, Qt::KeepAspectRatio));

    splash.show();

    MainWindow w;
    w.setWindowTitle("ToneMix");

    load(&splash);
    splash.finish(&w);

    w.show();

    return a.exec();
}
