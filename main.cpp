#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qSetMessagePattern("[ %{file}: %{line} ] %{message}");  //change the way of Debugger display
    MainWindow w;
    w.show();

    return a.exec();
}
