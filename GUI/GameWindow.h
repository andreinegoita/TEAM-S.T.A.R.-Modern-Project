#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include<qvector.h>
#include <cpr/cpr.h>
#include<qgraphicsscene.h>
#include<qgraphicsview.h>

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    GameWindow(QWidget* parent = nullptr);

public:
    void displayMap(const QJsonArray& mapArray);

private:
    QGridLayout* gridLayout;
    QGraphicsScene* scene;
    QGraphicsView* view;

    float m_targetX, m_targetY;
    float m_x ,m_y;

    void setupUI();

    void fetchMap();

    int m_mapWidth;
    int m_mapHeight;

    QVector<QVector<QString>> m_mapData;
};