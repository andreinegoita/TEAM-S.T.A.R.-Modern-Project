#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include<QVector>
#include <cpr/cpr.h>
#include<QKeyEvent>
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

private slots:
    void updateGraphics();
    bool canMoveTo(float newX, float newY);
    void updatePlayerTexture(const QString& direction);

private:
    QLabel* playerLabel;
    QGridLayout* gridLayout;
    float m_targetX, m_targetY;
    float m_x ,m_y;
    float m_currentSpeedX, m_currentSpeedY;
    float m_speed;


    void setupUI();

    void fetchMap();

    int m_mapWidth;
    int m_mapHeight;

    QVector<QVector<QString>> m_mapData;


};