#include "password.h"
#include "ui_password.h"

password::password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::password)
{
    ui->setupUi(this);
    this->setWindowTitle("PASSWORD");
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(button_handler()));
}
void password::button_handler(){
    emit emit_text_value(ui->lineEdit_pass->text());
    close();
}
password::~password()
{
    delete ui;
}
