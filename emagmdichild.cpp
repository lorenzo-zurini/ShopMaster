#include "emagmdichild.h"

EMAGMdiChild::EMAGMdiChild() : QTableWidget(30, 10)
  //SECO PLS EXPLAIN HOW THIS ABOVE WORKS CAUSE I DIDNT UNDERSTAND EVEN THOUGH I SMILED AND NODDED
{
 setAttribute(Qt::WA_DeleteOnClose);
 QTableWidgetItem PuleSule;
 QVariant Pullaaa("caca");
 PuleSule.setData(1, Pullaaa);
}
