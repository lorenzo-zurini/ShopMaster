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

void OrderEditForm::PassDatabase(QSqlDatabase DataBase)
{
    OrderEditForm::DataBase = DataBase;
}

void OrderEditForm::PassID(const QString OrderID)
{
    OrderEditForm::OrderID = OrderID;
}

void OrderEditForm::Populate()
{
    OrderEditForm::DataBase.open();
    QSqlQueryModel * QueryModel = new QSqlQueryModel;
    QSqlQuery DBQuery;
    DBQuery.exec("SELECT * FROM EMAG_ORDERS WHERE id == " + OrderEditForm::OrderID);
    DBQuery.next();
    OrderEditForm::IsLegalEntity = DBQuery.value(35).toBool();
    QueryModel->setQuery(DBQuery);
    OrderEditForm::WidgetMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    OrderEditForm::WidgetMapper->setModel(0);
    OrderEditForm::WidgetMapper->setModel(QueryModel);

    OrderEditForm::WidgetMapper->addMapping(ui->OrderIdDisplayLineEdit, 0);
    OrderEditForm::WidgetMapper->addMapping(ui->DateDisplayLineEdit, 1);
    OrderEditForm::WidgetMapper->addMapping(ui->ContactNameLineEdit, 47);
    OrderEditForm::WidgetMapper->addMapping(ui->ContactPhoneLineEdit, 48);
    OrderEditForm::WidgetMapper->addMapping(ui->ContactCountryLineEdit, 49);
    OrderEditForm::WidgetMapper->addMapping(ui->ContactCountyLineEdit, 50);
    OrderEditForm::WidgetMapper->addMapping(ui->ContactCityLineEdit, 51);
    OrderEditForm::WidgetMapper->addMapping(ui->ContactAdressLineEdit, 52);

    if (!IsLegalEntity)
{

    ui->BillingFrame->raise();
    OrderEditForm::WidgetMapper->addMapping(ui->BillingNameLineEdit, 40);
    OrderEditForm::WidgetMapper->addMapping(ui->BillingCountryLineEdit, 42);
    OrderEditForm::WidgetMapper->addMapping(ui->BillingCountyLineEdit, 43);
    OrderEditForm::WidgetMapper->addMapping(ui->BillingCityLineEdit, 44);
    OrderEditForm::WidgetMapper->addMapping(ui->BillingAdressLineEdit, 45);

}

    else

{

    ui->LegalEntityBillingFrame->raise();
    OrderEditForm::WidgetMapper->addMapping(ui->LegalBillingCompanyNameLineEdit, 28);
    OrderEditForm::WidgetMapper->addMapping(ui->LegalBillingCIFLineEdit, 30);
    OrderEditForm::WidgetMapper->addMapping(ui->LegalBillingNRRCLineEdit, 31);
    OrderEditForm::WidgetMapper->addMapping(ui->LegalBillingCountryLineEdit, 42);
    OrderEditForm::WidgetMapper->addMapping(ui->LegalBillingCountyLineEdit, 43);
    OrderEditForm::WidgetMapper->addMapping(ui->LegalBillingCityLineEdit, 44);
    OrderEditForm::WidgetMapper->addMapping(ui->LegalBillingAdressLineEdit, 45);
    OrderEditForm::WidgetMapper->addMapping(ui->LegalBillingAccountLineEdit, 33);

}


    OrderEditForm::WidgetMapper->toFirst();

}

void OrderEditForm::on_CancelButton_clicked()
{
    this->parentWidget()->close();
}

void OrderEditForm::on_SaveButton_clicked()
{
    OrderEditForm::DataBase.open();
    QSqlQuery DBQuery;
    DBQuery.exec("SELECT * FROM EMAG_ORDERS WHERE id == " + OrderEditForm::OrderID);
    DBQuery.next();
    OrderEditForm::IsLegalEntity = DBQuery.value(35).toBool();

    DBQuery.exec("UPDATE EMAG_ORDERS SET shipping_contact='"+ui->ContactNameLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET shipping_phone='"+ui->ContactPhoneLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET shipping_country='"+ui->ContactCountryLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET shipping_suburb='"+ui->ContactCountyLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET shipping_city='"+ui->ContactCityLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET shipping_street='"+ui->ContactAdressLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);

    if (!IsLegalEntity)
    {

    DBQuery.exec("UPDATE EMAG_ORDERS SET billing_name='"+ui->BillingNameLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET billing_country='"+ui->BillingCountryLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET billing_suburb='"+ui->BillingCountyLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET billing_city='"+ui->BillingCityLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET billing_street='"+ui->BillingAdressLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);

    }

    else

    {

    DBQuery.exec("UPDATE EMAG_ORDERS SET company='"+ui->LegalBillingCompanyNameLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET code='"+ui->LegalBillingCIFLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET registration_number='"+ui->LegalBillingNRRCLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET billing_country='"+ui->LegalBillingCountryLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET billing_suburb='"+ui->LegalBillingCountyLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET billing_city='"+ui->LegalBillingCityLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET billing_street='"+ui->LegalBillingAdressLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);
    DBQuery.exec("UPDATE EMAG_ORDERS SET iban='"+ui->LegalBillingAccountLineEdit->text()+"' WHERE id == " + OrderEditForm::OrderID);

    }

    this->parentWidget()->close();
    emit OrderModifySaved();

    // Add the saving here then refresh the tables via signal.

}
