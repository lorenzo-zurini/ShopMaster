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
    DBQuery.exec("SELECT * FROM EMAG_ORDERS WHERE ORDER_ID == " + OrderEditForm::OrderID);
    QueryModel->setQuery(DBQuery);
    WidgetMapper->setModel(QueryModel);
    WidgetMapper->addMapping(ui->OrderIDDisplayLabel, 1);
    WidgetMapper->toFirst();
}

void OrderEditForm::on_CancelButton_clicked()
{
    this->parentWidget()->close();
}

void OrderEditForm::on_SaveButton_clicked()
{
    // Add the saving here then refresh the tables via signal.

}
