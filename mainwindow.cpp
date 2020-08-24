#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //The code below makes the mainwindow default to screen resolution.
    //The pixel height of the taskbar should be subtracted from the height so there's no overlap.
    //Good enough for now, will implement into subwindows too.
    QScreen *MainScreen = QGuiApplication::primaryScreen();
    QRect MainScreenAvailableGeometry = MainScreen->availableGeometry();
    this->resize(MainScreenAvailableGeometry.width(), MainScreenAvailableGeometry.height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionPreluare_eMAG_triggered()
{
    //We create an MdiChild by using this function:
    EMAGMdiChild * EMAGMdiChildInUse = MainWindow::CreateMdiChild();
    QRect MainScreenAvailableGeometry = QGuiApplication::primaryScreen()->availableGeometry();

    EMAGMdiChildInUse->parentWidget()->resize(MainScreenAvailableGeometry.width(), MainScreenAvailableGeometry.height());
    EMAGMdiChildInUse->parentWidget()->updateGeometry();

    EMAGMdiChildInUse->show();
}

void MainWindow::on_actionAutentificare_eMAG_triggered()
{
    //This function is executed when the user clicks "Autentificare EMAG" from the top menu.
    //It just executes an AuthDialog.
    AuthDialog AuthDialogInUse;
    AuthDialogInUse.setModal(true);
    AuthDialogInUse.exec();
}

EMAGMdiChild * MainWindow::CreateMdiChild()
{
    //This function creates an MdiChildWindow and returns a pointer to it so we have a handle on it.
    EMAGMdiChild * child = new EMAGMdiChild;
    ui->mdiArea->addSubWindow(child);
    return child;
}
