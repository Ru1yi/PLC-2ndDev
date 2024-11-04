#include "PlcControl/PlcControl.h"

int main(int argc, char* argv[])
{
	GlobalPlc = new PlcControl();

	try {
		bool ret = GlobalPlc->PlcConnectInit();

		GlobalPlc->DBWrite_Int(1080, 262, 1);

		/*int xintiao = GlobalPlc->DBRead_Int(1080, 262);
		std::cout << "Read int: " << xintiao << "\n";*/

		//GlobalPlc->DBWrite_String(1080, 0, "BYD12345", 50);

		//std::string sn = GlobalPlc->DBRead_String(1080, 0);
		//std::cout << "Read string: " << sn << "\n";
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
