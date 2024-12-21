#include "LoginWindow.h"
#include <QvBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include "RegisterWindow.h"
#include <QFile>

LoginWindow::LoginWindow(QWidget* parent) :QMainWindow(parent)
{
	setupUI();
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
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

    
    QString backgroundImagePath = "battleCity.jpg";
    QFile file( backgroundImagePath );
    if (!file.exists()) {
        qDebug() << "Imaginea de fundal nu a fost gasita: " << backgroundImagePath;
    }
    else {
        centralWidget->setStyleSheet(QString(
            "QWidget {"
            "    background-image: url('%1');"
            "    background-position: center;"
            "    background-repeat: no-repeat;"
            "    background-size: cover;"
            "}"
        ).arg(backgroundImagePath));
    }

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Introduceti numele de utilizator");
    layout->addWidget(usernameEdit);

    loginButton = new QPushButton("Conectare", this);
    loginButton->setStyleSheet("font-size: 16px; padding: 10px; border-radius: 4px; background-color: white; color: black;");
    layout->addWidget(loginButton);

    registerButton = new QPushButton("Inregistrare", this);
    registerButton->setStyleSheet("font-size: 16px; padding: 10px; border-radius: 4px; background-color: white; color: black;");
    layout->addWidget(registerButton);

    layout->setAlignment(Qt::AlignCenter);


    usernameEdit->setGeometry(500, 300, 200, 50);
    loginButton->setGeometry(500, 400, 200, 50);
    registerButton->setGeometry(500, 500, 200, 50);


    connect(registerButton, &QPushButton::clicked, this, []() {
        RegisterWindow* registerWindow = new RegisterWindow();
        registerWindow->show();
        });
}

void LoginWindow::onLoginClicked() {
    // TODO If valid, atutentificate logic
    QString playerName = usernameEdit->text().trimmed();
    if (playerName.isEmpty()) {
        qDebug() << "Numele de utilizator este gol. Introduceti un nume valid.";
        return;
    }
    emit loginSuccessful(playerName);
    this->close();

}

void LoginWindow::openMainMenuWindow() {
    if (!mainMenuWindow) {
        mainMenuWindow = new MainMenuWindow();
        QString playerName = usernameEdit->text();
        emit loginSuccessful(playerName);
        mainMenuWindow->show();
    }
    close();
}