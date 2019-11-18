#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getLineEditText()
{
    return ui->lineEdit->text();
}
QString Dialog::getLineEdit_2Text()
{
    return ui->lineEdit_2->text();
}


void Dialog::setLineEditText(const QString &text)
{
   ui->lineEdit->setText(text);
}

void Dialog::setLineEdit_2Text(const QString &text)
{
    ui->lineEdit_2->setText(text);
}

void Dialog::on_buttonBox_accepted()
{
    emit ok_down(this);
}
