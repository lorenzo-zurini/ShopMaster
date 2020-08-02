#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QMainWindow>
#include <QObject>
#include <QDialog>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QBitArray>
#include <QCoreApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QStringList>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QUrlQuery>

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    ~AuthDialog();

private slots:
    void on_LoginButton_clicked();
    void on_CancelButton_clicked();   
    void RequestComplete(QNetworkReply *reply);

private:
    Ui::AuthDialog *ui;
    QNetworkAccessManager *AuthManager;
};

#endif // AUTHDIALOG_H
