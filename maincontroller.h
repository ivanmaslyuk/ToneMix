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
    // устанавливает размер плейлиста
    void setPlaylistSize(int size);
    // устанавливает, нужно ли включать трек в генерируемые плейлисты
    void setIsTrackIncluded(int trackIndex, bool value);
    // устанавливает текущую открытую папку
    void setWorkingDirectory(QString path);
    // возвращает значение, возможно ли сейчас сгенерировать плейлист
    bool canGenerate();

signals:
    //Сигнализирует о том, что генерация началась (и еще не завершилась).
    void generationInProgress();
    // сигнализирует о том, что началось сканирование файлов в выбранной папке
    // либо началось сканирование одного добавленного файла
    void scanningFiles();
    // Сигнализирует о том, что генерация плейлиста завершилась.
    void generated(QList<Track> playlist);
    // сигнализирует о том, что сканирование файлов завершено
    void finishedScanningFiles();
    // сигнализирует, когда изменилось значение [возможли ли сейчас сгенерировать плейлист]
    void canGenerateChanged(); // !!!!!!!!!!!

public slots:
    // начинает генерацию нового плейлиста
    void generate();
    // возвращает предыдущий плейлист
    void restorePrevious();

private:
    QList<Track> currentPlaylist;
    QList<QList<Track>*> generationHistory;
    QList<Track*> allFiles;
    QDir workingDirectory;
    int playlistSize;
};

#endif // MAINCONTROLLER_H
