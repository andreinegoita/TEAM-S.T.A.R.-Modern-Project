#include "RegisterWindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


RegisterWindow::RegisterWindow(QWidget* parent):QMainWindow(parent)
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
    if (usernameEdit->text().isEmpty() ) {
        QMessageBox::warning(this, "Eroare", "Toate campurile trebuie completate!");
        return false;
    }

    return true;
}

void RegisterWindow::onRegisterButtonClicked()
{
    if (validateInputs()) {
        QString playerName = usernameEdit->text(); // Assume you have a QLineEdit for the name
        int playerPoints = 300; // Default points for a new player
        int playerId = generateUniqueId(); // Create a method to generate unique IDs

        QSqlDatabase db = QSqlDatabase::database(); // Assuming a database connection is already established
        if (!db.isOpen()) {
            QMessageBox::critical(this, "Database Error", "Database connection is not open.");
            return;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO players (id, name, points) VALUES (:id, :name, :points)");
        query.bindValue(":id", playerId);
        query.bindValue(":name", playerName);
        query.bindValue(":points", playerPoints);

        if (query.exec()) {
            QMessageBox::information(this, "Success", "Utilizator inregistrat cu succes!");
            this->close();
        }
        else {
            QMessageBox::critical(this, "Database Error", query.lastError().text());
        }
    }
}

int RegisterWindow::generateUniqueId()
{
    static int nextId = 1;
    return nextId++;
}