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
    // TODO If valid, atutentificate logic
    QString playerName = usernameEdit->text().trimmed();
    if (playerName.isEmpty()) {
        qDebug() << "Numele de utilizator este gol. Introduceti un nume valid.";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id,points FROM players WHERE name = :name");
    query.bindValue(":name", playerName);

    if (!query.exec()) {
        qDebug() << "Error executing query: " << query.lastError().text();
        QMessageBox::critical(this, "Eroare", "Nu s-a putut verifica utilizatorul in baza de date.");
        return;
    }
    if (query.next()) { // Găsește utilizatorul
        int playerId = query.value(0).toInt(); // id-ul jucătorului
        int playerPoints = query.value(1).toInt(); // punctele jucătorului

        qDebug() << "Player found: ID=" << playerId << ", Points=" << playerPoints;

        // Trimite datele către server folosind CPR
        cpr::Response response = cpr::Get(
            cpr::Url{ base_url+"/login" }, // URL-ul serverului tău
            cpr::Body{ R"({"id": ")" + std::to_string(playerId) + R"(", "name": ")" + playerName.toStdString() + R"(", "points": ")" + std::to_string(playerPoints) + R"("})" },
            cpr::Header{ {"Content-Type", "application/json"} }
        );


        if (response.status_code == 200) {
            qDebug() << "Player logged in successfully: " << response.text.c_str();
            emit loginSuccessful(playerName);
            this->close();
        }
		else if (response.status_code == 409) {
			qDebug() << "Player already logged in!";
			QMessageBox::warning(this, "Eroare autentificare", "Utilizatorul este deja autentificat.");
		}
        else {
            qDebug() << "Error logging in: " << response.text.c_str();
            QMessageBox::critical(this, "Eroare server", "Nu s-a putut autentifica utilizatorul.");
        }
    }
    else {
        // Playerul nu există
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