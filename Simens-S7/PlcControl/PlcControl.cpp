#include "PlcControl.h"
#pragma execution_character_set("utf-8")

QStringList SNList;
PlcControl* GlobalPlc;

PlcControl::PlcControl(QObject* parent)
	: QObject(parent)
{
	// Init plc params
	QSettings settings("PlcControl/config.ini", QSettings::IniFormat);
	ip_qstr = settings.value("PLC/ip", "localhost").toString();
	rack = settings.value("PLC/rack", "9999").toInt();
	slot = settings.value("PLC/slot", "9999").toInt();

	qDebug() << "---------PLC params init successfully!---------\n"
		<< "[ip]: " << ip_qstr << "\n"
		<< "[rack]: " << rack << "\n"
		<< "[slot]: " << slot;;

	mClient = new TS7Client;
}

PlcControl::~PlcControl()
{
	delete mClient;
}

bool PlcControl::PlcConnectInit()
{
	mClient->Disconnect();

	QByteArray byteArray = ip_qstr.toUtf8();
	const char* ip_cstr = byteArray.constData();

	if (mClient->ConnectTo(ip_cstr, rack, slot) == 0) {	// if success return 0
		isConnect = true;
		qDebug() << "PLC connect successfully!";
	}
	else
		qDebug() << "PLC connect failed!";
	return isConnect;
}

int PlcControl::DBRead_Int(int DBNumber, int Start)
{
	int IntValue = -1;
	byte res[256] = { 0 };
	// byte to int
	if (0 == mClient->DBRead(DBNumber, Start, 2, &res))
	{
		uint value = res[1] & 0xFF;  // Int occupies 2 bytese in PLC
		value |= ((res[0] << 8) & 0xFF00);
		IntValue = value;
	}
	return IntValue;
}

bool PlcControl::DBWrite_Int(int DBNumber, int Start, int IntValue)
{
	bool ret = false;
	byte data[2] = { 0 };
	data[1] = (unsigned char)(0xff & IntValue);
	data[0] = (unsigned char)((0xff00 & IntValue) >> 8);
	if (0 == mClient->DBWrite(DBNumber, Start, 2, &data))
		ret = true;
	return ret;
}

std::string PlcControl::DBRead_String(int DBNumber, int Start, int PlcStringLength)
{
	//读取string 类型plc DB块  +2（因为plc中前两个会有换行符）s
	char* test_string = (char*)malloc(PlcStringLength * sizeof(char));
	int result = mClient->DBRead(DBNumber, Start, PlcStringLength + 2, test_string);
	char* charBuffer = reinterpret_cast<char*>(test_string + 2);
	*(charBuffer + PlcStringLength) = '\0';
	std::string te(charBuffer);
	//free(test_string);
	return te;
}

bool PlcControl::DBWrite_String(int DBNumber, int Start, std::string StrVal, int PlcStringLength)
{
	int statue1 = -1;
	int statue2 = -1;
	int statue3 = -1;
	bool ret = false;
	char* buf;
	int len_ = StrVal.length();
	buf = (char*)malloc((len_ + 1) * sizeof(char));
	StrVal.copy(buf, len_, 0);
	*(buf + len_) = '\0';
	int StringLength = StrVal.length();
	BYTE string_size;
	string_size = StringLength;
	BYTE maxsize = PlcStringLength;
	statue1 = mClient->DBWrite(DBNumber, Start, 1, &maxsize);   //第0位总长度
	statue2 = mClient->DBWrite(DBNumber, Start + 1, 1, &string_size);  //第1位有效长度
	statue3 = mClient->DBWrite(DBNumber, Start + 2, StringLength, buf);
	if (statue1 == 0 && statue2 == 0 && statue3 == 0)
		ret = true;
	return ret;
}

