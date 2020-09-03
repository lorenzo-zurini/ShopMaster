#include "ordereditform.h"
#include "ui_ordereditform.h"

OrderEditForm::OrderEditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderEditForm)
{
    ui->setupUi(this);
}

OrderEditForm::~OrderEditForm()
{
    delete ui;
}

void OrderEditForm::SetOrderPath(QString OrderPathToPass)
{
    // REWRITE FOR DATABASE
}

void OrderEditForm::on_CancelButton_clicked()
{
    this->parentWidget()->close();
}

void OrderEditForm::on_SaveButton_clicked()
{
    // Add the saving here then refresh the tables via signal.

}
