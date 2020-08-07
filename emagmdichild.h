#ifndef EMAGMDICHILD_H
#define EMAGMDICHILD_H

#include <QWidget>

namespace Ui {
class EMAGMdiChild;
}

class EMAGMdiChild : public QWidget
{
    Q_OBJECT

public:
    explicit EMAGMdiChild(QWidget *parent = nullptr);
    ~EMAGMdiChild();

private:
    Ui::EMAGMdiChild *ui;
};

#endif // EMAGMDICHILD_H
