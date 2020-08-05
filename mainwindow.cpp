#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_authdialog.h"
#include <authdialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionPreluare_eMAG_triggered()
{
    EMAGMdiChild * EMAGMdiChildInUse = MainWindow::CreateMdiChild();
    EMAGMdiChildInUse->show();
}

void MainWindow::on_actionAutentificare_eMAG_triggered()
{
    AuthDialog AuthDialogInUse;
    AuthDialogInUse.setModal(true);
    AuthDialogInUse.exec();
}

EMAGMdiChild * MainWindow::CreateMdiChild()
{
    EMAGMdiChild * child = new EMAGMdiChild;
    ui->mdiArea->addSubWindow(child);
    return child;
}
