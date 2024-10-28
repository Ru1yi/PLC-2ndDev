#pragma once
#include <Windows.h>
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QSettings>
#include <QDebug>

#include <iostream>
//#include <ctime>
#include "../Model/Model.h"
#include "snap7.h"
#pragma comment(lib, "snap7.lib")

class PlcControl : public QObject
{
	Q_OBJECT

public:
	PlcControl(QObject* parent);
	~PlcControl();
	bool isConnect = false;
	bool PlcConnectInit();
	int DBRead_Int(int DBNumber, int Start);
	bool DBWrite_Int(int DBNumber, int Start, int IntValue);
	std::string DBRead_String(int DBNumber, int Start, int PlcStringLength);
	bool DBWrite_String(int DBNumber, int Start, std::string StrVal, int PlcStringLength);


public slots:

private:
	TS7Client* mClient;
	QTimer* timer;

	int rack;
	int slot;
	QString ip_qstr;
	
signals:
	void SN_SendResult(QStringList SNs);
};

extern PlcControl* GlobalPlc;

