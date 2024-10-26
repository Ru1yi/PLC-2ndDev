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
#pragma comment(lib, "snap7.lib")	//	��д��������������������д���

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

	//����ȱ�ݣ���plc string DB����дstringֵʱ�������д����ң���Ҫд��string ֵ��plc����Ҫд��char DB�飬Ȼ��plc��char ƴ�ӳ�string.
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

