#include "GUI.h"
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
GUI::GUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

GUI::~GUI()
{}
