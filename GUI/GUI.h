#pragma once
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <QtWidgets/QMainWindow>
#include "ui_GUI.h"

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    GUI(QWidget *parent = nullptr);
    ~GUI();

private:
    Ui::GUIClass ui;
};
