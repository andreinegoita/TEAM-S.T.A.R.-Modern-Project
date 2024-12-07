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

private:
    QGridLayout* gridLayout;
    QGraphicsScene* scene;
    QGraphicsView* view;

    void setupUI();

    void fetchMap();

    int mapWidth;
    int mapHeight;

    QVector<QVector<QString>> mapData;
public:
    void displayMap(const QJsonArray& mapArray);

};