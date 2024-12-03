#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include <cpr/cpr.h>

class MapWindow : public QMainWindow {
    Q_OBJECT

public:
    MapWindow(QWidget* parent = nullptr);

private:
    QGridLayout* gridLayout;

    void setupUI();

    void fetchMap();

};