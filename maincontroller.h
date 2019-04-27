#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QDir>

#include "track.h"

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = nullptr);
    // добавляет один трек по пути к файлу
    void addSingleTrack(QString path);
    // устанавливает, нужно ли включать трек в генерируемые плейлисты
    void setIsTrackIncluded(int trackIndex, bool value);
    // устанавливает текущую открытую папку
    void setWorkingDirectory(QString path);
    // возвращает значение, возможно ли сейчас сгенерировать плейлист
    bool canGenerate();
    void setFirstTrack(int index);
    //bool canGoBack();

signals:
    //Сигнализирует о том, что генерация началась (и еще не завершилась).
    void generationInProgress();
    // сигнализирует о том, что началось сканирование файлов в выбранной папке
    // либо началось сканирование одного добавленного файла
    void scanningFiles();
    // Сигнализирует о том, что генерация плейлиста завершилась.
    void generated(QList<Track> playlist);
    // сигнализирует, когда изменилось значение [возможли ли сейчас сгенерировать плейлист]
    void canGenerateChanged(); // !!!!!!!!!!!
    void canGoBackChanged(bool);

public slots:
    // начинает генерацию нового плейлиста
    void generate();
    // возвращает предыдущий плейлист
    void restorePrevious();
    // устанавливает размер плейлиста
    void setPlaylistSize(int size);

private:
    // текущий плейлист
    QList<Track*> currentPlaylist;
    // хранит историю генераций
    QList<QList<Track*>> generationHistory;
    // хранит все треки, добавленные в программу
    QList<Track*> allFiles;
    // Выбранная папка.
    QDir workingDirectory;
    // Максимальный размер плейлиста.
    int playlistSize = 200;
    // Проверяет наличие повторяющихся треков в текущем плейлисте.
    void checkRepeated();
    // отправляет текущий плейлист через сигнал generated.
    void sendCurrentPlaylist();
};

#endif // MAINCONTROLLER_H
