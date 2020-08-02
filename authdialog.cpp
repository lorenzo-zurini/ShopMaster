#include "authdialog.h"
#include "ui_authdialog.h"


AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
    ui->PasswordTextInput->setEchoMode(QLineEdit::Password);
    AuthManager = new QNetworkAccessManager();
    connect(AuthManager, &QNetworkAccessManager::finished, this, &AuthDialog::RequestComplete);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::on_LoginButton_clicked()
{
    //The inputs are polled and the hash is generated.
    qDebug() << ui->LoginTextInput->text();
    qDebug() << ui->PasswordTextInput->text();
    QString UnencryptedString;
    UnencryptedString.push_back(ui->LoginTextInput->text());
    UnencryptedString.push_back(":");
    UnencryptedString.push_back(ui->PasswordTextInput->text());
    qDebug() << UnencryptedString;
    QString EncryptedString = UnencryptedString.toUtf8().toBase64();
    qDebug() << EncryptedString;

}

void AuthDialog::on_CancelButton_clicked()
{
 this->close();
}

void AuthDialog::RequestComplete(QNetworkReply *reply)
{
    qDebug() << "Surprise Buttsex";
}
