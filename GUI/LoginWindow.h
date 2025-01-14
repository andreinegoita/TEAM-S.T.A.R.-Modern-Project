#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QMainWindow>
#include "MainMenuWindow.h"
#include<string>

class QLineEdit;
class QPushButton;
class QLabel;


class LoginWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit LoginWindow(QWidget* parent = nullptr);
	~LoginWindow();

private:
	QLabel* backgroundLabel;
	QLineEdit* usernameEdit;
	QLineEdit* passwordEdit;
	QPushButton* loginButton;
	QPushButton* registerButton;
	MainMenuWindow* mainMenuWindow;
	void setupUI();
	void onLoginClicked();
	std::string base_url = "http://localhost:18080";


private slots:
	void openMainMenuWindow();

signals:
	void loginSuccessful(const QString& playerName);
};

#endif