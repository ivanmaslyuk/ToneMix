#include "maincontroller.h"
#include "harmonicplaylistgenerator.h"
#include "tagreader.h"
#include "emptyplaylistexception.h"

#include <QStringList>

MainController::MainController(QObject *parent) : QObject(parent)
{

}

void MainController::generate()
{
    if (canGenerate()) {
        emit generationInProgress();

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

        generationHistory.append(&currentPlaylist);
        emit generated(currentPlaylist);
    }
    else {
        // если файлы еще не были выбраны
        EmptyPlaylistException e;
        e.raise();
    }
}

void MainController::restorePrevious()
{
    currentPlaylist = *(generationHistory.takeLast());
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

    if (allFiles.size() > 0)
        emit canGenerateChanged();
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
