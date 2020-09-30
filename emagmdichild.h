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
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
class EMAGMdiChild;
}

class EMAGMdiChild : public QWidget
{
    Q_OBJECT

public:
    explicit EMAGMdiChild(QWidget *parent = nullptr);
    ~EMAGMdiChild();

    void PassDatabase(QSqlDatabase DataBase);

private slots:
    void on_AuthRequestComplete(QNetworkReply * AuthReply);
    
    void PopulateOrderViewTable();

    void PopulateOrderDetailsViewTable();

    void on_PreviousDayButton_clicked();

    void on_NextDayButton_clicked();

    void on_OrderDateView_userDateChanged(const QDate &date);

    void on_ModifyOrderButton_clicked();

    void on_OrdersView_clicked(const QModelIndex &index);

    void UpdateTables();

private:
    Ui::EMAGMdiChild *ui;

    QNetworkAccessManager *AuthManager;

    void GetEMAGOrders();

    QDir EMAGOrdersDirectory;

    QDate CurrentDate;

    QSqlDatabase DataBase;
    
signals:
    void OrderGetComplete();

    void OrderEdit(QString);
    
};

#endif // EMAGMDICHILD_H
