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

    //The NetworkManager class is initialised and the signal is connected to our slot function.
    AuthManager = new QNetworkAccessManager();
    connect(AuthManager, &QNetworkAccessManager::finished, this, &MainWindow::on_AuthRequestComplete);

    //The post request is constructed and then executed.
    QUrl ApiUrl;
    ApiUrl.setUrl("https://marketplace.emag.ro/api-3/order/read");
    //ApiUrl.setUrl("https://marketplace.emag.ro/api-3");

    QByteArray AuthData;

    QFile CredentialStore(QCoreApplication::applicationDirPath() + "/CredentialStore.bin");
    QString EncryptedString;

    if(CredentialStore.open(QIODevice::ReadOnly | QIODevice::Text))
        {
        EncryptedString = CredentialStore.readAll();
        }

    AuthData.append(EncryptedString);
    AuthData.prepend("Basic ");

    QNetworkRequest ApiRequest;
    ApiRequest.setUrl(ApiUrl);
    ApiRequest.setRawHeader("Authorization", AuthData);
    ApiRequest.setRawHeader("Content-Type", "application/json");

    QByteArray PostData;
    PostData.append("currentPage=1&itemsPerPage=20");
    AuthManager->post(ApiRequest, PostData);
    qDebug()<< PostData;
}

void MainWindow::on_AuthRequestComplete(QNetworkReply * AuthReply)
{
    if(!AuthReply->hasRawHeader("X-User-Id"))
    {
        QMessageBox WrongCredentials;
        WrongCredentials.setText("Este necesara autentificarea pe platforma eMAG");
        WrongCredentials.exec();
    }
    else
    {
        qDebug() << AuthReply->readAll();
        EMAGMdiChild * EMAGMdiChildInUse = MainWindow::CreateMdiChild();
        EMAGMdiChildInUse->show();
    }
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
