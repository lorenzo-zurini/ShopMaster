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


void MainWindow::on_pushButton_clicked()
{
    AuthDialog AuthDialogInUse;
    AuthDialogInUse.setModal(true);
    AuthDialogInUse.exec();
}
