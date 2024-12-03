#include "MapWindow.h"
#include<QGraphicsPixmapItem>

MapWindow::MapWindow(QWidget* parent)
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


void MapWindow::displayMap(const QJsonArray& mapArray) {
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);

    for (int row = 0; row < mapArray.size(); ++row) {
        QJsonArray rowArray = mapArray[row].toArray();
        for (int col = 0; col < rowArray.size(); ++col) {
            QString cellType = rowArray[col].toString();
            QLabel* cellLabel = new QLabel(this);

            QPixmap texture;
            if (cellType == "Empty") texture.load("Empty.png");
            else if (cellType == "Wall") texture.load("Breakable.png");
            else if (cellType == "Unbreakable") texture.load("Unbreakable.png");
            else if (cellType == "Player") texture.load("Player.png");

            texture = texture.scaled(64, 64, Qt::IgnoreAspectRatio, Qt::FastTransformation);
            cellLabel->setPixmap(texture);
            cellLabel->setFixedSize(64, 64);

            gridLayout->addWidget(cellLabel, row, col);
        }
    }
    setFixedSize(gridLayout->sizeHint());
}
