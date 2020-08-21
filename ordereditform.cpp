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
