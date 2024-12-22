#include "GameWindow.h"
#include<QGraphicsPixmapItem>
#include<QTimer>

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent),m_x(0),m_y(m_mapWidth),m_targetX(0),
m_targetY(0), m_speed(0.05f), m_currentSpeedX(0), m_currentSpeedY(0) {
    setupUI();
    resize(400, 400);

    playerLabel = new QLabel(this);
    updatePlayerTexture("Up");
    playerLabel->setScaledContents(true);
    playerLabel->resize(64, 64);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::updateGraphics);
    timer->start(16);

    setFocusPolicy(Qt::StrongFocus);
    fetchMap();
}
void GameWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W)
    {
        m_targetY -= 0.1f;
        m_currentSpeedY = -m_speed;
        updatePlayerTexture("Up");
    }
    else if (event->key() == Qt::Key_S)
    {
        m_targetY += 0.1f;
        m_currentSpeedY = m_speed;
        updatePlayerTexture("Down");
    }
    else if (event->key() == Qt::Key_A)
    {
        m_targetX -= 0.1f;
        m_currentSpeedX = -m_speed;
        updatePlayerTexture("Left");
    }
    else if (event->key() == Qt::Key_D)
    {
        m_targetX += 0.1f;
        m_currentSpeedX = m_speed;
        updatePlayerTexture("Right");
    }

}
void GameWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_S)
    {
        m_currentSpeedY = 0;
    }
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_D)
    {
        m_currentSpeedX = 0;
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

void GameWindow::updateGraphics() {
    float newX = m_x + m_currentSpeedX * 64;
    float newY = m_y + m_currentSpeedY * 64;
    if (canMoveTo(newX, newY))
    {
        m_x = newX;
        m_y = newY;
        int playerGridX = static_cast<int>(m_x / 64);
        int playerGridY = static_cast<int>(m_y / 64);


        displayPlayerPosition(playerGridX, playerGridY);
    }
    m_x = std::max(0.0f, (((m_x) < (static_cast<float>(m_mapWidth) * 64.0f - 64.0f)) ? (m_x) : (static_cast<float>(m_mapWidth) * 64.0f - 64.0f)));
    m_y = std::max(0.0f, (((m_y) < (static_cast<float>(m_mapHeight) * 64.0f - 64.0f)) ? (m_y) : (static_cast<float>(m_mapHeight) * 64.0f - 64.0f)));
    playerLabel->move(static_cast<int>(m_x), static_cast<int>(m_y));
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

bool GameWindow::canMoveTo(float newX, float newY)
{
   
        int blockSize = 64;
        int collisionOffset = 10;

        int gridX1 = static_cast<int>((newX + collisionOffset) / blockSize);
        int gridY1 = static_cast<int>((newY + collisionOffset) / blockSize);
        int gridX2 = static_cast<int>((newX + blockSize - collisionOffset) / blockSize);
        int gridY2 = static_cast<int>((newY + blockSize - collisionOffset) / blockSize);

        if (gridX1 < 0 || gridY1 < 0 || gridX2 >= m_mapWidth || gridY2 >= m_mapHeight) {
            return false;
        }

        if (m_mapData[gridY1][gridX1] == "Wall" || m_mapData[gridY1][gridX1] == "Unbreakable" ||
            m_mapData[gridY1][gridX1] == "Player" ||
            m_mapData[gridY1][gridX2] == "Wall" || m_mapData[gridY1][gridX2] == "Unbreakable" ||
            m_mapData[gridY1][gridX2] == "Player" ||
            m_mapData[gridY2][gridX1] == "Wall" || m_mapData[gridY2][gridX1] == "Unbreakable" ||
            m_mapData[gridY2][gridX1] == "Player" ||
            m_mapData[gridY2][gridX2] == "Wall" || m_mapData[gridY2][gridX2] == "Unbreakable" ||
            m_mapData[gridY2][gridX2] == "Player") {
            return false;
        }

        return true;

}

void GameWindow::updatePlayerTexture(const QString& direction) {
    QString texturePath = "Player" + direction + ".png";
    QPixmap playerTexture(texturePath);
    playerTexture = playerTexture.scaled(64, 64, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    playerLabel->setPixmap(playerTexture);
}

void GameWindow::addBullet(float x, float y, int direction)
{
    m_bulletData bullet = { x, y, direction };
    bullets.push_back(bullet);
    
    QLabel* bulletLabel = new QLabel(this);
    QPixmap bulletPixmap;

    if (direction == 0 || direction == 1) {
        bulletPixmap.load("BulletUpDown.png");
    }
    else if (direction == 2 || direction == 3) {
        bulletPixmap.load("BulletLeftRight.png");
    }

    if (bulletPixmap.isNull()) {
        qDebug() << "Failed to load bullet texture!";
        return;
    }

    bulletPixmap = bulletPixmap.scaled(64, 64, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    bulletLabel->setPixmap(bulletPixmap);
    bulletLabel->setFixedSize(64, 64);
    bulletLabel->setScaledContents(true);
    bulletLabel->move(static_cast<int>(x), static_cast<int>(y));
    bulletLabel->show();

    bulletLabels.push_back(bulletLabel);
}

void GameWindow::fetchPlayerPosition() {
    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/player_position" });

    if (response.status_code == 200) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        QJsonObject playerPosition = jsonDoc.object();

        int x = playerPosition.value("x").toInt();
        int y = playerPosition.value("y").toInt();

        displayPlayerPosition(x, y);
    }
    else {
        QLabel* errorLabel = new QLabel("Failed to fetch player position from server!", this);
        gridLayout->addWidget(errorLabel, 0, 0);
    }
}



void GameWindow::displayPlayerPosition(int x, int y) {
    if (!positionLabel) {
        positionLabel = new QLabel(this);
        gridLayout->addWidget(positionLabel, 0, 0);
    }

    positionLabel->setText(QString("(%1, %2)").arg(x).arg(y));
}
