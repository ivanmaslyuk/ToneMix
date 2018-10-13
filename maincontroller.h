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
    void addSingleTrack(QString path);
    void setPlaylistSize(int size);
    void setIsTrackIncluded(int trackIndex, bool value);
    void setWorkingDirectory(QString path);
    bool canGenerate();

signals:
    /**
     * @brief Сигнализирует о том, что генерация началась (и еще не завершилась).
     */
    void generationInProgress();
    void scanningFiles();
    /**
     * @brief Сигнализирует о том, что генерация плейлиста завершилась.
     * @param Готовый плейлист.
     */
    void generated(QList<Track> playlist);
    void finishedScanningFiles();
    void canGenerateChanged(bool value); // !!!!!!!!!!!

public slots:
    void generate();
    void restorePrevious();

private:
    QList<Track> currentPlaylist;
    QList<QList<Track>*> generationHistory;
    QList<Track*> allFiles;
    QDir workingDirectory;
    int playlistSize;
};

#endif // MAINCONTROLLER_H
