#ifndef EMPTYPLAYLISTEXCEPTION_H
#define EMPTYPLAYLISTEXCEPTION_H

#include <QException>
#include <QDebug>
#include <QMessageBox>

class EmptyPlaylistException : public QException
{
public:
    void raise() const override {
        QMessageBox messageBox;
        messageBox.setWindowTitle("Ошибка");
        messageBox.setText("Пожалуйста, выберите файл или папку с файлами для генерации плейлиста");
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
        /*qDebug() << "Невозможно сгенерировать плейлист, если не было выбрано исходных файлов.";
        throw *this;*/
    }
    EmptyPlaylistException *clone() const override { return new EmptyPlaylistException(*this); }
};

#endif // EMPTYPLAYLISTEXCEPTION_H
