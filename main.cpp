#include "mainwindow.h"
#include <QThread>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //ShowMaximized makes the window launch maximized. This is not optimal behaviour.
    //This applies to the child windows as well. The windows should launch at scren resolution.
    //However, they should not be maximised.
    //In the case of the child windows, they should occupy the full size of the MdiArea when launched.
    w.showMaximized();
    return a.exec();
}
