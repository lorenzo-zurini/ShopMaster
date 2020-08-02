#include "authdialog.h"
#include "ui_authdialog.h"


AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
    ui->PasswordTextInput->setEchoMode(QLineEdit::Password);


}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::on_LoginButton_clicked()
{
    //The inputs are polled and the hash is generated.
    QString UnencryptedString;
    UnencryptedString.push_back(ui->LoginTextInput->text());
    UnencryptedString.push_back(":");
    UnencryptedString.push_back(ui->PasswordTextInput->text());
    QString EncryptedString = UnencryptedString.toUtf8().toBase64();

    //The NetworkManager class is initialised and the signal is connected to our slot function.
    AuthManager = new QNetworkAccessManager();
    connect(AuthManager, &QNetworkAccessManager::finished, this, &AuthDialog::RequestComplete);

    //The post request is constructed and then executed.
    QUrl ApiUrl;
    ApiUrl.setUrl("https://marketplace.emag.ro/api-3/order/read");

    QByteArray PostData;
    QUrlQuery Params;
    Params.addQueryItem("code", ui->APICodeTextInput->text());
    Params.addQueryItem("username", ui->LoginTextInput->text());
    Params.addQueryItem("data", "currentPage=1&itemsPerPage=100");
    Params.addQueryItem("hash", EncryptedString);
    PostData.append(Params.toString());
    QNetworkRequest ApiRequest;
    ApiRequest.setUrl(ApiUrl);
    ApiRequest.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("0"));
    AuthManager->post(ApiRequest, PostData);
    qDebug()<< PostData;

    //QNetworkRequest GetRequest;
    //ApiUrl.setUserName(ui->LoginTextInput->text());
    //ApiUrl.setPassword(ui->PasswordTextInput->text());
    //GetRequest.setUrl(ApiUrl);
    //AuthManager->get(GetRequest);


}

void AuthDialog::on_CancelButton_clicked()
{
 QCoreApplication::quit();
}

void AuthDialog::RequestComplete(QNetworkReply *AuthReply)
{
    qDebug() << AuthReply->readAll();
}
