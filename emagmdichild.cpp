#include "emagmdichild.h"
#include "ui_emagmdichild.h"

EMAGMdiChild::EMAGMdiChild(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EMAGMdiChild)
{
    EMAGOrdersDirectory.setPath(QCoreApplication::applicationDirPath() + "/Orders/eMAG");
    EMAGOrdersDirectory.mkpath(EMAGOrdersDirectory.path());
    EMAGMdiChild::GetEMAGOrders();
    ui->setupUi(this);
}

EMAGMdiChild::~EMAGMdiChild()
{
    delete ui;
}

void EMAGMdiChild::GetEMAGOrders()
{
    //The NetworkManager class is initialised and the signal is connected to our slot function.
    AuthManager = new QNetworkAccessManager();
    connect(AuthManager, &QNetworkAccessManager::finished, this, &EMAGMdiChild::on_AuthRequestComplete);

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

void EMAGMdiChild::on_AuthRequestComplete(QNetworkReply * AuthReply)
{
    //This function is called asynchronously when the QNetworkAccessManager has finished the request.
    //The connection between the QNetworkAccessManager and this function is handled by the Signal/Slot System.
    //We also check if the reply has the "X-User-Id" header which is only present if the credentials are right.
    //If they are right, we launch a mdi subwindow using the EMAGMdiChild class.
    if(!AuthReply->hasRawHeader("X-User-Id"))
    {
        QMessageBox WrongCredentials;
        WrongCredentials.setText("Eroare de autentificare eMAG.");
        WrongCredentials.exec();
    }
    else
    {
        QJsonArray OrdersArray = QJsonDocument::fromJson(AuthReply->readAll()).object()["results"].toArray();
        for (int i = 0; i < OrdersArray.size(); i++)
        {
            QFile * OrderFile = new QFile;
            OrderFile->setFileName(EMAGOrdersDirectory.path() + "/" + OrdersArray.at(i).toObject()["id"].toString() + ".order");
            if(OrderFile->open(QIODevice::WriteOnly | QIODevice::Text))
                {
                OrderFile->write("Contact" + OrdersArray.at(i).toObject()["customer"].toObject()["shipping_contact"].toString().toUtf8());
                OrderFile->close();
                }
            qDebug().noquote().nospace() << "DATE LIVRARE:";
            qDebug().noquote().nospace() << "Nume: " << OrdersArray.at(i).toObject()["customer"].toObject()["shipping_contact"].toString();
            qDebug().noquote().nospace() << "Telefon: " << OrdersArray.at(i).toObject()["customer"].toObject()["shipping_phone"].toString();
            qDebug().noquote().nospace() << "Nota-Observatii: Comanda nr. " << OrdersArray.at(i).toObject()["id"].toInt() << " din data " << OrdersArray.at(i).toObject()["date"].toString();
            qDebug().noquote().nospace() << "Judet: " << OrdersArray.at(i).toObject()["customer"].toObject()["shipping_suburb"].toString();
            qDebug().noquote().nospace() << "Localitate: " <<OrdersArray.at(i).toObject()["customer"].toObject()["shipping_city"].toString();
            qDebug().noquote().nospace() << "Adresa: " << OrdersArray.at(i).toObject()["customer"].toObject()["shipping_street"].toString()<< "\n";
            qDebug().noquote().nospace() << "DATE FACTURARE:";
            qDebug().noquote().nospace() << "Nume: " << OrdersArray.at(i).toObject()["customer"].toObject()["billing_name"].toString();
            qDebug().noquote().nospace() << "Judet: " << OrdersArray.at(i).toObject()["customer"].toObject()["billing_suburb"].toString();
            qDebug().noquote().nospace() << "Localitate, Adresa: " << OrdersArray.at(i).toObject()["customer"].toObject()["billing_city"].toString() << ", " << OrdersArray.at(i).toObject()["customer"].toObject()["billing_street"].toString();
            qDebug().noquote().nospace() << "Tara: " << OrdersArray.at(i).toObject()["customer"].toObject()["billing_country"].toString() << "\n" << "\n";
        }
    }
}
