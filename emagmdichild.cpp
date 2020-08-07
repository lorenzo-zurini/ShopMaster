#include "emagmdichild.h"
#include "ui_emagmdichild.h"

EMAGMdiChild::EMAGMdiChild(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EMAGMdiChild)
{
    ui->setupUi(this);
}

EMAGMdiChild::~EMAGMdiChild()
{
    delete ui;
}
