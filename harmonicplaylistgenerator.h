#ifndef HARMONICPLAYLISTGENERATOR_H
#define HARMONICPLAYLISTGENERATOR_H

#include "track.h"
#include <QList>

class HarmonicPlaylistGenerator
{
public:
    // Из принятого списка треков генерирует гармонически сочетаемый плейлист
    // и возвращает его. Если второй аргумент равен true, то будут генерироваться
    // уникальные плейлисты.
    static QList<Track*> &harmonicSort(QList<Track*>, bool random = false);

private:
    // Не разрешается создавать экземпляры этого класса.
    HarmonicPlaylistGenerator();

    // Проверяет, совпадают ли символы.
    static bool matchKey(char, char);

    // Проверяет, совпадают ли числа.
    static bool sameNum(int, int);

    // Проверяет, лежат ли рядом числа на круге.
    static bool adjacentNum(int, int);

    // Проверяет, совместимы ли треки.
    static bool checkCompatible(int, char, int, char);

    // Рекурсивная функция, сортирующая треки по тональности.
    static QList<Track*> &harmonicSortHelper(QList<Track*> &, QList<Track*> &);

    // Рекурсивная функция, создающая уникальные плейлисты,
    // где все треки совместимы.
    static QList<Track*> &harmonicSortRandomHelper(QList<Track*> &, QList<Track*> &);

    // Изымает из переданного списка треков случайный трек, совместимый с переданным треком.
    static Track *takeRandomCompatible(QList<Track*> &playlist, Track *track);
};

#endif // HARMONICPLAYLISTGENERATOR_H
