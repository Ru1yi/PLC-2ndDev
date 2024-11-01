#include "PlcControl/PlcControl.h"

int main(int argc, char* argv[])
{
	GlobalPlc = new PlcControl();

	try {
		bool ret = GlobalPlc->PlcConnectInit();

		GlobalPlc->DBWrite_Int(1080, 264, 0);

		//int xintiao = GlobalPlc->DBRead_Int(1080, 268);

		//GlobalPlc->DBWrite_String(1080, 0, "BYD12345", 50);

		//std::string xintiao = GlobalPlc->DBRead_String(1080, 0);
	}
	catch (std::exception& e)
	{

	}

	return 0;
}
