#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QMainWindow>
#include<QLabel>
#include<QComboBox>
#include<QSpinBox>
#include<QToolButton>
#include<QScrollArea>
#include<QObject>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    friend class MainWindow;
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void setLineEditText(const QString &text);
    void setLineEdit_2Text(const QString &text);
    QString getLineEditText();
    QString getLineEdit_2Text();
signals:
    void ok_down(Dialog *);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
