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
    //When the user logs in, AuthWindow sends a post request to eMAG servers to verify the credentials.
    //The post request is identical to the one in MainWindow::on_actionPreluare_eMAG_triggered so refer to that.
    //Nothing is done with the data returned. Only the credentials are checked for validity.

    QString UnencryptedString;
    UnencryptedString.push_back(ui->LoginTextInput->text());
    UnencryptedString.push_back(":");
    UnencryptedString.push_back(ui->PasswordTextInput->text());
    AuthDialog::Hash = UnencryptedString.toUtf8().toBase64();

    AuthManager = new QNetworkAccessManager();
    connect(AuthManager, &QNetworkAccessManager::finished, this, &AuthDialog::RequestComplete);

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

    //The credentials are checked for validity and if correct, the hash is saved in a file to be used in MainWindow.
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
        WrongCredentials.setText("Date de autentificare gresite.");
        WrongCredentials.exec();
    }


}
