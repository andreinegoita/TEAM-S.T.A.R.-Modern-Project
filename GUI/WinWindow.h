#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class WinWindow : public QWidget {
    Q_OBJECT

public:
    explicit WinWindow(QWidget* parent = nullptr);
};