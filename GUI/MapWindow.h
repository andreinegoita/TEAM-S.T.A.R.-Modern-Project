#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include <cpr/cpr.h>
#include<qgraphicsscene.h>
#include<qgraphicsview.h>

class MapWindow : public QMainWindow {
    Q_OBJECT

public:
    MapWindow(QWidget* parent = nullptr);

private:
    QGridLayout* gridLayout;
    QGraphicsScene* scene;
    QGraphicsView* view;

    void setupUI();

    void fetchMap();
public:
    void displayMap(const QJsonArray& mapArray);

};