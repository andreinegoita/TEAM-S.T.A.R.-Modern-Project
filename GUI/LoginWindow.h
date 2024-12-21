#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QMainWindow>
#include "MainMenuWindow.h"

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


private slots:
	void openMainMenuWindow();

signals:
	void loginSuccessful(const QString& playerName);
};

#endif