#include "PlcControl.h"
#pragma execution_character_set("utf-8")

QStringList SNList;
PlcControl* GlobalPlc;

PlcControl::PlcControl(QObject* parent)
	: QObject(parent)
{
	//const char ip[] = "192.168.30.228";//PLC-IP��ַ
	rack = 0;//���ܺ�
	slot = 1;  //�ۺ�
	isConnect = false;
	mClient = new TS7Client;
}

PlcControl::~PlcControl()
{
	delete mClient;
}

bool PlcControl::PlcConnectInit()
{
	int status = -1;
	mClient->Disconnect();
	char ip[] = "192.168.10.190";//PLC-IP��ַ
	//status = mClient->ConnectTo(ip, rack, slot);//���ӳɹ����أ�0

	if (mClient->ConnectTo(ip, rack, slot) == 0)	//	if success return 0
	{
		isConnect = true;
	}
	else
	{
		isConnect = false;
	}
	return isConnect;
}

int PlcControl::Read_IntDB(int DBNumber,int Start)
{
	//��ȡplc db����  int��  
	int IntValue = -1;
	byte res[256] = { 0 };
	int status = mClient->DBRead(DBNumber, Start, 2, &res);
	//byte[]תint
	if (status == 0)
	{
		uint value = res[1] & 0xFF;  //PLC��int����ռ2�ֽڳ���
		value |= ((res[0] << 8) & 0xFF00);
		IntValue = value;
	}
	return IntValue;
}

bool PlcControl::Write_IntDB(int DBNumber, int Start, int IntValue)
{
	//д��plc db�� int��
	//int test_value = 203;
	bool CheckResult = false;
	int status = -1;
	byte data[2] = { 0 };
	data[1] = (unsigned char)(0xff & IntValue);
	data[0] = (unsigned char)((0xff00 & IntValue) >> 8);
	status = mClient->DBWrite(DBNumber, Start, 2, &data);
	if (status == 0)
	{
		CheckResult = true;
	}
	else
	{
		CheckResult = false;
	}
	return CheckResult;
}

std::string PlcControl::Read_StringDB(int DBNumber, int Start,int PlcStringLength)
{
	//��ȡstring ����plc DB��  15��plc�ж�����������ͳ��� 17���������ͳ���+2����Ϊplc��ǰ�������л��з���
	char* test_string = (char*)malloc(PlcStringLength * sizeof(char));
	int result = mClient->DBRead(DBNumber, Start, PlcStringLength+2 , test_string);
	char* charBuffer = reinterpret_cast<char*>(test_string + 2);
	*(charBuffer + PlcStringLength) = '\0';
	std::string te(charBuffer);
	//free(test_string);
	return te;
}

bool PlcControl::Write_StringDB(int DBNumber, int Start, std::string StrVal, int PlcStringLength)
{
	int statue1 = -1;
	int statue2 = -1;
	int statue3 = -1;
	bool CheckResult = false;
	char* buf;
	int len_ = StrVal.length();
	buf = (char*)malloc((len_ + 1) * sizeof(char));
	StrVal.copy(buf, len_, 0);
	*(buf + len_) = '\0';
	int StringLength = StrVal.length();
	BYTE string_size;
	string_size = StringLength;
	BYTE maxsize = PlcStringLength;
	statue1 = mClient->DBWrite(DBNumber, Start, 1, &maxsize);   //��0λ�ܳ���
	statue2 = mClient->DBWrite(DBNumber, Start + 1, 1, &string_size);  //��1λ��Ч����
	statue3 = mClient->DBWrite(DBNumber, Start + 2, StringLength, buf);
	if (statue1 == 0 && statue2 == 0 && statue3 == 0)
	{
		CheckResult = true;
	}
	else {
		CheckResult = false;
	}
	return CheckResult;
}


bool PlcControl::Write_CharDB(int DBNumber, int Start, std::string StrVal)
{
	// Convert integer to bytes in big-endian format
	bool CheckResult = false;
	int statue = -1;
	int value1 = 254;
	std::vector<unsigned char> bytes1;
	bytes1.push_back(static_cast<unsigned char>(value1));
	// Get length of the string and convert it to bytes in big-endian format
	int length = StrVal.length();
	std::vector<unsigned char> bytes2;
	bytes2.push_back(static_cast<unsigned char>(length));
	// Convert string to bytes using ASCII encoding
	std::vector<unsigned char> bytes3(StrVal.begin(), StrVal.end());
	// Concatenate all byte vectors
	std::vector<unsigned char> finalBytes;
	finalBytes.clear();
	finalBytes.insert(finalBytes.end(), bytes1.begin(), bytes1.end());
	finalBytes.insert(finalBytes.end(), bytes2.begin(), bytes2.end());
	finalBytes.insert(finalBytes.end(), bytes3.begin(), bytes3.end());
	for (int i = 2; i < finalBytes.size(); i++)
	{
		statue = mClient->DBWrite(DBNumber, Start - 2 + i, 1, &finalBytes[i]);//2+iΪ��Ӧ����ƫ����Ϊ4�������װ��Ҫ�޸ġ�
	}
	if (statue == 0)
	{
		CheckResult = true;
	}else {
		CheckResult = false;
	}
	return CheckResult;
}

