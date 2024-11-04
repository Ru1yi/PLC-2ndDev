#pragma once

#include <iostream>
#include <vector>
#include <mutex>

#include "snap7.h"
#include "inih-r58/cpp/INIReader.h"
#include "../IVT/IVTException.h"

#pragma comment(lib, "snap7.lib")

class PlcControl 
{
public:
	PlcControl();
	~PlcControl();
	bool isConnect = false;
	bool PlcConnectInit();
	int DBRead_Int(int DBNumber, int Start);
	bool DBWrite_Int(int DBNumber, int Start, int IntValue);
	std::string DBRead_String(int DBNumber, int Start);
	bool DBWrite_String(int DBNumber, int Start, std::string StrVal, int maxLength);

private:
	TS7Client* mClient;
	std::mutex m_mtx;

	int rack;
	int slot;
	std::string ip_str;
	
};

extern PlcControl* GlobalPlc;

