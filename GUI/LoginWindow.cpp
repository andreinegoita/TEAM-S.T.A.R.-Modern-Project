#include "LoginWindow.h"
#include <QvBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include "RegisterWindow.h"
#include <QFile>
#include <QtSql/QSqlQuery>
#include <QtSQl/QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <cpr/cpr.h>

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
    setFixedSize(1600, 900);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    
    QString backgroundImagePath = "battleCity.jpg";
    QFile file(backgroundImagePath);
    if (!file.exists()) {
        qDebug() << "Image can't be processed: " << backgroundImagePath;
    }
    else {
        QPalette palette;
        QPixmap backgroundImage(backgroundImagePath);
        palette.setBrush(QPalette::Window, QBrush(backgroundImage));
        centralWidget->setPalette(palette);
        centralWidget->setAutoFillBackground(true);
    }

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Introduceti numele de utilizator");
    usernameEdit->setStyleSheet("QLineEdit { "
        "background-color: white; "
        "color: black; "
        "border: 2px solid #333; "
        "border-radius: 4px; }");
    layout->addWidget(usernameEdit);


    loginButton = new QPushButton("Conectare", this);
    loginButton->setStyleSheet("font-size: 16px; padding: 10px; border-radius: 4px; background-color: cyan; color: black;");
    layout->addWidget(loginButton);

    registerButton = new QPushButton("Inregistrare", this);
    registerButton->setStyleSheet("font-size: 16px; padding: 10px; border-radius: 4px; background-color: cyan; color: black;");
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
    QString playerName = usernameEdit->text().trimmed();
    if (playerName.isEmpty()) {
        qDebug() << "Numele de utilizator este gol. Introduceti un nume valid.";
        QMessageBox::warning(this, "Eroare", "Numele de utilizator nu poate fi gol.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT name, points FROM players WHERE name = :name");
    query.bindValue(":name", playerName);

    if (!query.exec()) {
        qDebug() << "Error executing query: " << query.lastError().text();
        QMessageBox::critical(this, "Eroare", "Nu s-a putut verifica utilizatorul in baza de date.");
        return;
    }

    if (query.next()) {
        QString fetchedName = query.value(0).toString();
        int playerPoints = query.value(1).toInt();

        qDebug() << "Player found: Name=" << fetchedName << ", Points=" << playerPoints;

        
        std::string payload = R"({"name":")" + fetchedName.toStdString() + R"(","points":)" + std::to_string(playerPoints) + R"(})";

      
        cpr::Response response = cpr::Post(
            cpr::Url{ base_url + "/login" },
            cpr::Body{ payload },
            cpr::Header{ { "Content-Type", "application/json" } }
        );

        if (response.status_code == 200) {
            qDebug() << "Player logged in successfully: " << response.text.c_str();
            emit loginSuccessful(fetchedName);
            this->close();
        }
        else if (response.status_code == 409) {
            qDebug() << "Player already logged in!";
            QMessageBox::warning(this, "Eroare autentificare", "Utilizatorul este deja autentificat.");
        }
        else if (response.status_code == 404) {
            qDebug() << "Player not found on server: " << response.text.c_str();
            QMessageBox::warning(this, "Eroare autentificare", "Utilizatorul nu există pe server.");
        }
        else {
            qDebug() << "Error logging in: " << response.text.c_str();
            QMessageBox::critical(this, "Eroare server", "Nu s-a putut autentifica utilizatorul.");
        }
    }
    else {
        qDebug() << "Playerul nu a fost gasit in baza de date.";
        QMessageBox::warning(this, "Eroare autentificare", "Utilizatorul nu exista in baza de date.");
    }
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