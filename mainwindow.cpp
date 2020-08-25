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

    // WE CREATE THE DATA DIR AS IT IS NOT CREATED AUTOMATICALLY.
    MainWindow::DataDir.setPath(QCoreApplication::applicationDirPath() + "/Data");
    MainWindow::DataDir.mkpath(MainWindow::DataDir.path());

    // WE ADD THE DATABASE.
    MainWindow::DataBase.setDatabaseName(QCoreApplication::applicationDirPath() + "/Data/DataBase.db");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAutentificare_eMAG_triggered()
{
    //This function is executed when the user clicks "Autentificare EMAG" from the top menu.
    //It just executes an AuthDialog.
    AuthDialog AuthDialogInUse;
    AuthDialogInUse.setModal(true);
    AuthDialogInUse.exec();
}

void MainWindow::on_actionPreluare_eMAG_triggered()
{
    //We create an MdiChild by using this function:
    EMAGMdiChild * EMAGMdiChildInUse = MainWindow::CreateEMAGMdiChild();
    EMAGMdiChildInUse->PassDatabase(MainWindow::DataBase);
    EMAGMdiChildInUse->parentWidget()->resize(ui->mdiArea->size().width(), ui->mdiArea->size().height());
    EMAGMdiChildInUse->show();
}

EMAGMdiChild * MainWindow::CreateEMAGMdiChild()
{
    //This function creates an MdiChildWindow and returns a pointer to it so we have a handle on it.
    EMAGMdiChild * child = new EMAGMdiChild;
    ui->mdiArea->addSubWindow(child);
    connect(child, &EMAGMdiChild::OrderEdit, this, &MainWindow::OrderEdit);
    return child;
}

void MainWindow::OrderEdit()
{
    // We create an mdi child and pass the path of the order file to be edited via a public method.
    OrderEditForm * OrderEditFormInUse = MainWindow::CreateOrderEditForm();
    // PASS ORDER ID VIA METHOD
    OrderEditFormInUse->parentWidget()->setFixedSize(OrderEditFormInUse->parentWidget()->size());
    OrderEditFormInUse->show();
}

OrderEditForm * MainWindow::CreateOrderEditForm()
{
    //This function creates an OrderEditForm and returns the pointer.
    OrderEditForm * child = new OrderEditForm;
    ui->mdiArea->addSubWindow(child);
    return child;
}
