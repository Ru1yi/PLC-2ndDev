#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_plc_test.h"

class PLC_TEST : public QMainWindow
{
    Q_OBJECT

public:
    PLC_TEST(QWidget *parent = Q_NULLPTR);

private:
    Ui::PLC_TESTClass ui;
};
