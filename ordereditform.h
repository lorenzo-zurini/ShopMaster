#ifndef ORDEREDITFORM_H
#define ORDEREDITFORM_H

#include <QDialog>
#include <QSettings>
#include <QDebug>
#include <QSql>
#include <QSqlDatabase>

namespace Ui {
class OrderEditForm;
}

class OrderEditForm : public QDialog
{
    Q_OBJECT

public:
    explicit OrderEditForm(QWidget *parent = nullptr);
    ~OrderEditForm();

    void SetOrderPath(const QString);

private slots:
    void on_CancelButton_clicked();

    void on_SaveButton_clicked();

private:
    Ui::OrderEditForm *ui;

    QString OrderPath;

    QSettings OrderFile;

};

#endif // ORDEREDITFORM_H
