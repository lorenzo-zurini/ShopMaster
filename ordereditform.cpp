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
    QDataWidgetMapper * WidgetMapper = new QDataWidgetMapper;
    QSqlQueryModel * QueryModel = new QSqlQueryModel;
    QSqlQuery DBQuery;
    DBQuery.exec("SELECT * FROM EMAG_ORDERS WHERE id == " + OrderEditForm::OrderID);
    DBQuery.next();
    OrderEditForm::IsLegalEntity = DBQuery.value(35).toBool();
    QueryModel->setQuery(DBQuery);
    WidgetMapper->setModel(0);
    WidgetMapper->setModel(QueryModel);

    WidgetMapper->addMapping(ui->OrderIdDisplayLineEdit, 0);
    WidgetMapper->addMapping(ui->DateDisplayLineEdit, 1);
    WidgetMapper->addMapping(ui->ContactNameLineEdit, 47);
    WidgetMapper->addMapping(ui->ContactPhoneLineEdit, 48);
    WidgetMapper->addMapping(ui->ContactCountryLineEdit, 49);
    WidgetMapper->addMapping(ui->ContactCountyLineEdit, 50);
    WidgetMapper->addMapping(ui->ContactCityLineEdit, 51);
    WidgetMapper->addMapping(ui->ContactAdressLineEdit, 52);

    if (!IsLegalEntity)
{
    ui->BillingFrame->raise();
    WidgetMapper->addMapping(ui->BillingNameLineEdit, 40);
    WidgetMapper->addMapping(ui->BillingCountryLineEdit, 42);
    WidgetMapper->addMapping(ui->BillingCountyLineEdit, 43);
    WidgetMapper->addMapping(ui->BillingCityLineEdit, 44);
    WidgetMapper->addMapping(ui->BillingAdressLineEdit, 45);
}
    else
{
    ui->LegalEntityBillingFrame->raise();
    WidgetMapper->addMapping(ui->LegalBillingCompanyNameLineEdit, 28);
    WidgetMapper->addMapping(ui->LegalBillingCIFLineEdit, 30);
    WidgetMapper->addMapping(ui->LegalBillingNRRCLineEdit, 31);
    WidgetMapper->addMapping(ui->LegalBillingCountryLineEdit, 42);
    WidgetMapper->addMapping(ui->LegalBillingCountyLineEdit, 43);
    WidgetMapper->addMapping(ui->LegalBillingCityLineEdit, 44);
    WidgetMapper->addMapping(ui->LegalBillingAdressLineEdit, 45);
    WidgetMapper->addMapping(ui->LegalBillingAccountLineEdit, 33);
}

    WidgetMapper->toFirst();
    //ui->OrderIDDisplayLabel->setText(OrderEditForm::OrderID);

}

void OrderEditForm::on_CancelButton_clicked()
{
    this->parentWidget()->close();
}

void OrderEditForm::on_SaveButton_clicked()
{
    // Add the saving here then refresh the tables via signal.

}
