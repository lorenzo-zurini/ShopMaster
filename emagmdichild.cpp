#include "emagmdichild.h"
#include "ui_emagmdichild.h"

EMAGMdiChild::EMAGMdiChild(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EMAGMdiChild)
{
    //WE SET THE PATH OF THE ORDERS DIRECTORY ASS A CLASS VARIABLE AS IT IS USED BY MANY THINGS
    EMAGOrdersDirectory.setPath(QCoreApplication::applicationDirPath() + "/Orders/eMAG");

    //WE MAKE SURE TO CREATE THE DIRECTORY OR ELSE THOSE MANY THINGS WOULD FAIL
    EMAGOrdersDirectory.mkpath(EMAGOrdersDirectory.path());

    //WE LINK THE END OF THE PROCESSING OF THE NETWORK REQUEST INTO FILES TO THE POPULATION OF THE TABLE
    //THIS WILL BE CHANGED AND THE ACTUAL ORDER GETTING AND THE TABLE VIEW WILL BE SEPARATED
    connect(this, &EMAGMdiChild::OrderGetComplete, this, &EMAGMdiChild::PopulateOrderViewTable);

    //WE SET THE CURRENT DATE AS A CLASS VARIABLE AS IT IS INTEGRAL TO THE DIRECTORY STRUCTURE
    EMAGMdiChild::CurrentDate = QDate::currentDate();

    //WE CALL THE GER ORDERS FUNCTION HERE FOR NOW
    //IT SHOULD BE MOVED OUT OF THE CONSTRUCTOR AND ONTO A BUTTON ONCE IT IS SEPARATED FROM THE TABLE VIEW
    EMAGMdiChild::GetEMAGOrders();

    //DO NOT CALL ANYTHING USING ui-> BEFORE THIS; IT WILL CRASH THE PROGRAM
    //I DID THIS AND SPENT AN HOUR TRYING TO FIGURE IT OUT
    ui->setupUi(this);

    //WE INITIALIZE THE WINDOW BY SETTING THE DATE OF THE DATE WIDGET TO THE CURRENT ONE
    ui->OrderDateView->setDate(EMAGMdiChild::CurrentDate);

    //WE SET THE RESIZE POLICY FOR THE TABLES
    ui->OrdersView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->OrderDetailsView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

EMAGMdiChild::~EMAGMdiChild()
{
    delete ui;
}

void EMAGMdiChild::PassDatabase(QSqlDatabase DataBase)
{
    EMAGMdiChild::DataBase = DataBase;
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
    PostData.append("currentPage=1&itemsPerPage=10");

    //The request is made and the data is printed to the debug console.
    AuthManager->post(ApiRequest, PostData);
    qDebug() << PostData;
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
        EMAGMdiChild::DataBase.open();
        QSqlQuery DBQuery;
        QString QueryData;
        QueryData.append("CREATE TABLE IF NOT EXISTS EMAG_ORDERS");
        QueryData.append(" (");
        QueryData.append("id INTEGER UNIQUE PRIMARY KEY, ");
        QueryData.append("date DATETIME, ");
        QueryData.append("status INTEGER, ");
        QueryData.append("internal_status INTEGER, ");
        QueryData.append("type INTEGER, ");
        QueryData.append("cancellation_request INTEGER, ");
        QueryData.append("cancellation_reason INTEGER, ");
        QueryData.append("is_storno INTEGER, ");
        QueryData.append("cashed_co INTEGER, ");
        QueryData.append("cashed_cod INTEGER, ");
        QueryData.append("delivery_mode VARCHAR(10), ");
        QueryData.append("detailed_payment_method TEXT, ");
        QueryData.append("emag_club INTEGER, ");
        QueryData.append("has_editable_products INTEGER, ");
        QueryData.append("is_complete INTEGER, ");
        QueryData.append("maximum_date_for_shipment TEXT, ");
        QueryData.append("observation TEXT, ");
        QueryData.append("parent_id INTEGER, ");
        QueryData.append("payment_mode TEXT, ");
        QueryData.append("payment_mode_id INTEGER, ");
        QueryData.append("payment_status INTEGER, ");
        QueryData.append("refund_status INTEGER, ");
        QueryData.append("refunded_amount INTEGER, ");
        QueryData.append("shipping_tax REAL, ");
        QueryData.append("vendor_name TEXT");
        QueryData.append(")");
        DBQuery.exec(QueryData);
        QueryData.clear();

        for (int i = 0; i < OrdersArray.size(); i++)
        {
            QueryData.append("INSERT INTO EMAG_ORDERS");
            QueryData.append(" (");
            QueryData.append("id, ");
            QueryData.append("date, ");
            QueryData.append("status, ");
            QueryData.append("internal_status, ");
            QueryData.append("type, ");
            QueryData.append("cancellation_request, ");
            QueryData.append("cancellation_reason, ");
            QueryData.append("is_storno, ");
            QueryData.append("cashed_co, ");
            QueryData.append("cashed_cod, ");
            QueryData.append("delivery_mode, ");
            QueryData.append("detailed_payment_method, ");
            QueryData.append("emag_club, ");
            QueryData.append("has_editable_products, ");
            QueryData.append("is_complete, ");
            QueryData.append("maximum_date_for_shipment, ");
            QueryData.append("observation, ");
            QueryData.append("parent_id, ");
            QueryData.append("payment_mode, ");
            QueryData.append("payment_mode_id, ");
            QueryData.append("payment_status, ");
            QueryData.append("refund_status, ");
            QueryData.append("refunded_amount, ");
            QueryData.append("shipping_tax, ");
            QueryData.append("vendor_name");
            QueryData.append(") ");
            QueryData.append("VALUES");
            QueryData.append(" (");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["id"].toInt()) + ", ");
            QueryData.append(OrdersArray.at(i).toObject()["date"].toString() + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["status"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["status"].toInt()) + ", "); // CHANGE THIS
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["type"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["cancellation_request"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["cancellation_reason"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["is_storno"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["cashed_co"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["cashed_cod"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["delivery_mode"].toInt()) + ", ");
            QueryData.append(OrdersArray.at(i).toObject()["detailed_payment_method"].toString() + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["emag_club"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["has_editable_products"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["is_complete"].toInt()) + ", ");
            QueryData.append(OrdersArray.at(i).toObject()["maximum_date_for_shipment"].toString() + ", ");
            QueryData.append(OrdersArray.at(i).toObject()["observation"].toString() + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["parent_id"].toInt()) + ", ");
            QueryData.append(OrdersArray.at(i).toObject()["payment_mode"].toString() + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["payment_mode_id"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["payment_status"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["refund_status"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["refunded_amount"].toInt()) + ", ");
            QueryData.append(QString::number(OrdersArray.at(i).toObject()["shipping_tax"].toDouble()) + ", ");
            QueryData.append(OrdersArray.at(i).toObject()["vendor_name"].toString());
            QueryData.append( + ")");
            DBQuery.exec(QueryData);
            QueryData.clear();
        }

        DBQuery.exec("CREATE TABLE IF NOT EXISTS ORDERED_PRODUCTS (parent_order_id INTEGER UNIQUE PRIMARY KEY)");
        DBQuery.exec("CREATE TABLE IF NOT EXISTS CUSTOMERS (parent_order_id INTEGER UNIQUE PRIMARY KEY)");

        emit OrderGetComplete();
    }
}

void EMAGMdiChild::PopulateOrderViewTable()
{
    //REWRITE FOR DATABASE
}

void EMAGMdiChild::PopulateOrderDetailsViewTable()
{
    //REWRITE FOR DATABASE

}

void EMAGMdiChild::on_PreviousDayButton_clicked()
{
    //REWRITE FOR DATABASE
    EMAGMdiChild::CurrentDate.setDate(EMAGMdiChild::CurrentDate.addDays(-1).year(), EMAGMdiChild::CurrentDate.addDays(-1).month(), EMAGMdiChild::CurrentDate.addDays(-1).day() );
    ui->OrderDateView->setDate(EMAGMdiChild::CurrentDate);
    PopulateOrderViewTable();
}

void EMAGMdiChild::on_NextDayButton_clicked()
{
    //REWRITE FOR DATABASE
    EMAGMdiChild::CurrentDate.setDate(EMAGMdiChild::CurrentDate.addDays(1).year(), EMAGMdiChild::CurrentDate.addDays(1).month(), EMAGMdiChild::CurrentDate.addDays(1).day() );
    ui->OrderDateView->setDate(EMAGMdiChild::CurrentDate);
    PopulateOrderViewTable();
}

void EMAGMdiChild::on_OrderDateView_userDateChanged(const QDate &date)
{
    //REWRITE FOR DATABASE
    EMAGMdiChild::CurrentDate.setDate(date.year(), date.month(), date.day());
    PopulateOrderViewTable();
}

void EMAGMdiChild::on_OrdersView_itemSelectionChanged()
{
    PopulateOrderDetailsViewTable();
}

void EMAGMdiChild::on_ModifyOrderButton_clicked()
{
    //ADD AN IF FOR NO TABLE SELECTION TO AVOID CRASH
    QString MonthWithZero = QString::number(EMAGMdiChild::CurrentDate.month());
    if (MonthWithZero.length() == 1)
    {
        MonthWithZero.prepend(QString::number(0));
    }

    QString DayWithZero = QString::number(EMAGMdiChild::CurrentDate.day());
    if (DayWithZero.length() == 1)
    {
        DayWithZero.prepend(QString::number(0));
    }
    //REWRITE FOR DATABASE
}
