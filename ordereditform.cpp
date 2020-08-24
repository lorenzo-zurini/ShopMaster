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
    // This sucks, do this better.
    QSettings * OrderFile = new QSettings(OrderPathToPass, QSettings::IniFormat);
    OrderEditForm::OrderPath = OrderPathToPass;

    ui->OrderIDDisplayLabel->setText(QString::number(OrderFile->value("ORDER_DATA/ID").toInt()));
    ui->DateDisplayLabel->setText(OrderFile->value("ORDER_DATA/DATE").toString());
    ui->ContactNameLineEdit->setText(OrderFile->value("CONTACT_DATA/CONTACT_NAME").toString());
    ui->ContactPhoneLineEdit->setText(OrderFile->value("CONTACT_DATA/CONTACT_PHONE").toString());
    ui->ContactCountryLineEdit->setText(OrderFile->value("CONTACT_DATA/CONTACT_COUNTRY").toString());
    ui->ContactCountyLineEdit->setText(OrderFile->value("CONTACT_DATA/CONTACT_COUNTY").toString());
    ui->ContactCityLineEdit->setText(OrderFile->value("CONTACT_DATA/CONTACT_CITY").toString());
    ui->ContactAdressLineEdit->setText(OrderFile->value("CONTACT_DATA/CONTACT_ADRESS").toString());
    ui->BillingNameLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_NAME").toString());
    ui->BillingCountryLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_COUNTRY").toString());
    ui->BillingCountyLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_COUNTY").toString());
    ui->BillingCityLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_CITY").toString());
    ui->BillingAdressLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_ADRESS").toString());
    ui->LegalBillingCompanyNameLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_COMPANY_NAME").toString());
    ui->LegalBillingCIFLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_CIF").toString());
    ui->LegalBillingNRRCLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_NRRC").toString());
    // DONT FORGET TO ADD A CHECK WHEN SAVING IF LEGAL ENTITY, THE FOLLOWING GET VALUES REGARDLESS.
    ui->LegalBillingCountryLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_COUNTRY").toString());
    ui->LegalBillingCountyLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_COUNTY").toString());
    ui->LegalBillingCityLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_CITY").toString());
    ui->LegalBillingAdressLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_ADRESS").toString());
    ui->LegalBillingAccountLineEdit->setText(OrderFile->value("BILLING_DATA/BILLING_ACCOUNT").toString());
}

void OrderEditForm::on_CancelButton_clicked()
{
    this->parentWidget()->close();
}

void OrderEditForm::on_SaveButton_clicked()
{
    QSettings * OrderFile = new QSettings(OrderEditForm::OrderPath, QSettings::IniFormat);
    // Add the saving here then refresh the tables via signal.

}
