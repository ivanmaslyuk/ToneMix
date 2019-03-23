#include "harmonicplaylistgenerator.h"
#include <QList>
#include <QRandomGenerator>
#include <QTime>

//https://github.com/NRec22/harmonic-mixing-generator/blob/master/generator.js
HarmonicPlaylistGenerator::HarmonicPlaylistGenerator() {}

// check matching
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

// Получая две песни, определяет, сочетаются ли они гармонически.
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

/**
 * Вынимает из плейлиста случайную песню, совместимую с данной.
 */
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


/**
 * Из полученного списка треков составляет гармонический плейлист.
 */
QList<Track*> &HarmonicPlaylistGenerator::harmonicSort(QList<Track*> playlist, bool random)
{
    QList<Track*> *unsort = new QList<Track*>(playlist);

    QList<Track*> *sortedPlaylist = new QList<Track*>();
    sortedPlaylist->append(unsort->takeFirst());

    if (random)
        return harmonicSortRandomHelper(*sortedPlaylist, *unsort);
    else
        return harmonicSortHelper(*sortedPlaylist, *unsort);
}

QList<Track*> &HarmonicPlaylistGenerator::harmonicSortHelper(QList<Track*> &sortlist, QList<Track*> &unsortlist)
{
    // можно написать функцию которая будет находить наиболее подходящую песню для данной песни, чтобы было меньше отсеенных треков
    // одним из критериев оценивания будет то, сколько песен после себя она может поставить
    // главный критерий -- если есть песня с такой же тональностью, ставим ее.
    for(int i = 0; i < unsortlist.size(); i++) {
        if (checkCompatible(sortlist[sortlist.size() - 1]->num, sortlist[sortlist.size() - 1]->key, unsortlist[i]->num, unsortlist[i]->key)) {
            sortlist.append(unsortlist.takeAt(i));
            harmonicSortHelper(sortlist, unsortlist);
        }
        else if (i == unsortlist.size() - 1) {
            //sortlist.append(unsortlist.takeFirst()); // добавляет не поддающиеся сортировке песни в конец списка
            unsortlist.removeFirst(); // удаляет не поддающиеся сортировке песни из плейлиста
            harmonicSortHelper(sortlist, unsortlist);
        }
    }
    return sortlist;
}

QList<Track*> &HarmonicPlaylistGenerator::harmonicSortRandomHelper(QList<Track*> &sortlist, QList<Track*> &unsortlist)
{
    if (unsortlist.size() != 0) {
        // Берем последний трек в сортированном списке, чтобы подобрать к нему совместимый.
        Track *last = sortlist.last();

        // Забираем из неотсортированного списка случайную совместимую песню.
        Track *randomCompatible = takeRandomCompatible(unsortlist, last);

        // Если удалось найти совместимую композицию.
        if (randomCompatible != nullptr) {
            sortlist.append(randomCompatible);
            harmonicSortRandomHelper(sortlist, unsortlist);
        }
        // Если не удалось найти совместимую композицию.
        else {
            return sortlist;
        }
    }

    return sortlist;
}
