#ifndef EMAGMDICHILD_H
#define EMAGMDICHILD_H

#include <QWidget>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QStringList>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>

namespace Ui {
class EMAGMdiChild;
}

class EMAGMdiChild : public QWidget
{
    Q_OBJECT

public:
    explicit EMAGMdiChild(QWidget *parent = nullptr);
    ~EMAGMdiChild();

private slots:
    void on_AuthRequestComplete(QNetworkReply * AuthReply);

private:
    Ui::EMAGMdiChild *ui;

    QNetworkAccessManager *AuthManager;

    void GetEMAGOrders();

    QDir EMAGOrdersDirectory;
};

#endif // EMAGMDICHILD_H
