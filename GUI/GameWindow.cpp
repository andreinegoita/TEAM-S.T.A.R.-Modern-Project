#include "GameWindow.h"
#include<QGraphicsPixmapItem>

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent),m_x(0),m_y(m_mapWidth),m_targetX(0),
m_targetY(0){
    setupUI();
    resize(400, 400);
    fetchMap();
}
void GameWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W)
    {
        m_targetY -= 0.1f;
        m_currentSpeedY = -m_speed;
        /*TO DO:
          updatePlayerTexture("Up")*/
    }
    else if (event->key() == Qt::Key_S)
    {
        m_targetY += 0.1f;
        m_currentSpeedY = m_speed;
        /*TO DO:
          updatePlayerTexture(*/
    }
    else if (event->key() == Qt::Key_A)
    {
        m_targetX -= 0.1f;
        m_currentSpeedX = -m_speed;
        /*TO DO:
         updatePlayerTexture("Down)*/
    }
    else if (event->key() == Qt::Key_D)
    {
        m_targetX += 0.1f;
        m_currentSpeedX = m_speed;
        /*TO DO:
         uppdatePlayerTexture("Left")*/
    }
}
void GameWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);
}

void GameWindow::fetchMap() {
    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/map" });
    if (response.status_code == 200) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        QJsonArray mapArray = jsonDoc.array();

        m_mapHeight = mapArray.size();
        if (m_mapHeight > 0)
        {
            m_mapWidth = mapArray[0].toArray().size();
        }
        displayMap(mapArray);
    }
    else {
        QLabel* errorLabel = new QLabel("Failed to fetch map from server!", this);
        gridLayout->addWidget(errorLabel, 0, 0);
    }
}


void GameWindow::displayMap(const QJsonArray& mapArray) {
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);

    m_mapData.clear();

    int blockSize = 64;

    for (int row = 0; row < mapArray.size(); ++row) {
        QJsonArray rowArray = mapArray[row].toArray();
        QVector<QString> rowData;

        for (int col = 0; col < rowArray.size(); ++col) {
            QString cellType = rowArray[col].toString();
            QLabel* cellLabel = new QLabel(this);

            QPixmap texture;
            if (cellType == "Empty") texture.load("Empty.png");
            else if (cellType == "Wall") texture.load("Breakable.png");
            else if (cellType == "Unbreakable") texture.load("Unbreakable.png");
            else if (cellType == "Player") texture.load("Player.png");

            texture = texture.scaled(blockSize, blockSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
            cellLabel->setPixmap(texture);
            cellLabel->setFixedSize(blockSize, blockSize);

            rowData.append(cellType);
            gridLayout->addWidget(cellLabel, row, col);
        }

        m_mapData.append(rowData);
    }

    setFixedSize(gridLayout->sizeHint());
}
