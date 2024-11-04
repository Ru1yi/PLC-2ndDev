#include "PlcControl.h"
#pragma execution_character_set("utf-8")

PlcControl* GlobalPlc;

PlcControl::PlcControl()
{
	// Init plc params
	INIReader reader("PlcControl/config.ini");
	if (reader.ParseError() < 0)
		throw INIParseException();
	ip_str = reader.Get("PLC", "ip", "localhost");
	rack = reader.GetInteger("PLC", "rack", 9999);
	slot = reader.GetInteger("PLC", "slot", 9999);

	std::cout << "---------PLC params init successfully!---------\n"
		<< "[ip]: " << ip_str << "\n"
		<< "[rack]: " << rack << "\n"
		<< "[slot]: " << slot << "\n";

	mClient = new TS7Client;
}

PlcControl::~PlcControl()
{
	delete mClient;
}

bool PlcControl::PlcConnectInit()
{
	mClient->Disconnect();

	const char* ip_cstr = ip_str.c_str();
	if (0 == mClient->ConnectTo(ip_cstr, rack, slot)) 	// if success return 0
		isConnect = true;
	else
		throw PLCException('c');
	return isConnect;
}

int PlcControl::DBRead_Int(int DBNumber, int Start)
{
	std::lock_guard<std::mutex> lock(m_mtx);

	byte res[2] = { 0 };
	if (mClient->DBRead(DBNumber, Start, sizeof(res), res) == 0)
		return static_cast<int>((static_cast<unsigned int>(res[0]) << 8) | (res[1] & 0xFF));
	else
		throw PLCException('r');
	return -1;
}

bool PlcControl::DBWrite_Int(int DBNumber, int Start, int IntValue)
{
	std::lock_guard<std::mutex> lock(m_mtx);

	byte data[2] = {
	  static_cast<byte>((IntValue >> 8) & 0xFF), // high byte
	  static_cast<byte>(IntValue & 0xFF)         // low byte
	};

	if (0 == mClient->DBWrite(DBNumber, Start, sizeof(data), data))
		return true;
	else {
		throw PLCException('w');
		return false;
	}
}

std::string PlcControl::DBRead_String(int DBNumber, int Start)
{
	std::lock_guard<std::mutex> lock(m_mtx);

	byte buf[2];
	std::string out;
	int ret = mClient->DBRead(DBNumber, Start, 2, buf);	// 0 means success
	if (0 != ret) {
		out = std::string();
		throw PLCException('r');
	}

	if (buf[0] < buf[1] || buf[0] > 254 || buf[1] > 254) {
		out = std::string();
		throw PLCException('d');
	}

	std::vector<char> buf2(buf[1] + 1, 0);
	ret = mClient->DBRead(DBNumber, Start + 2, buf[1], buf2.data());
	if (0 == ret) {
		out.assign(buf2.data(), buf[1]);
	}
	else
		throw PLCException('r');
	return out;
}

bool PlcControl::DBWrite_String(int DBNumber, int Start, std::string in, int maxLength)
{
	std::lock_guard<std::mutex> lock(m_mtx);

	std::vector<byte> buf(in.length() + 2);
	buf[0] = static_cast<byte>(maxLength);
	buf[1] = static_cast<byte>(in.length());
	std::copy(in.begin(), in.end(), buf.begin() + 2);

	int ret = mClient->DBWrite(DBNumber, Start, buf.size(), buf.data());

	if (0 == ret)
		return true;
	else {
		throw PLCException('w');
		return false;
	}
}

