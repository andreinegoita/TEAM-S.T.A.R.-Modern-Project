#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include<QVector>
#include <cpr/cpr.h>
#include<QKeyEvent>
#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QJsonObject>
#include<QDebug>
#include <QtConcurrent/QtConcurrent>
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS


class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    GameWindow(QWidget* parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
public:
    void displayMap(const QJsonArray& mapArray);
    void updateMap(const QJsonArray& mapArray);
    void returnToMainMenu();

private slots:
    void updateGraphics();
    bool canMoveTo(float newX, float newY);
    void updatePlayerTexture(const QString& direction);
    void addBullet(float x, float y, int direction);
    void updateBullets();
    void destroyCells(int x, int y);
private:
    QLabel* positionLabel;
    QLabel* playerLabel;
    QGridLayout* gridLayout;
    float m_targetX, m_targetY;
    float m_x ,m_y;
    float m_currentSpeedX, m_currentSpeedY;
    float m_speed;
    float m_bulletSpeed;
struct m_bulletData {
    float x;
    float y;
    uint8_t direction;
};

    void setupUI();
    void shootBullet();
    void fetchMap();
    void fetchPlayerPosition();
    void updateServerPlayerPosition();
    void displayPlayerPosition(int x, int y);
    void updateServerBulletsPosition();
    void updateServerMapCell(int row, int col);
    void setPlayerStartPosition();
    int m_mapWidth;
    int m_mapHeight;
    uint8_t m_direction;
    QTimer* bulletCooldownTimer;
    bool canShoot;
    std::string m_playerName;
    QJsonArray m_mapArray;
    QVector<QVector<QString>> m_mapData;
    QVector<m_bulletData> bullets;
    QVector<QLabel*> bulletLabels;

};