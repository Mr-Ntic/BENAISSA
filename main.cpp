#include "b_calts.h" //BATTRY_CHARGE_AND LOAD TEST SET .
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    B_CALTS w;
    w.showMaximized();
    return a.exec();
}


