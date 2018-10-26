#include "maincontroller.h"
#include "harmonicplaylistgenerator.h"
#include "tagreader.h"
#include "emptyplaylistexception.h"
#include <QStringList>
//Установка первого трека
//Отображение списка треков сразу при добавлении

MainController::MainController(QObject *parent) : QObject(parent)
{

}

void MainController::generate()
{
    if (canGenerate()) {
        emit generationInProgress();

        QList<Track> *copy = new QList<Track>(currentPlaylist);
        generationHistory.append(copy);

        // составляем список неисключенных из генерации треков для их передачи генератору
        QList<Track> included;
        foreach (Track *track, allFiles) {
            if (!track->excluded)
                included.append(*track);
        }

        currentPlaylist = HarmonicPlaylistGenerator::harmonicSort(included, true);

        // обрезаем плейлист чтобы его размер не привышал playlistSize
        while (currentPlaylist.size() > playlistSize)
            currentPlaylist.removeLast();

        emit generated(currentPlaylist);

        if (generationHistory.size() > 1) emit canGoBackChanged(true);
    }
    else {
        // если файлы еще не были выбраны
        EmptyPlaylistException e;
        e.raise();
    }
}

void MainController::restorePrevious()
{
    if (generationHistory.size() > 0) {
        //generationHistory.removeLast();
        currentPlaylist = *(generationHistory.takeLast());
        emit generated(currentPlaylist);
        if (generationHistory.size() < 1) emit canGoBackChanged(false);
    }
    else qDebug() << "нет предыдущих версий. невозможно вернуться";
}

void MainController::setWorkingDirectory(QString path)
{
    emit scanningFiles();

    workingDirectory = QDir(path);
    QStringList filesInWorkingDirectory = workingDirectory
                                            .entryList(QStringList() << "*.mp3" << "*.MP3", QDir::Files)
                                            .replaceInStrings(QRegExp("^"), workingDirectory.absolutePath() + "/");
    TagReader tagReader = TagReader(filesInWorkingDirectory);
    allFiles = tagReader.read();

    emit finishedScanningFiles();

    if (allFiles.size() > 0) {
        emit canGenerateChanged();
        //QList<Track> _allFiles;
        foreach (Track *tr, allFiles)
            currentPlaylist.append(*tr);
        emit generated(currentPlaylist);
    }

}

void MainController::addSingleTrack(QString path)
{
    emit scanningFiles();
    TagReader tagReader = TagReader(QStringList() << path);
    QList<Track*> file = tagReader.read();
    allFiles.append(file);
    emit finishedScanningFiles();

    // тут возможено косяки
    currentPlaylist.append(*(file[0]));
    emit generated(currentPlaylist);

    if (allFiles.size() > 0)
        emit canGenerateChanged();
}

bool MainController::canGenerate()
{
    return allFiles.size() > 0;
}

void MainController::setPlaylistSize(int size)
{
    this->playlistSize = size;
}

void MainController::setIsTrackIncluded(int trackIndex, bool value)
{
    // моенять на указатель
    //currentPlaylist[trackIndex].excluded = value;

    QString trackPath = currentPlaylist[trackIndex].path;
    for (int i = 0; i < allFiles.size(); i++)
        if (allFiles[i]->path == trackPath) {
            allFiles[i]->excluded = value;
            break;
        }
}

void MainController::setFirstTrack(int index)
{
    //allFiles.move(index, 0); return; // говнокод
    qDebug() << "setting first track";
    QString file = currentPlaylist[index].path;
    qDebug() << file;
    for (int i = 0; i < allFiles.size(); i++) {
        if (allFiles[i]->path == file) {
            allFiles.move(i, 0);
            return;
        }
    }
}
