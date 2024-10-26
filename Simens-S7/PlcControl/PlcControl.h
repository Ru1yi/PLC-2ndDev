#pragma once
#include <Windows.h>
#include <QObject>
#include <QTimer>
#include <QThread>
#include <iostream>
#include <random>
#include <ctime>
#include "../Model/Model.h"
#include "snap7.h"
#pragma comment(lib, "snap7.lib")	//	可写在链接器输入中替代本行代码

class PlcControl : public QObject
{
	Q_OBJECT

public:
	PlcControl(QObject* parent);
	~PlcControl();
	bool isConnect;
	int Read_DB = 1;
	bool Read_PlcHead = false;
	bool PlcConnectInit();
	int Read_IntDB(int DBNumber, int Start);
	bool Write_IntDB(int DBNumber, int Start, int IntValue);
	std::string Read_StringDB(int DBNumber, int Start, int PlcStringLength);
	bool Write_StringDB(int DBNumber, int Start, std::string StrVal, int PlcStringLength);

	//库有缺陷，往plc string DB块中写string值时，会出现写入混乱，若要写入string 值给plc，需要写入char DB块，然后plc将char 拼接成string.
	bool Write_CharDB(int DBNumber, int Start, std::string StrVal);


public slots:

private:
	TS7Client* mClient;
	QTimer* timer;
	//static const char ip[];
	int rack;
	int slot;
signals:
	void SN_SendResult(QStringList SNs);
};

extern PlcControl* GlobalPlc;

