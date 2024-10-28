#include "plc_test.h"
#include <QtWidgets/QApplication>
#include "PlcControl/PlcControl.h"

int main(int argc, char *argv[])
{
   GlobalPlc = new PlcControl();
    bool ret = GlobalPlc->PlcConnectInit();

    GlobalPlc->DBWrite_Int(1080, 260, 25);

    //int xintiao = GlobalPlc->DBRead_Int(1080, 260);
    
    //GlobalPlc->DBWrite_String(1080, 0, "BYD12345", 50);

    //std::string xintiao = GlobalPlc->DBRead_String(1080, 0);

    return 0;
}
