#include "harmonicplaylistgenerator.h"
#include <QList>
#include <QRandomGenerator>
#include <QTime>

//https://github.com/NRec22/harmonic-mixing-generator/blob/master/generator.js
HarmonicPlaylistGenerator::HarmonicPlaylistGenerator()
{

}

// добавляет в плейлист
void HarmonicPlaylistGenerator::storeTrack(QList<Track> &playlist, QString title, int num, char key)
{
    //playlist.append(Track(title, num, key));
}

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

//4A <-> 5A or 4B <-> 5B
bool HarmonicPlaylistGenerator::adjacentNum(int num1, int num2)
{
    if (num1 == num2 - 1 || num1 == num2 + 11)
        return true;
    else if(num1 == num2 + 1 || num1 == num2 - 11)
        return true;
    else
        return false;
}
/*
//4A -> 6A or 4A -> 11A
bool HarmonicPlaylistGenerator::energyNum(int num1, int num2)
{
    if (num1 == num2 - 2|| num1 == num2 + 10)
        return true;
    else if (num1 == num2 - 7 || num1 == num2 + 5)
        return true;
    else
        return false;
}

//4A -> 3B or 4B -> 5A
bool HarmonicPlaylistGenerator::diagMix(int num1, char key1, int num2, char key2)
{
    if(num1 == num2 - 1 || num1 == num2 + 11){
        if(key1 == 'd' && key2 == 'm'){
            return true;
        }
        else{
            return false;
        }
    }
    else if(num1 == num2 + 1 || num1 == num2 - 11){
        if(key1 == 'm' && key2 == 'd'){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

//4A -> 7B or 4B -> 1A
// не пониятно почему это здесь...
bool HarmonicPlaylistGenerator::diagMix2(int num1, char key1, int num2, char key2)
{
    if(num1 == num2 - 3 || num1 == num2 + 9){
        if(key1 == 'm' && key2 == 'd'){
            return true;
        }
        else{
            return false;
        }
    }
    else if(num1 == num2 + 3 || num1 == num2 - 9){
        if(key1 == 'd' && key2 == 'm'){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}*/

//given two songs, returns whether they can be harmonically mixed
bool HarmonicPlaylistGenerator::checkCompatible(int num1, char key1, int num2, char key2)
{
    if(sameNum(num1, num2) == true){
        return true;
    }
    else if(adjacentNum(num1, num2) == true && matchKey(key1, key2) == true){
        return true;
    }
//    else if(energyNum(num1, num2) == true && matchKey(key1, key2) == true){
//        return true;
//    }
//    else if(diagMix(num1, key1, num2, key2) == true){
//        return true;
//    }
//    else if(diagMix2(num1, key1, num2, key2) == true){
//        return true;
//    }
    else{
        return false;
    }
}

//Given a playlist, this method creates a new playlist with matching or compatible songs based on a given key.
//Создает новый плейлист, в котором все песни совместимы с первой в списке.
QList<Track> &HarmonicPlaylistGenerator::listCompatible(QList<Track> playlist)
{
    //QList<Track> given(playlist);
    QList<Track> *newList = new QList<Track>();
    newList->append(playlist.takeFirst());

    for (int i = 0; i < playlist.size(); i++)
        if (checkCompatible(newList->at(0).num, newList->at(0).key, playlist[i].num, playlist[i].key))
            newList->append(playlist[i]);

    return *newList;
}

/*
Given a playlist, this method sorts the playlist prioritizing harmonic transitions first.
Works in a first come, first serve basis.
*/
QList<Track> &HarmonicPlaylistGenerator::harmonicSort(QList<Track> playlist)
{
    // equivalent of:
    //  var sortedPlaylist = [playlist.shift()];
    //QList<Track> *sortedPlaylist;
    //sortedPlaylist->append(playlist[0]);
    //playlist->erase(playlist->begin());

    QList<Track> *unsort = new QList<Track>(playlist);

    QList<Track> *sortedPlaylist = new QList<Track>();
    /*if (random) {
        QRandomGenerator generator(QTime::currentTime().msec());
        generator.bounded(0, unsort->size() - 1);
        int seed = generator.generate();
        sortedPlaylist->append(unsort->takeAt(seed));
    }
    else */sortedPlaylist->append(unsort->takeFirst());

    return harmonicSortHelper(*sortedPlaylist, *unsort);
}

QList<Track> &harmonicSortRandom(QList<Track> playlist)
{
    // с помощью функции listCompatible получаем совместимые треки и берем рандомный

    // СДЕЛАТЬ CONTROLLER для окна с функцией setFirstTrack, cancel, addTrack, setPlaylistSize, сигналом generated и тд и тп
    QList<Track> *result = new QList<Track>();

    return *result;
}

QList<Track> &harmonicSortRandomHelper(QList<Track> &sortlist, QList<Track> &unsortlist)
{
    for(int i = 0; i < unsortlist.size(); i++) {
        if (checkCompatible(sortlist[sortlist.size() - 1].num, sortlist[sortlist.size() - 1].key, unsortlist[i].num, unsortlist[i].key)) {
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

QList<Track> &HarmonicPlaylistGenerator::harmonicSortHelper(QList<Track> &sortlist, QList<Track> &unsortlist)
{
    // можно написать функцию которая будет находить наиболее подходящую песню для данной песни, чтобы было меньше отсеенных треков
    // одним из критериев оценивания будет то, сколько песен после себя она может поставить
    // главный критерий -- если есть песня с такой же тональностью, ставим ее.
    for(int i = 0; i < unsortlist.size(); i++) {
        if (checkCompatible(sortlist[sortlist.size() - 1].num, sortlist[sortlist.size() - 1].key, unsortlist[i].num, unsortlist[i].key)) {
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

// вспомогательная функция
// удаляет из массива первый элемент и возвращает его значение
/*Track HarmonicPlaylistGenerator::shiftArray(QList<Track> &array)
{
    //QList<Track> *sortedPlaylist;
    //sortedPlaylist->append(playlist[0]);
    Track track = array[0];
    array.erase(array.begin());
    return track;
}*/
