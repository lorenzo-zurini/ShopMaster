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
    ApiRequest.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    //The data body of the post request is constructed here (doesn´t appear to work yet).
    QByteArray PostData;
    PostData.append("currentPage=1&itemsPerPage=100");

    //The request is made and the data is printed to the debug console.
    AuthManager->post(ApiRequest, PostData);
    // qDebug() << PostData;
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

        //THE MAIN ORDERS TABLE
        QueryData.append("");   QueryData.append("CREATE TABLE IF NOT EXISTS EMAG_ORDERS");     QueryData.append(" (");   
        QueryData.append("");   QueryData.append("id INTEGER UNIQUE PRIMARY KEY");              QueryData.append(", ");
        QueryData.append("");   QueryData.append("date DATETIME");                              QueryData.append(", ");
        QueryData.append("");   QueryData.append("status INTEGER");                             QueryData.append(", ");
        QueryData.append("");   QueryData.append("internal_status INTEGER");                    QueryData.append(", ");
        QueryData.append("");   QueryData.append("type INTEGER");                               QueryData.append(", ");
        QueryData.append("");   QueryData.append("cancellation_request INTEGER");               QueryData.append(", ");
        QueryData.append("");   QueryData.append("cancellation_reason INTEGER");                QueryData.append(", ");
        QueryData.append("");   QueryData.append("is_storno INTEGER");                          QueryData.append(", ");
        QueryData.append("");   QueryData.append("cashed_co REAL");                             QueryData.append(", ");
        QueryData.append("");   QueryData.append("cashed_cod REAL");                            QueryData.append(", ");
        QueryData.append("");   QueryData.append("delivery_mode TEXT");                         QueryData.append(", ");
        QueryData.append("");   QueryData.append("detailed_payment_method TEXT");               QueryData.append(", ");
        QueryData.append("");   QueryData.append("emag_club INTEGER");                          QueryData.append(", ");
        QueryData.append("");   QueryData.append("has_editable_products INTEGER");              QueryData.append(", ");
        QueryData.append("");   QueryData.append("is_complete INTEGER");                        QueryData.append(", ");
        QueryData.append("");   QueryData.append("maximum_date_for_shipment DATETIME");         QueryData.append(", ");
        QueryData.append("");   QueryData.append("observation TEXT");                           QueryData.append(", ");
        QueryData.append("");   QueryData.append("parent_id INTEGER");                          QueryData.append(", ");
        QueryData.append("");   QueryData.append("payment_mode TEXT");                          QueryData.append(", ");
        QueryData.append("");   QueryData.append("payment_mode_id INTEGER");                    QueryData.append(", ");
        QueryData.append("");   QueryData.append("payment_status INTEGER");                     QueryData.append(", ");
        QueryData.append("");   QueryData.append("refund_status INTEGER");                      QueryData.append(", ");
        QueryData.append("");   QueryData.append("refunded_amount REAL");                       QueryData.append(", ");
        QueryData.append("");   QueryData.append("shipping_tax REAL");                          QueryData.append(", ");
        QueryData.append("");   QueryData.append("vendor_name TEXT");                           QueryData.append(", ");
        //THE CUSTOMER SECTION
        QueryData.append("");   QueryData.append("customer_id INTEGER");                        QueryData.append(", "); // Modified COLUMN name!!
        QueryData.append("");   QueryData.append("name TEXT");                                  QueryData.append(", "); // ADD MORE FIELDS THAT ARE
        QueryData.append("");   QueryData.append("email TEXT");                                 QueryData.append(", "); // NOT IN THE DOCUMENTATION
        QueryData.append("");   QueryData.append("company TEXT");                               QueryData.append(", "); // BUT SHOW UP IN THE JSONS
        QueryData.append("");   QueryData.append("gender TEXT");                                QueryData.append(", ");
        QueryData.append("");   QueryData.append("code TEXT");                                  QueryData.append(", ");
        QueryData.append("");   QueryData.append("registration_number TEXT");                   QueryData.append(", ");
        QueryData.append("");   QueryData.append("bank TEXT");                                  QueryData.append(", ");
        QueryData.append("");   QueryData.append("iban TEXT");                                  QueryData.append(", ");
        QueryData.append("");   QueryData.append("fax TEXT");                                   QueryData.append(", ");
        QueryData.append("");   QueryData.append("legal_entity INTEGER");                       QueryData.append(", ");
        QueryData.append("");   QueryData.append("is_vat_payer INTEGER");                       QueryData.append(", ");
        QueryData.append("");   QueryData.append("phone_1 TEXT");                               QueryData.append(", ");
        QueryData.append("");   QueryData.append("phone_2 TEXT");                               QueryData.append(", ");
        QueryData.append("");   QueryData.append("phone_3 TEXT");                               QueryData.append(", ");
        QueryData.append("");   QueryData.append("billing_name TEXT");                          QueryData.append(", ");
        QueryData.append("");   QueryData.append("billing_phone TEXT");                         QueryData.append(", ");
        QueryData.append("");   QueryData.append("billing_country TEXT");                       QueryData.append(", ");
        QueryData.append("");   QueryData.append("billing_suburb TEXT");                        QueryData.append(", ");
        QueryData.append("");   QueryData.append("billing_city TEXT");                          QueryData.append(", ");
        QueryData.append("");   QueryData.append("billing_street TEXT");                        QueryData.append(", ");
        QueryData.append("");   QueryData.append("billing_postal_code TEXT");                   QueryData.append(", ");
        QueryData.append("");   QueryData.append("shipping_contact TEXT");                      QueryData.append(", ");
        QueryData.append("");   QueryData.append("shipping_phone TEXT");                        QueryData.append(", ");
        QueryData.append("");   QueryData.append("shipping_country TEXT");                      QueryData.append(", ");
        QueryData.append("");   QueryData.append("shipping_suburb TEXT");                       QueryData.append(", ");
        QueryData.append("");   QueryData.append("shipping_city TEXT");                         QueryData.append(", ");
        QueryData.append("");   QueryData.append("shipping_street TEXT");                       QueryData.append(", ");
        QueryData.append("");   QueryData.append("shipping_postal_code TEXT");                  QueryData.append(", ");
        QueryData.append("");   QueryData.append("billing_locality_id INTEGER");                QueryData.append(", ");
        QueryData.append("");   QueryData.append("shipping_locality_id INTEGER");               QueryData.append(")");

        DBQuery.exec(QueryData);
        // qDebug().noquote() << QueryData;
        QueryData.clear();

        //THE LINKED ORDERED PRODUCTS TABLE
        QueryData.append("");   QueryData.append("CREATE TABLE IF NOT EXISTS EMAG_ORDERED_PRODUCTS");       QueryData.append(" ("); // ADD ATTACHMENTS, DETAILS
        QueryData.append("");   QueryData.append("parent_order_id INTEGER PRIMARY KEY");                    QueryData.append(", "); // Internal key
        QueryData.append("");   QueryData.append("id INTEGER");                                             QueryData.append(", ");
        QueryData.append("");   QueryData.append("product_id INTEGER");                                     QueryData.append(", ");
        QueryData.append("");   QueryData.append("status INTEGER");                                         QueryData.append(", ");
        QueryData.append("");   QueryData.append("part_number TEXT");                                       QueryData.append(", ");
        QueryData.append("");   QueryData.append("created DATETIME");                                       QueryData.append(", ");
        QueryData.append("");   QueryData.append("modified DATETIME");                                      QueryData.append(", ");
        QueryData.append("");   QueryData.append("currency TEXT");                                          QueryData.append(", ");
        QueryData.append("");   QueryData.append("quantity INTEGER");                                       QueryData.append(", ");
        QueryData.append("");   QueryData.append("sale_price REAL");                                        QueryData.append(", ");
        QueryData.append("");   QueryData.append("details TEXT");                                           QueryData.append(", ");
        QueryData.append("");   QueryData.append("ext_part_number TEXT");                                   QueryData.append(", ");
        QueryData.append("");   QueryData.append("retained_amount REAL");                                   QueryData.append(", ");
        QueryData.append("");   QueryData.append("original_price REAL");                                    QueryData.append(", ");
        QueryData.append("");   QueryData.append("mkt_id INTEGER");                                         QueryData.append(", ");
        QueryData.append("");   QueryData.append("vat INTEGER");                                            QueryData.append(", ");
        QueryData.append("");   QueryData.append("initial_qty INTEGER");                                    QueryData.append(", ");
        QueryData.append("");   QueryData.append("storno_qty INTEGER");                                     QueryData.append(", ");
        QueryData.append("");   QueryData.append("reversible_vat_charging INTEGER");                        QueryData.append(")");

        DBQuery.exec(QueryData);
       // qDebug().noquote() << QueryData;
        QueryData.clear();

        for (int i = 0; i < OrdersArray.size(); i++)
        {
            QueryData.append("INSERT INTO EMAG_ORDERS");
            QueryData.append(" (");         QueryData.append("id");                           QueryData.append(", ");
            QueryData.append("");           QueryData.append("date");                         QueryData.append(", ");
            QueryData.append("");           QueryData.append("status");                       QueryData.append(", ");
            QueryData.append("");           QueryData.append("internal_status");              QueryData.append(", ");
            QueryData.append("");           QueryData.append("type");                         QueryData.append(", ");
            QueryData.append("");           QueryData.append("cancellation_request");         QueryData.append(", ");
            QueryData.append("");           QueryData.append("cancellation_reason");          QueryData.append(", ");
            QueryData.append("");           QueryData.append("is_storno");                    QueryData.append(", ");
            QueryData.append("");           QueryData.append("cashed_co");                    QueryData.append(", ");
            QueryData.append("");           QueryData.append("cashed_cod");                   QueryData.append(", ");
            QueryData.append("");           QueryData.append("delivery_mode");                QueryData.append(", ");
            QueryData.append("");           QueryData.append("detailed_payment_method");      QueryData.append(", ");
            QueryData.append("");           QueryData.append("emag_club");                    QueryData.append(", ");
            QueryData.append("");           QueryData.append("has_editable_products");        QueryData.append(", ");
            QueryData.append("");           QueryData.append("is_complete");                  QueryData.append(", ");
            QueryData.append("");           QueryData.append("maximum_date_for_shipment");    QueryData.append(", ");
            QueryData.append("");           QueryData.append("observation");                  QueryData.append(", ");
            QueryData.append("");           QueryData.append("parent_id");                    QueryData.append(", ");
            QueryData.append("");           QueryData.append("payment_mode");                 QueryData.append(", ");
            QueryData.append("");           QueryData.append("payment_mode_id");              QueryData.append(", ");
            QueryData.append("");           QueryData.append("payment_status");               QueryData.append(", ");
            QueryData.append("");           QueryData.append("refund_status");                QueryData.append(", ");
            QueryData.append("");           QueryData.append("refunded_amount");              QueryData.append(", ");
            QueryData.append("");           QueryData.append("shipping_tax");                 QueryData.append(", ");
            QueryData.append("");           QueryData.append("vendor_name");                  QueryData.append(", ");

            QueryData.append("");           QueryData.append("customer_id");                  QueryData.append(", ");   // Modified COLUMN Name
            QueryData.append("");           QueryData.append("name");                         QueryData.append(", ");
            QueryData.append("");           QueryData.append("email");                        QueryData.append(", ");
            QueryData.append("");           QueryData.append("company");                      QueryData.append(", ");
            QueryData.append("");           QueryData.append("gender");                       QueryData.append(", ");
            QueryData.append("");           QueryData.append("code");                         QueryData.append(", ");
            QueryData.append("");           QueryData.append("registration_number");          QueryData.append(", ");
            QueryData.append("");           QueryData.append("bank");                         QueryData.append(", ");
            QueryData.append("");           QueryData.append("iban");                         QueryData.append(", ");
            QueryData.append("");           QueryData.append("fax");                          QueryData.append(", ");
            QueryData.append("");           QueryData.append("legal_entity");                 QueryData.append(", ");
            QueryData.append("");           QueryData.append("is_vat_payer");                 QueryData.append(", ");
            QueryData.append("");           QueryData.append("phone_1");                      QueryData.append(", ");
            QueryData.append("");           QueryData.append("phone_2");                      QueryData.append(", ");
            QueryData.append("");           QueryData.append("phone_3");                      QueryData.append(", ");
            QueryData.append("");           QueryData.append("billing_name");                 QueryData.append(", ");
            QueryData.append("");           QueryData.append("billing_phone");                QueryData.append(", ");
            QueryData.append("");           QueryData.append("billing_country");              QueryData.append(", ");
            QueryData.append("");           QueryData.append("billing_suburb");               QueryData.append(", ");
            QueryData.append("");           QueryData.append("billing_city");                 QueryData.append(", ");
            QueryData.append("");           QueryData.append("billing_street");               QueryData.append(", ");
            QueryData.append("");           QueryData.append("billing_postal_code");          QueryData.append(", ");
            QueryData.append("");           QueryData.append("shipping_contact");             QueryData.append(", ");
            QueryData.append("");           QueryData.append("shipping_phone");               QueryData.append(", ");
            QueryData.append("");           QueryData.append("shipping_country");             QueryData.append(", ");
            QueryData.append("");           QueryData.append("shipping_suburb");              QueryData.append(", ");
            QueryData.append("");           QueryData.append("shipping_city");                QueryData.append(", ");
            QueryData.append("");           QueryData.append("shipping_street");              QueryData.append(", ");
            QueryData.append("");           QueryData.append("shipping_postal_code");         QueryData.append(", ");
            QueryData.append("");           QueryData.append("billing_locality_id");          QueryData.append(", ");
            QueryData.append("");           QueryData.append("shipping_locality_id");         QueryData.append(") ");
            QueryData.append("VALUES");
            QueryData.append(" (");         QueryData.append(QString::number(OrdersArray.at(i).toObject()["id"].toInt()));                              QueryData.append(", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["date"].toString());                                          QueryData.append("\", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["status"].toInt()));                          QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["status"].toInt()));                          QueryData.append(", ");  // CHANGE THIS
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["type"].toInt()));                            QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["cancellation_request"].toInt()));            QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["cancellation_reason"].toInt()));             QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["is_storno"].toInt()));                       QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["cashed_co"].toDouble()));                    QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["cashed_cod"].toDouble()));                   QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["delivery_mode"].toInt()));                   QueryData.append(", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["detailed_payment_method"].toString());                       QueryData.append("\", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["emag_club"].toInt()));                       QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["has_editable_products"].toInt()));           QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["is_complete"].toInt()));                     QueryData.append(", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["maximum_date_for_shipment"].toString());                     QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["observation"].toString());                                   QueryData.append("\", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["parent_id"].toInt()));                       QueryData.append(", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["payment_mode"].toString());                                  QueryData.append("\", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["payment_mode_id"].toInt()));                 QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["payment_status"].toInt()));                  QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["refund_status"].toInt()));                   QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["refunded_amount"].toDouble()));                 QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["shipping_tax"].toDouble()));                 QueryData.append(", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["vendor_name"].toString());                                   QueryData.append("\", ");

            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["customer"].toObject()["id"].toInt()));                       QueryData.append(", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["name"].toString());                                   QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["email"].toString());                                  QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["company"].toString());                                QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["gender"].toString());                                 QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["code"].toString());                                   QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["registration_number"].toString());                    QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["bank"].toString());                                   QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["iban"].toString());                                   QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["fax"].toString());                                    QueryData.append("\", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["customer"].toObject()["legal_entity"].toInt()));             QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["customer"].toObject()["is_vat_payer"].toInt()));             QueryData.append(", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["phone_1"].toString());                                QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["phone_2"].toString());                                QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["phone_3"].toString());                                QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["billing_name"].toString());                           QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["billing_phone"].toString());                          QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["billing_country"].toString());                        QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["billing_suburb"].toString());                         QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["billing_city"].toString());                           QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["billing_street"].toString());                         QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["billing_postal_code"].toString());                    QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["shipping_contact"].toString());                       QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["shipping_phone"].toString());                         QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["shipping_country"].toString());                       QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["shipping_suburb"].toString());                        QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["shipping_city"].toString());                          QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["shipping_street"].toString());                        QueryData.append("\", ");
            QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["customer"].toObject()["shipping_postal_code"].toString());                   QueryData.append("\", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["customer"].toObject()["billing_locality_id"].toInt()));      QueryData.append(", ");
            QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["customer"].toObject()["shipping_locality_id"].toInt()));     QueryData.append(")");

            DBQuery.exec(QueryData);
           //  qDebug().noquote() << QueryData;
            QueryData.clear();

            for (int j = 0; j < OrdersArray.at(i).toObject()["products"].toArray().size(); j++)
            {
                QueryData.append("INSERT INTO EMAG_ORDERED_PRODUCTS");
                QueryData.append(" (");         QueryData.append("parent_order_id");                      QueryData.append(", ");
                QueryData.append("");           QueryData.append("id");                                   QueryData.append(", ");
                QueryData.append("");           QueryData.append("product_id");                           QueryData.append(", ");
                QueryData.append("");           QueryData.append("status");                               QueryData.append(", ");
                QueryData.append("");           QueryData.append("part_number");                          QueryData.append(", ");
                QueryData.append("");           QueryData.append("created");                              QueryData.append(", ");
                QueryData.append("");           QueryData.append("modified");                             QueryData.append(", ");
                QueryData.append("");           QueryData.append("currency");                             QueryData.append(", ");
                QueryData.append("");           QueryData.append("quantity");                             QueryData.append(", ");
                QueryData.append("");           QueryData.append("sale_price");                           QueryData.append(", ");   // ITS NOT ACTUALLY AN INT
                QueryData.append("");           QueryData.append("details");                              QueryData.append(", ");   // WILL NEED TO BE FIXED
                QueryData.append("");           QueryData.append("ext_part_number");                      QueryData.append(", ");   // THIS AND ALL TE OTHER PLACES
                QueryData.append("");           QueryData.append("retained_amount");                      QueryData.append(", ");   // WHERE THE COUMENTATION IS WRONG
                QueryData.append("");           QueryData.append("original_price");                       QueryData.append(", ");
                QueryData.append("");           QueryData.append("mkt_id");                               QueryData.append(", ");
                QueryData.append("");           QueryData.append("vat");                                  QueryData.append(", ");
                QueryData.append("");           QueryData.append("initial_qty");                          QueryData.append(", ");
                QueryData.append("");           QueryData.append("storno_qty");                           QueryData.append(", ");
                QueryData.append("");           QueryData.append("reversible_vat_charging");              QueryData.append(") ");
                QueryData.append("VALUES");
                QueryData.append(" (");         QueryData.append(QString::number(OrdersArray.at(i).toObject()["id"].toInt()));                                                              QueryData.append(", ");
                QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["id"].toInt()));                       QueryData.append(", ");
                QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["product_id"].toInt()));               QueryData.append(", ");
                QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["status"].toInt()));                   QueryData.append(", ");
                QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["part_number"].toString());                            QueryData.append("\", ");
                QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["created"].toString());                                QueryData.append("\", ");
                QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["modified"].toString());                               QueryData.append("\", ");
                QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["currency"].toString());                               QueryData.append("\", ");
                QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["quantity"].toInt()));                 QueryData.append(", ");
                QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["sale_price"].toString());                             QueryData.append("\", ");
                QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["details"].toString());                                QueryData.append("\", ");
                QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["ext_part_number"].toString());                        QueryData.append("\", ");
                QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["retained_ammount"].toString());                       QueryData.append("\", ");
                QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["original_price"].toString());                         QueryData.append("\", ");
                QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["mkt_id"].toInt()));                   QueryData.append(", ");
                QueryData.append("\"");         QueryData.append(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["vat"].toString());                                    QueryData.append("\", ");
                QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["initial_qty"].toInt()));              QueryData.append(", ");
                QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["storno_qty"].toInt()));               QueryData.append(", ");
                QueryData.append("");           QueryData.append(QString::number(OrdersArray.at(i).toObject()["products"].toArray().at(j).toObject()["reversible_vat_charging"].toInt()));  QueryData.append(")");

                DBQuery.exec(QueryData);
                // qDebug().noquote() << QueryData;
                QueryData.clear();
            }
        }

        emit OrderGetComplete();
    }
}

void EMAGMdiChild::PopulateOrderViewTable()
{
    EMAGMdiChild::DataBase.open();
    QSqlQueryModel * QueryModel = new QSqlQueryModel;
    QSqlQuery DBQuery;
    QString QueryData;
    DBQuery.exec("SELECT * FROM EMAG_ORDERS WHERE date BETWEEN \"" + EMAGMdiChild::CurrentDate.startOfDay().toString(Qt::ISODate).replace("T", " ") + "\" AND \"" + EMAGMdiChild::CurrentDate.endOfDay().toString(Qt::ISODate).replace("T", " ") + "\"");
    QueryModel->setQuery(DBQuery);
    ui->OrdersView->setModel(QueryModel);
}

void EMAGMdiChild::PopulateOrderDetailsViewTable()
{

    //REWRITE FOR DATABASE

}

void EMAGMdiChild::on_PreviousDayButton_clicked()
{
    ui->OrderDetailsView->setModel(0);
    EMAGMdiChild::CurrentDate.setDate(EMAGMdiChild::CurrentDate.addDays(-1).year(), EMAGMdiChild::CurrentDate.addDays(-1).month(), EMAGMdiChild::CurrentDate.addDays(-1).day() );
    ui->OrderDateView->setDate(EMAGMdiChild::CurrentDate);
    PopulateOrderViewTable();
}

void EMAGMdiChild::on_NextDayButton_clicked()
{
    ui->OrderDetailsView->setModel(0);
    EMAGMdiChild::CurrentDate.setDate(EMAGMdiChild::CurrentDate.addDays(1).year(), EMAGMdiChild::CurrentDate.addDays(1).month(), EMAGMdiChild::CurrentDate.addDays(1).day() );
    ui->OrderDateView->setDate(EMAGMdiChild::CurrentDate);
    PopulateOrderViewTable();
}

void EMAGMdiChild::on_OrderDateView_userDateChanged(const QDate &date)
{
    ui->OrderDetailsView->setModel(0);
    EMAGMdiChild::CurrentDate.setDate(date.year(), date.month(), date.day());
    PopulateOrderViewTable();
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
    QItemSelectionModel *select = ui->OrdersView->selectionModel();

    //REWRITE FOR DATABASE
    if (select->hasSelection() == 1)
    {
        emit OrderEdit(ui->OrdersView->model()->data(ui->OrdersView->model()->index(select->currentIndex().row() , 0)).toString());
    }
        else
    {
        QMessageBox NoOrderSelected;
        NoOrderSelected.setText("Intai selectati o comanda");
        NoOrderSelected.exec();
    }
}

void EMAGMdiChild::on_OrdersView_clicked(const QModelIndex &index)
{
    ui->OrderDetailsView->setModel(0);
    // qDebug() << ui->OrdersView->model()->data(ui->OrdersView->model()->index(index.row() , 0)).toString();
    EMAGMdiChild::DataBase.open();
    QSqlQueryModel * QueryModel = new QSqlQueryModel;
    QSqlQuery DBQuery;
    DBQuery.exec( "SELECT * FROM EMAG_ORDERED_PRODUCTS WHERE parent_order_id == " + ui->OrdersView->model()->data(ui->OrdersView->model()->index(index.row() , 0)).toString());
    QueryModel->setQuery(DBQuery);
    ui->OrderDetailsView->setModel(QueryModel);

}

