#include "harmonicplaylistgenerator.h"
#include <QList>
#include <QRandomGenerator>
#include <QTime>

HarmonicPlaylistGenerator::HarmonicPlaylistGenerator() {}

bool HarmonicPlaylistGenerator::matchKey(char key1, char key2)
{
    return key1 == key2;
}

// 4A <-> 4A
bool HarmonicPlaylistGenerator::sameNum(int num1, int num2)
{
    return num1 == num2;
}

//4A <-> 5A или 4B <-> 5B
bool HarmonicPlaylistGenerator::adjacentNum(int num1, int num2)
{
    if (num1 == num2 - 1 || num1 == num2 + 11)
        return true;
    else if(num1 == num2 + 1 || num1 == num2 - 11)
        return true;
    else
        return false;
}

bool HarmonicPlaylistGenerator::checkCompatible(int num1, char key1, int num2, char key2)
{
    // Если число одно и то же (4А <-> 4A, 4А <-> 4B)
    if(num1 == num2){
        return true;
    }
    // Если близлижащая тональность на одном и том же ладу.
    else if(adjacentNum(num1, num2) && key1 == key2){
        return true;
    }
    else{
        return false;
    }
}

Track* HarmonicPlaylistGenerator::takeRandomCompatible(QList<Track*> &tracklist, Track *track)
{
    // Получаем полный список совместимых песен.
    QList<Track*> compatible;
    for (int i = 0; i < tracklist.size(); i++)
        if (checkCompatible(track->num, track->key, tracklist[i]->num, tracklist[i]->key))
            compatible.append(tracklist.at(i));

    // Если нет совместимых песен, возвращаем nullptr.
    if (compatible.size() == 0) return nullptr;

    // Получаем случайный индекс.
    QRandomGenerator generator(QTime::currentTime().msec());
    int index = generator.bounded(0, compatible.size() - 1);

    // Удаляем из списка tracklist выбранный трек.
    tracklist.removeOne(compatible[index]);

    // Возвращаем песню из списка совместимых по полученному
    // случайному индексу.
    return compatible[index];
}

QList<Track*> &HarmonicPlaylistGenerator::harmonicSort(QList<Track*> playlist)
{
    QList<Track*> *unsort = new QList<Track*>(playlist);

    QList<Track*> *sortedPlaylist = new QList<Track*>();
    sortedPlaylist->append(unsort->takeFirst());

    return harmonicSortHelper(*sortedPlaylist, *unsort);
}

QList<Track*> &HarmonicPlaylistGenerator::harmonicSortHelper(QList<Track*> &sortlist, QList<Track*> &unsortlist)
{
    if (unsortlist.size() != 0) {
        // Берем последний трек в сортированном списке, чтобы подобрать к нему совместимый.
        Track *last = sortlist.last();

        // Забираем из неотсортированного списка случайную совместимую песню.
        Track *randomCompatible = takeRandomCompatible(unsortlist, last);

        // Если удалось найти совместимую композицию.
        if (randomCompatible != nullptr) {
            sortlist.append(randomCompatible);
            harmonicSortHelper(sortlist, unsortlist);
        }
        // Если не удалось найти совместимую композицию.
        else {
            return sortlist;
        }
    }

    return sortlist;
}
