#include "authdialog.h"
#include "ui_authdialog.h"


AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
    ui->PasswordTextInput->setEchoMode(QLineEdit::Password);
    this->setWindowTitle("Autentificare eMAG");
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
    //ApiUrl.setUrl("https://marketplace.emag.ro/api-3");

    QByteArray AuthData;
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

void AuthDialog::on_CancelButton_clicked()
{
 QCoreApplication::quit();
}

void AuthDialog::RequestComplete(QNetworkReply *AuthReply)
{
    qDebug() << AuthReply->readAll();
}
