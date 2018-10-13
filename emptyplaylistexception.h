#ifndef EMPTYPLAYLISTEXCEPTION_H
#define EMPTYPLAYLISTEXCEPTION_H

#include <QException>
#include <QDebug>

class EmptyPlaylistException : public QException
{
public:
    void raise() const override {
        qDebug() << "Невозможно сгенерировать плейлист, если не было выбрано исходных файлов.";
        throw *this;
    }
    EmptyPlaylistException *clone() const override { return new EmptyPlaylistException(*this); }
};

#endif // EMPTYPLAYLISTEXCEPTION_H
