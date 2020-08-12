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
            //THIS NEEDS TO BE CHANGED FOR MODULARITY
            //WE NEED TO COME UP WITH AN UNIVERSAL FORMAT FOR ORDERS
            //AND USE THAT WHEN SAVING ORDERS
            //AND PASS THE ORDER TO THE BACKEND MODULE (CUSTOM FOR EACH CLIENT)
            QSettings* OrderFile = new QSettings(EMAGOrdersDirectory.path() + "/" + QString::number(OrdersArray.at(i).toObject()["id"].toInt()) + ".order", QSettings::IniFormat);
            OrderFile->beginGroup("ORDER_DATA");
            OrderFile->setValue("ID", QString::number(OrdersArray.at(i).toObject()["id"].toInt()));
            OrderFile->setValue("DATE", OrdersArray.at(i).toObject()["date"].toString());
            OrderFile->endGroup();
            OrderFile->beginGroup("DELIVERY_DATA");
            OrderFile->setValue("NAME", OrdersArray.at(i).toObject()["customer"].toObject()["shipping_contact"].toString());
            OrderFile->setValue("PHONE", OrdersArray.at(i).toObject()["customer"].toObject()["shipping_phone"].toString());
            OrderFile->setValue("FLI-NOTA-OBSERVATII", "Comanda nr. " + QString::number(OrdersArray.at(i).toObject()["id"].toInt()) + " din data " + OrdersArray.at(i).toObject()["date"].toString());
            OrderFile->setValue("JUDET", OrdersArray.at(i).toObject()["customer"].toObject()["shipping_suburb"].toString());
            OrderFile->setValue("LOCALITATE", OrdersArray.at(i).toObject()["customer"].toObject()["shipping_city"].toString());
            OrderFile->setValue("ADRESA", OrdersArray.at(i).toObject()["customer"].toObject()["shipping_street"].toString());
            OrderFile->endGroup();
            OrderFile->beginGroup("BILLING_DATA");
            //AICI TREBUIE IMPLEMENTATA SI O OPTIUNE PENTRU PERSOANE JURIDICE, CU COD CIF SI NR. REG. COM.
            OrderFile->setValue("NUME", OrdersArray.at(i).toObject()["customer"].toObject()["billing_name"].toString());
            OrderFile->setValue("JUDET", OrdersArray.at(i).toObject()["customer"].toObject()["billing_suburb"].toString());
            OrderFile->setValue("FLI-LOCALITATE-ADRESA", OrdersArray.at(i).toObject()["customer"].toObject()["billing_city"].toString() + ", " + OrdersArray.at(i).toObject()["customer"].toObject()["billing_street"].toString());
            OrderFile->setValue("TARA", OrdersArray.at(i).toObject()["customer"].toObject()["billing_country"].toString());
            OrderFile->endGroup();
            for (int j=0; j < OrdersArray.at(i).toObject()["products"].toArray().size(); j++)
            {
                OrderFile->beginGroup("PRODUCT_" + QString::number(j + 1));
                OrderFile->setValue("PRODUCT_ID", OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["part_number"].toString());
                OrderFile->setValue("SALE_PRICE", OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["sale_price"].toString());
                OrderFile->setValue("QUANTITY", QString::number(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["quantity"].toInt()));
                OrderFile->endGroup();
            }
            OrderFile->sync();
            //REMOVE THIS DATA FROM MEMORY SOMEHOW SO AS TO NOT USE TOO MUCH MEMORY
        }
    }
}
