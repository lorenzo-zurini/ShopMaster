#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <authdialog.h>
#include <QString>
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
#include <QMenuBar>
#include <QStatusBar>
#include <QScreen>
#include <emagmdichild.h>

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

    void on_AuthRequestComplete(QNetworkReply * AuthReply);

    EMAGMdiChild * CreateMdiChild();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *AuthManager;
};
#endif // MAINWINDOW_H
