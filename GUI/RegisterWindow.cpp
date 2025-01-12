import RegisterWindow;

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

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
        // TODO: Add Player in database
        QMessageBox::information(this, "Succes", "Utilizator inregistrat cu succes!");
        this->close();
    }
}
