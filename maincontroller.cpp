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

        generationHistory.append(QList<Track*>(currentPlaylist));

        // составляем список неисключенных из генерации треков для их передачи генератору
        QList<Track*> included;
        foreach (Track *track, allFiles) {
            if (!track->excluded && track->isAnalyzed && track->notation != UnknownToneNotation)
                included.append(track);
        }

        currentPlaylist = HarmonicPlaylistGenerator::harmonicSort(included);

        // обрезаем плейлист чтобы его размер не привышал playlistSize
        while (currentPlaylist.size() > playlistSize)
            currentPlaylist.removeLast();

        sendCurrentPlaylist();

        if (generationHistory.size() > 0) emit canGoBackChanged(true);
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
        currentPlaylist = generationHistory.takeLast();
        sendCurrentPlaylist();
        if (generationHistory.size() == 0) emit canGoBackChanged(false);
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

    if (allFiles.size() > 0) {
        emit canGenerateChanged();
        foreach (Track *tr, allFiles)
            currentPlaylist.append(tr);
        sendCurrentPlaylist();
    }

}

void MainController::addSingleTrack(QString path)
{
    // Считываем тональность и добавляем в список всех файлов.
    TagReader tagReader = TagReader(QStringList() << path);
    QList<Track*> file = tagReader.read();
    allFiles.append(file);

    // Сразу же добавляем в текущий плейлист и отправляем его представлению.
    currentPlaylist.append(file[0]);
    sendCurrentPlaylist();

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
    currentPlaylist[trackIndex]->excluded = !value;
}

void MainController::setFirstTrack(int index)
{
    qDebug() << "setting first track";
    QString file = currentPlaylist[index]->path;
    qDebug() << file;
    for (int i = 0; i < allFiles.size(); i++) {
        if (allFiles[i]->path == file) {
            allFiles.move(i, 0);
            return;
        }
    }
}

void MainController::checkRepeated() {
    // Сначала сбрасываем флаг у всех треков.
    foreach (Track *t, allFiles) {
        t->repeatedInPlaylist = false;
    }

    // Копируем текущий плелист.
    QList<Track*> list(currentPlaylist);

    // Проверяем на повторения.
    foreach(Track *track, currentPlaylist) {
        list.removeFirst();
        foreach(Track* tr, list) {
            if (*track == *tr) {
                tr->repeatedInPlaylist = true;
                track->repeatedInPlaylist = true;
                qDebug() << "Трек" << track->path << "повторяется больше одного раза. ";
                qDebug() << tr->title << "|||||" << track->title;
            }
        }
    }
    qDebug() << "Проверка на повторяющиуеся треки завершена.";
}

void MainController::sendCurrentPlaylist()
{
    // проверяем какие треки повторяются перед передачей
    checkRepeated();

    QList<Track> playlist;
    foreach(Track *t, currentPlaylist) {
        playlist.append(*t);
    }

    emit generated(playlist);
}
