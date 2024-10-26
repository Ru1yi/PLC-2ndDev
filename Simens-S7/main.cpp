#include "plc_test.h"
#include <QtWidgets/QApplication>
#include "PlcControl/PlcControl.h"

int main(int argc, char *argv[])
{
   GlobalPlc = new PlcControl(NULL);
    bool ret = GlobalPlc->PlcConnectInit();

    //GlobalPlc->DBWrite_Int(1080, 0, 99);

    int xintiao = GlobalPlc->DBRead_Int(1080, 256);
    
    //GlobalPlc->DBWrite_String(1080, 0, "BYD123", 6);

    //std::string xintiao = GlobalPlc->DBRead_String(1080, 0, 50);

    return 0;
}
