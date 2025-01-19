#include "RegisterWindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <cpr/cpr.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>


RegisterWindow::RegisterWindow(QWidget* parent) :QMainWindow(parent)
{
    setupUI();
}

RegisterWindow::~RegisterWindow()
{
}

void RegisterWindow::setupUI()
{
    setFixedSize(400, 300);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    QLabel* usernameLabel = new QLabel("Nume utilizator:", this);
    layout->addWidget(usernameLabel);

    usernameEdit = new QLineEdit(this);
    layout->addWidget(usernameEdit);

    registerButton = new QPushButton("Inregistreaza-te", this);
    layout->addWidget(registerButton);

    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterButtonClicked);

    centralWidget->setStyleSheet("QLineEdit { font-size: 16px; padding: 8px; border-radius: 4px; background-color: white; color: black }"
        "QPushButton { font-size: 16px; padding: 10px; border-radius: 4px; background-color: #3498db; color: white; }"
        "QPushButton:hover { background-color: #2980b9; }");
}

bool RegisterWindow::validateInputs()
{
    if (usernameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Eroare", "Toate câmpurile trebuie completate!");
        return false;
    }

    QRegularExpression usernameRegex("^[A-Za-z][A-Za-z0-9]*$");
    QRegularExpressionMatch match = usernameRegex.match(usernameEdit->text());
    if (!match.hasMatch()) {
        QMessageBox::warning(this, "Eroare", "Numele utilizatorului trebuie să înceapă cu o literă și să nu conțină caractere speciale!");
        return false;
    }

    return true;
}

void RegisterWindow::onRegisterButtonClicked() {
    if (validateInputs()) {
        QString playerName = usernameEdit->text();
        int playerPoints = 300; 
        int playerId = generateUniqueId();

        if (sendPlayerDataToServer(playerId, playerName, playerPoints)) {
            QMessageBox::information(this, "Success", "Utilizator înregistrat cu succes!");
            this->close();
        }
    }
}
int RegisterWindow::generateUniqueId()
{
    static int nextId = 1;
    return nextId++;
}

bool RegisterWindow::sendPlayerDataToServer(int playerId, const QString& playerName, int playerPoints) {
    
    QString jsonString = QString("{\"id\": %1, \"name\": \"%2\", \"points\": %3}")
        .arg(playerId)
        .arg(playerName)
        .arg(playerPoints);

    
    cpr::Response response = cpr::Post(
        cpr::Url{ base_url+"/register" },
        cpr::Body{ jsonString.toStdString() },
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    
    if (response.status_code == 200) {
      
        auto responseText = QString::fromStdString(response.text);
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseText.toUtf8());
        QJsonObject responseObj = responseDoc.object();

        if (responseObj["status"].toString() == "success") {
            return true;
        }
        else {
            QMessageBox::critical(nullptr, "Server Error", responseObj["error"].toString());
            return false;
        }
    }
        else if (response.status_code == 409)
        {
        QMessageBox::critical(nullptr, "Error",
            QString("User already exists: %1").arg(response.status_code));
        return false;
        }
    else {
        QMessageBox::critical(nullptr, "Error",
            QString("Failed to connect to server: %1").arg(response.status_code));
        return false;
    }
}