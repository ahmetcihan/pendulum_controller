#ifndef PASSWORD_H
#define PASSWORD_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class password;
}

class password : public QDialog
{
    Q_OBJECT

public:
    explicit password(QWidget *parent = 0);
    ~password();
signals:
    void emit_text_value(QString val);

private slots:
    void button_handler();

private:
    Ui::password *ui;
};

#endif // PASSWORD_H
