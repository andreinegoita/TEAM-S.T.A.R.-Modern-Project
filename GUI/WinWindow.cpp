#include "WinWindow.h"

WinWindow::WinWindow(QWidget* parent) : QWidget(parent)
{
    setFixedSize(400, 200);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* messageLabel = new QLabel("YOU WIN!", this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: green;");

    QPushButton* closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &WinWindow::close);

    layout->addWidget(messageLabel);
    layout->addWidget(closeButton);
}
