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
    AuthDialog::Hash = UnencryptedString.toUtf8().toBase64();

    //The NetworkManager class is initialised and the signal is connected to our slot function.
    AuthManager = new QNetworkAccessManager();
    connect(AuthManager, &QNetworkAccessManager::finished, this, &AuthDialog::RequestComplete);

    //The post request is constructed and then executed.
    QUrl ApiUrl;
    ApiUrl.setUrl("https://marketplace.emag.ro/api-3/order/read");

    QByteArray AuthData;
    AuthData.append(Hash);
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
 this->close();
}

void AuthDialog::RequestComplete(QNetworkReply *AuthReply)
{
    qDebug() << AuthReply->readAll();

    if(AuthReply->hasRawHeader("X-User-Id"))
    {
        QFile CredentialStore(QCoreApplication::applicationDirPath() + "/CredentialStore.bin");

        if(CredentialStore.open(QIODevice::WriteOnly | QIODevice::Text))
            {
            CredentialStore.write(AuthDialog::Hash.toUtf8());
            CredentialStore.close();
            }
        this->close();
    }
    else
    {
        QMessageBox WrongCredentials;
        WrongCredentials.setText("Nigga you fucked up");
        WrongCredentials.exec();
    }


}
