#ifndef ORDEREDITFORM_H
#define ORDEREDITFORM_H

#include <QDialog>

namespace Ui {
class OrderEditForm;
}

class OrderEditForm : public QDialog
{
    Q_OBJECT

public:
    explicit OrderEditForm(QWidget *parent = nullptr);
    ~OrderEditForm();

private:
    Ui::OrderEditForm *ui;
};

#endif // ORDEREDITFORM_H
