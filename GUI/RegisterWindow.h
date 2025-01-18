#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H
#include<string>
#include <QMainWindow>
#include <QRegularExpression>
#include <QMessageBox>


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
    std::string base_url = "http://localhost:18080";
    void setupUI();
    bool validateInputs();
    int generateUniqueId();
    bool sendPlayerDataToServer(int playerId, const QString& playerName, int playerPoints); 

};

#endif
