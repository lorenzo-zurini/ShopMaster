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
    qDebug() << ui->LoginTextInput->text();
    qDebug() << ui->PasswordTextInput->text();
}

void AuthDialog::on_CancelButton_clicked()
{
 this->close();
}
