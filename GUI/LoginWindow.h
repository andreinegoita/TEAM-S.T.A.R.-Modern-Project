#define LOGINWINDOW_H
#include <QMainWindow>

class QlineEdit;
class QPushButton;
class QLabel;


class LoginWindow: public QMainWindow
{
	Q_OBJECT
public:
	explicit LoginWindow(QWidget* parent = nullptr);
	~LoginWindow();

private:
	QLabel* backgroundLabel;
	QlineEdit* usernameEdit;
	QlineEdit* passwordEdit;
	QPushButton* loginButton;
	QPushButton* registerButton;

	void setupUI();
};
