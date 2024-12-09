#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include<qvector.h>
#include <cpr/cpr.h>
#include<qgraphicsscene.h>
#include<qgraphicsview.h>
#include<QKeyEvent>


class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    GameWindow(QWidget* parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent* event) override;
public:
    void displayMap(const QJsonArray& mapArray);

private:
    QGridLayout* gridLayout;
    QGraphicsScene* scene;
    QGraphicsView* view;

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