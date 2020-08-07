#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_authdialog.h"
#include <authdialog.h>

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

    //The NetworkManager class is initialised and the signal is connected to our slot function.
    AuthManager = new QNetworkAccessManager();
    connect(AuthManager, &QNetworkAccessManager::finished, this, &MainWindow::on_AuthRequestComplete);

    //The post request is constructed and then executed.
    QUrl ApiUrl;
    ApiUrl.setUrl("https://marketplace.emag.ro/api-3/order/read");


    //The auth data that will be sent in the post request headers is stored in AuthData which we initialise here.
    //The hash that was stored in the AuthDialog class is retrieved from the file.
    QByteArray AuthData;
    QFile CredentialStore(QCoreApplication::applicationDirPath() + "/CredentialStore.bin");
    QString EncryptedString;

    if(CredentialStore.open(QIODevice::ReadOnly | QIODevice::Text))
        {
        EncryptedString = CredentialStore.readAll();
        }

    AuthData.append(EncryptedString);
    AuthData.prepend("Basic ");

    //The request is constructed and the HTTP headers are added:
    QNetworkRequest ApiRequest;
    ApiRequest.setUrl(ApiUrl);
    ApiRequest.setRawHeader("Authorization", AuthData);
    ApiRequest.setRawHeader("Content-Type", "application/json");

    //The data body of the post request is constructed here (doesn´t appear to work yet).
    QByteArray PostData;
    PostData.append("currentPage=1&itemsPerPage=20");

    //The request is made and the data is printed to the debug console.
    AuthManager->post(ApiRequest, PostData);
    qDebug()<< PostData;
}

void MainWindow::on_AuthRequestComplete(QNetworkReply * AuthReply)
{
    //This function is called asynchronously when the QNetworkAccessManager has finished the request.
    //The connection between the QNetworkAccessManager and this function is handled by the Signal/Slot System.
    //We also check if the reply has the "X-User-Id" header which is only present if the credentials are right.
    //If they are right, we launch a mdi subwindow using the EMAGMdiChild class.
    if(!AuthReply->hasRawHeader("X-User-Id"))
    {
        QMessageBox WrongCredentials;
        WrongCredentials.setText("Este necesara autentificarea pe platforma eMAG");
        WrongCredentials.exec();
    }
    else
    {
        qDebug() << AuthReply->readAll();
        //We create an MdiChild by using this function:
        EMAGMdiChild * EMAGMdiChildInUse = MainWindow::CreateMdiChild();
        EMAGMdiChildInUse->show();
    }
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
