#include "emagmdichild.h"

EMAGMdiChild::EMAGMdiChild() : QTableWidget(1, 1)
{
 setAttribute(Qt::WA_DeleteOnClose);
 QTableWidgetItem PuleSule;
 QVariant Pullaaa("caca");
 PuleSule.setData(1, Pullaaa);
}
