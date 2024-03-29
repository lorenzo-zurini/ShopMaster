#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <authdialog.h>
#include <emagmdichild.h>
#include <ordereditform.h>
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
#include <QMenuBar>
#include <QStatusBar>
#include <QScreen>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void on_actionPreluare_eMAG_triggered();

    void on_actionAutentificare_eMAG_triggered();

    void OrderEdit(QString OrderID);

private:

    EMAGMdiChild * CreateEMAGMdiChild();

    OrderEditForm * CreateOrderEditForm();

    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");

    QDir DataDir;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
