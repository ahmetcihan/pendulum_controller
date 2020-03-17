#include "keyboard.h"
#include "ui_keyboard.h"


Keyboard::Keyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Keyboard)
{
    ui->setupUi(this);
    //if (cpu_is_arm())
        //setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("KEYBOARD");
    setButtonAndTextList();

    signalMapper = new QSignalMapper(this);
    for (int i = 0; i < buttonList.count(); ++i) {
        QPushButton *button = buttonList[i];
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(button, textList[i]);
    }
    connect(signalMapper, SIGNAL(mapped(const QString &)),this, SLOT(clicked(const QString &)));

    keyboard_mode = NUMERIC;
    ui->groupBox_alphabet->hide();
    ui->pushButton_mode->setText(QStringLiteral("ABC"));
    connect(ui->pushButton_mode, SIGNAL(clicked()),this, SLOT(alphanumeric_mode_handler()));
    connect(ui->pushButton_backspace, SIGNAL(clicked()),this, SLOT(backspace_handler()));
}
void Keyboard::backspace_handler(){
    ui->lineEdit->backspace();
}
void Keyboard::alphanumeric_mode_handler(){
    if(keyboard_mode == NUMERIC){
        keyboard_mode = ALPHABET;
        ui->pushButton_mode->setText(QStringLiteral("123"));
        ui->groupBox_alphabet->show();
        ui->groupBox_numeric->hide();
    }
    else{
        keyboard_mode = NUMERIC;
        ui->pushButton_mode->setText(QStringLiteral("ABC"));
        ui->groupBox_alphabet->hide();
        ui->groupBox_numeric->show();
    }
}
Keyboard::~Keyboard()
{
    delete ui;
}
void Keyboard::clicked(const QString &text)
{
    QLineEdit *lEdit = ui->lineEdit;
    lEdit->setText(lEdit->text() + text);
}
void Keyboard::setSpinBox(QString text)
{
    ui->lineEdit->setText(text);
    keyboardText = text;
}
void Keyboard::setEchoMode(QLineEdit::EchoMode m)
{
    ui->lineEdit->setEchoMode(m);
}
void Keyboard::on_pushButton_cancel_clicked()
{
    close();
}
void Keyboard::setButtonAndTextList()
{
    buttonList.append(ui->pushButton_key_0);
    buttonList.append(ui->pushButton_key_1);
    buttonList.append(ui->pushButton_key_2);
    buttonList.append(ui->pushButton_key_3);
    buttonList.append(ui->pushButton_key_4);
    buttonList.append(ui->pushButton_key_5);
    buttonList.append(ui->pushButton_key_6);
    buttonList.append(ui->pushButton_key_7);
    buttonList.append(ui->pushButton_key_8);
    buttonList.append(ui->pushButton_key_9);
    buttonList.append(ui->pushButton_key_dash);
    buttonList.append(ui->pushButton_key_point);
    buttonList.append(ui->pushButton_key_A);
    buttonList.append(ui->pushButton_key_B);
    buttonList.append(ui->pushButton_key_C);
    buttonList.append(ui->pushButton_key_D);
    buttonList.append(ui->pushButton_key_E);
    buttonList.append(ui->pushButton_key_F);
    buttonList.append(ui->pushButton_key_G);
    buttonList.append(ui->pushButton_key_H);
    buttonList.append(ui->pushButton_key_I);
    buttonList.append(ui->pushButton_key_J);
    buttonList.append(ui->pushButton_key_K);
    buttonList.append(ui->pushButton_key_L);
    buttonList.append(ui->pushButton_key_M);
    buttonList.append(ui->pushButton_key_N);
    buttonList.append(ui->pushButton_key_O);
    buttonList.append(ui->pushButton_key_P);
    buttonList.append(ui->pushButton_key_Q);
    buttonList.append(ui->pushButton_key_R);
    buttonList.append(ui->pushButton_key_S);
    buttonList.append(ui->pushButton_key_T);
    buttonList.append(ui->pushButton_key_U);
    buttonList.append(ui->pushButton_key_V);
    buttonList.append(ui->pushButton_key_W);
    buttonList.append(ui->pushButton_key_X);
    buttonList.append(ui->pushButton_key_Y);
    buttonList.append(ui->pushButton_key_Z);
    buttonList.append(ui->pushButton_key_point_2);
    buttonList.append(ui->pushButton_key_comma);
    buttonList.append(ui->pushButton_key_colon);
    buttonList.append(ui->pushButton_key_space);
    buttonList.append(ui->pushButton_key_percent);

    textList.append(QStringLiteral("0"));
    textList.append(QStringLiteral("1"));
    textList.append(QStringLiteral("2"));
    textList.append(QStringLiteral("3"));
    textList.append(QStringLiteral("4"));
    textList.append(QStringLiteral("5"));
    textList.append(QStringLiteral("6"));
    textList.append(QStringLiteral("7"));
    textList.append(QStringLiteral("8"));
    textList.append(QStringLiteral("9"));
    textList.append(QStringLiteral("-"));
    textList.append(QStringLiteral("."));
    textList.append(QStringLiteral("A"));
    textList.append(QStringLiteral("B"));
    textList.append(QStringLiteral("C"));
    textList.append(QStringLiteral("D"));
    textList.append(QStringLiteral("E"));
    textList.append(QStringLiteral("F"));
    textList.append(QStringLiteral("G"));
    textList.append(QStringLiteral("H"));
    textList.append(QStringLiteral("I"));
    textList.append(QStringLiteral("J"));
    textList.append(QStringLiteral("K"));
    textList.append(QStringLiteral("L"));
    textList.append(QStringLiteral("M"));
    textList.append(QStringLiteral("N"));
    textList.append(QStringLiteral("O"));
    textList.append(QStringLiteral("P"));
    textList.append(QStringLiteral("Q"));
    textList.append(QStringLiteral("R"));
    textList.append(QStringLiteral("S"));
    textList.append(QStringLiteral("T"));
    textList.append(QStringLiteral("U"));
    textList.append(QStringLiteral("V"));
    textList.append(QStringLiteral("W"));
    textList.append(QStringLiteral("X"));
    textList.append(QStringLiteral("Y"));
    textList.append(QStringLiteral("Z"));
    textList.append(QStringLiteral("."));
    textList.append(QStringLiteral(","));
    textList.append(QStringLiteral(":"));
    textList.append(QStringLiteral(" "));
    textList.append(QStringLiteral("%"));

    keyboardText = "";
}
void Keyboard::on_pushButton_ok_clicked()
{
    keyboardText = ui->lineEdit->text();
    emit setKeyboardText();
    ui->lineEdit->clear();
    close();
}
void Keyboard::on_pushButton_clear_clicked()
{
    ui->lineEdit->clear();
}

