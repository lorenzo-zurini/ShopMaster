#ifndef ORDEREDITFORM_H
#define ORDEREDITFORM_H

#include <QDialog>
#include <QSettings>
#include <QDebug>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDataWidgetMapper>
#include <QSqlQueryModel>

namespace Ui {
class OrderEditForm;
}

class OrderEditForm : public QDialog
{
    Q_OBJECT

public:
    explicit OrderEditForm(QWidget *parent = nullptr);
    ~OrderEditForm();

    void PassDatabase(QSqlDatabase DataBase);

    void PassID(const QString OrderID);

    void Populate();

private slots:
    void on_CancelButton_clicked();

    void on_SaveButton_clicked();

private:
    Ui::OrderEditForm *ui;

    QString OrderID;

    QSqlDatabase DataBase;

    bool IsLegalEntity;

};

#endif // ORDEREDITFORM_H
