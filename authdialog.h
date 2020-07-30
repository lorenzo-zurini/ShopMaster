#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QMainWindow>
#include <QDialog>
#include <QDebug>

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    ~AuthDialog();

private slots:
    void on_LoginButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::AuthDialog *ui;
};

#endif // AUTHDIALOG_H
