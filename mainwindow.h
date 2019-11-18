#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QLabel>
#include<QComboBox>
#include<QSpinBox>
#include<QToolButton>
#include<QScrollArea>
#include"drawwidget.h"
#include"dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QAction *drawPenAction;
    void createFile();
    void openFile();
    bool saveFile();
    bool saveFileAs();
    signals:

public slots:
    void createfinal(Dialog *a);
    void changecolor();
    void SetColor(QColor);
    void changeBrushcolor();
    void SetBrushColor(QColor);
    void openfile();
    void savefile();
    void createfile();
    void solidline();
    void dashline();
    void dotline();
    void dashdotline();
    void dashdotdotline();
    void customdashline();
private slots:


private:
    Ui::MainWindow *ui;
    DrawWidget *area;
    QScrollArea *scrollArea;
     QLineEdit *qli;
     QLineEdit *qli_2;
};

#endif // MAINWINDOW_H
