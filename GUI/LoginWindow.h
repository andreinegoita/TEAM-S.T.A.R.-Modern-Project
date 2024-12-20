#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QMainWindow>

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

	void setupUI();
	void onLoginClicked();

signals:
	void loginSuccessful();
};

#endif