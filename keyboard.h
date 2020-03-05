#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QDialog>
#include <QPushButton>
#include <QSignalMapper>
#include <QSpinBox>
#include <QLineEdit>
#include <QDebug>

#define NUMERIC     0
#define ALPHABET    1

#define QStringLiteral QString::fromUtf8

namespace Ui {
class Keyboard;
}

class Keyboard : public QDialog
{
    Q_OBJECT

public:
    explicit Keyboard(QWidget *parent = 0);
    ~Keyboard();
    void setSpinBox(QString);
    void setEchoMode(QLineEdit::EchoMode m);
    QString getKeyboardText() { return keyboardText; }
    Ui::Keyboard *ui;

private:
    unsigned char keyboard_mode;

signals:
    void setKeyboardText();

private slots:
    void clicked(const QString &text);
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();
    void on_pushButton_clear_clicked();
    void alphanumeric_mode_handler(void);
    void backspace_handler(void);

private:
    QSignalMapper *signalMapper;
    QList <QString > textList;
    QList <QPushButton *> buttonList;
    QString keyboardText;
    void setButtonAndTextList();
};

#endif // KEYBOARD_H
