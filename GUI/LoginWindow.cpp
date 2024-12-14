#include "LoginWindow.h"
#include <QvBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

LoginWindow::LoginWindow(QWidget* parent)
{
	setupUI();
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::setupUI()
{
    setFixedSize(1200, 900);

	QWidget* centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    backgroundLabel = new QLabel(this);
    QPixmap backgroundPixmap("battleCity.jpg");

    if (backgroundPixmap.isNull()) {
        qDebug() << "Image couldn't load with success";
        return;
    }

    backgroundLabel->setPixmap(backgroundPixmap.scaled(1200, 1000, Qt::KeepAspectRatio));
   
    backgroundLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(backgroundLabel);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Introduceti numele de utilizator");
    layout->addWidget(usernameEdit);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Introduceti parola");
    passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordEdit);

    loginButton = new QPushButton("Conectare", this);
    layout->addWidget(loginButton);

    registerButton = new QPushButton("Inregistrare", this);
    layout->addWidget(registerButton);

    centralWidget->setStyleSheet("QWidget { background-color: #2c3e50; }"
        "QLineEdit { font-size: 16px; padding: 8px; border-radius: 4px; background-color: white; color: black }"
        "QPushButton { font-size: 16px; padding: 10px; border-radius: 4px; background-color: #3498db; color: white; }"
        "QPushButton:hover { background-color: #2980b9; }");
}