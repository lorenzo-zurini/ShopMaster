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
    OrderEditForm::WidgetMapper->submit();

    // Add the saving here then refresh the tables via signal.

}
