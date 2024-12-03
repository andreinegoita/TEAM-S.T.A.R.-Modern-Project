#include "MapWindow.h"

MapWindow::MapWindow(QWidget* parent = nullptr)
    : QMainWindow(parent) {
    setupUI();
    fetchMap(); 
}
void MapWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);
}

void MapWindow::fetchMap() {
    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/map" });
    if (response.status_code == 200) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        QJsonArray mapArray = jsonDoc.array();
        displayMap(mapArray);
    }
    else {
        QLabel* errorLabel = new QLabel("Failed to fetch map from server!", this);
        gridLayout->addWidget(errorLabel, 0, 0);
    }
}

