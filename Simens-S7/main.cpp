#include "plc_test.h"
#include <QtWidgets/QApplication>
#include "PlcControl/PlcControl.h"

int main(int argc, char *argv[])
{
   /* QApplication a(argc, argv);
    PLC_TEST w;
    w.show();
    return a.exec();*/
    
    GlobalPlc = new PlcControl(NULL);
    bool ret = GlobalPlc->PlcConnectInit();

    //GlobalPlc->Write_IntDB(1080, 0, 99);

    //int xintiao = GlobalPlc->Read_IntDB(1080, 0);
    
    //GlobalPlc->Write_StringDB(1080, 0, "BYD123", 6);

    std::string xintiao = GlobalPlc->Read_StringDB(1080, 0, 50);

    return 0;
}
