#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>

class QLineEdit;
class QPushButton;

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget* parent = nullptr);
    ~RegisterWindow();

private slots:
    void onRegisterButtonClicked();

private:
    QLineEdit* usernameEdit;
    QPushButton* registerButton;

    void setupUI();
    bool validateInputs();
};

#endif
