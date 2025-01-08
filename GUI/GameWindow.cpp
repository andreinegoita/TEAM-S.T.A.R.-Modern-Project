#include "GameWindow.h"
#include<QGraphicsPixmapItem>
#include<QTimer>
#include"MainMenuWindow.h"

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent),m_x(0),m_y(m_mapWidth),m_targetX(0),
m_targetY(0), m_speed(0.05f), m_currentSpeedX(0), m_currentSpeedY(0),canShoot(true),m_bulletSpeed(5.0f) {
    setupUI();
    resize(400, 400);

    playerLabel = new QLabel(this);
    updatePlayerTexture("Up");
    playerLabel->setScaledContents(true);
    playerLabel->resize(64, 64);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::updateGraphics);
    timer->start(16);

    QTimer* bulletTimer = new QTimer(this);
    connect(bulletTimer, &QTimer::timeout, this, &GameWindow::updateBullets);
    bulletTimer->start(16);

    bulletCooldownTimer = new QTimer(this);
    bulletCooldownTimer->setSingleShot(true);
    connect(bulletCooldownTimer, &QTimer::timeout, this, [=]() {
        canShoot = true;
        });

    setFocusPolicy(Qt::StrongFocus);
    fetchMap();
    updateMap(m_mapArray);
   // fetchPlayerPosition();
    setPlayerStartPosition();
   
}
void GameWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W)
    {
        m_targetY -= 0.1f;
        m_currentSpeedY = -m_speed;
        updatePlayerTexture("Up");
        m_direction = 0;
    }
    else if (event->key() == Qt::Key_S)
    {
        m_targetY += 0.1f;
        m_currentSpeedY = m_speed;
        updatePlayerTexture("Down");
        m_direction = 1;
    }
    else if (event->key() == Qt::Key_A)
    {
        m_targetX -= 0.1f;
        m_currentSpeedX = -m_speed;
        updatePlayerTexture("Left");
        m_direction = 2;
    }
    else if (event->key() == Qt::Key_D)
    {
        m_targetX += 0.1f;
        m_currentSpeedX = m_speed;
        updatePlayerTexture("Right");
        m_direction = 3;
    }
    if (event->key() == Qt::Key_Space) {
        shootBullet();
    }
    if (event->key() == Qt::Key_Escape) {
        returnToMainMenu();
    }
}

void GameWindow::returnToMainMenu()
{
  
    MainMenuWindow* mainMenuWindow = new MainMenuWindow();
    mainMenuWindow->show();


    this->close();
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

std::mutex mapMutex;

void GameWindow::fetchMap() {
    std::lock_guard<std::mutex> lock(mapMutex);
    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/map" });
    if (response.status_code == 200) {
        if (response.text.empty())
        {
            qDebug() << "Error : Received empty response from server!";
        }
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        if (jsonDoc.isNull()) {
            qDebug() << "Error: Failed to parse JSON from server response!";
            qDebug() << "Response text:" << QString::fromStdString(response.text);
            return;
        }
         m_mapArray = jsonDoc.array();

        if(m_mapHeight==0&&m_mapWidth==0)
        {
            m_mapHeight = m_mapArray.size();
            if (m_mapHeight > 0)
            {
                m_mapWidth = m_mapArray[0].toArray().size();
            }
        }
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
        updateServerPlayerPosition();


        displayPlayerPosition(playerGridX, playerGridY);
    }
    m_x = std::max(0.0f, (((m_x) < (static_cast<float>(m_mapWidth) * 64.0f - 64.0f)) ? (m_x) : (static_cast<float>(m_mapWidth) * 64.0f - 64.0f)));
    m_y = std::max(0.0f, (((m_y) < (static_cast<float>(m_mapHeight) * 64.0f - 64.0f)) ? (m_y) : (static_cast<float>(m_mapHeight) * 64.0f - 64.0f)));
    playerLabel->move(static_cast<int>(m_x), static_cast<int>(m_y));
}


void GameWindow::displayMap(const QJsonArray& mapArray) {
   
    if (mapArray.isEmpty()) {
        qDebug() << "Error: Map array is empty!";
        return;
    }

    for (const QJsonValue& rowValue : mapArray) {
        if (!rowValue.isArray()) {
            qDebug() << "Error: Expected a row to be a JSON array!";
            return;
        }
    }


    while (QLayoutItem* item = gridLayout->takeAt(0)) {
        if (item->widget())
        {
            item->widget()->deleteLater();
        }
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
            if (cellType == "Wall" || cellType == "Bomb") texture.load("Breakable.png");
            else if (cellType == "Unbreakable") texture.load("Unbreakable.png");
            else if (cellType == "Player") texture.load("PlayerUp.png");
            else if (cellType == "Empty") texture.load("Empty.png");

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

void GameWindow::updateMap(const QJsonArray& mapArray) {
    if (mapArray.isEmpty()) {
        qDebug() << "Error: Map array is empty!";
        return;
    }

    if (m_mapData.isEmpty()) {
        // First-time initialization
        displayMap(mapArray);
        return;
    }

    int blockSize = 64;
    for (int row = 0; row < mapArray.size(); ++row) {
        QJsonArray rowArray = mapArray[row].toArray();

        for (int col = 0; col < rowArray.size(); ++col) {
            QString cellType = rowArray[col].toString();

            if (row < m_mapData.size() && col < m_mapData[row].size() &&
                m_mapData[row][col] != cellType) {
                // Update only if the cell type has changed
                QLabel* cellLabel = qobject_cast<QLabel*>(gridLayout->itemAtPosition(row, col)->widget());
                if (cellLabel) {
                    QPixmap texture;
                    if (cellType == "Wall" || cellType == "Bomb") texture.load("Breakable.png");
                    else if (cellType == "Unbreakable") texture.load("Unbreakable.png");
                    else if (cellType == "Player") texture.load("PlayerUp.png");
                    else if (cellType == "Empty") texture.load("Empty.png");


                    texture = texture.scaled(blockSize, blockSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
                    cellLabel->setPixmap(texture);
                }
                m_mapData[row][col] = cellType;
            }
        }
    }
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
            m_mapData[gridY1][gridX1] == "Player" || m_mapData[gridY1][gridX1] == "Bomb"||
            m_mapData[gridY1][gridX2] == "Wall" || m_mapData[gridY1][gridX2] == "Unbreakable" ||
            m_mapData[gridY1][gridX2] == "Player" || m_mapData[gridY1][gridX2] == "Bomb" ||
            m_mapData[gridY2][gridX1] == "Wall" || m_mapData[gridY2][gridX1] == "Unbreakable" ||
            m_mapData[gridY2][gridX1] == "Player" || m_mapData[gridY2][gridX1] == "Bomb" ||
            m_mapData[gridY2][gridX2] == "Wall" || m_mapData[gridY2][gridX2] == "Unbreakable" ||
            m_mapData[gridY2][gridX2] == "Player" || m_mapData[gridY2][gridX2] == "Bomb" ){
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

void GameWindow::shootBullet() {
    if (canShoot) 
    {
        float bulletStartX = m_x;
        float bulletStartY = m_y;

        addBullet(bulletStartX, bulletStartY, m_direction);
        canShoot = false;
        bulletCooldownTimer->start(500);
        //QJsonObject bulletObject;
        //bulletObject["x"] = static_cast<int>(bulletStartX);
        //bulletObject["y"] = static_cast<int>(bulletStartY);
        //bulletObject["direction"] = m_direction;
        //bulletObject["speed"] = m_bulletSpeed; 

        //QString payload = QJsonDocument(bulletObject).toJson(QJsonDocument::Compact);

        //QtConcurrent::run([payload]() {
        //    cpr::Response response = cpr::Post(
        //        cpr::Url{ "http://localhost:18080/shoot_bullet" },
        //        cpr::Body{ payload.toStdString() },
        //        cpr::Header{ { "Content-Type", "application/json" } }
        //    );

        //    if (response.status_code != 200) {
        //        qDebug() << "Failed to send bullet info to server!";
        //    }
        //    });
    }
}
void GameWindow::updateServerBulletsPosition() {
    QJsonArray bulletArray;

    for (const m_bulletData& bullet : bullets) {
        QJsonObject bulletObject;
        bulletObject["x"] = static_cast<int>(bullet.x);
        bulletObject["y"] = static_cast<int>(bullet.y);
        bulletObject["direction"] = bullet.direction;
        bulletArray.append(bulletObject); // Adăugăm explicit obiectele în QJsonArray
    }

    QString payload = QJsonDocument(bulletArray).toJson(QJsonDocument::Compact);

    QtConcurrent::run([payload]() {
        cpr::Response response = cpr::Post(
            cpr::Url{ "http://localhost:18080/bullets_position" },
            cpr::Body{ payload.toStdString() },
            cpr::Header{ { "Content-Type", "application/json" } }
        );

        if (response.status_code != 200) {
            qDebug() << "Failed to update bullet positions on server!";
        }
        });
}

void GameWindow::updateServerMapCell(int row, int col)
{
        cpr::Response response = cpr::Post(
        cpr::Url{ "http://localhost:18080/map/empty/" + std::to_string(row) + "/" + std::to_string(col)},
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    if (response.status_code != 200) {
       
        qDebug() << "Error: " << response.status_code << " - " << "map could not be updated";
    }
}

void GameWindow::setPlayerStartPosition()
{
    if (m_mapData[0][0] == "Empty")
    {
        m_x = 0;
        m_y = 0;

    }
    else if (m_mapData[0][m_mapWidth-1] == "Empty")
    {
        m_y = 0;
        m_x = 64*(m_mapWidth-1);
    }
    else if (m_mapData[m_mapHeight-1][m_mapWidth-1] == "Empty")
    {
        m_y = 64*(m_mapHeight-1);
        m_x = 64*(m_mapWidth - 1);
    }
    else if (m_mapData[m_mapHeight - 1][0] == "Empty")
    {
        m_y = 64*(m_mapHeight - 1);
        m_x = 0;
    }
}

PowerUpType stringToPowerUpType(const QString& str) {
    if (str == "SpeedBoost") {
        return PowerUpType::SpeedBoost;
    }
    else if (str == "Shield") {
        return PowerUpType::Shield;
    }
    else if (str == "ExtraLife") {
        return PowerUpType::ExtraLife;
    }

    else {
        qDebug() << "Unknown power-up: " << str;
        return PowerUpType::SpeedBoost;
    }
}

void GameWindow::fetchPowerUpQueue()
{
    while (!powerUpQueue.empty()) {
        powerUpQueue.pop();
    }

    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/powerUpQueue" });

    qDebug() << "Response received:" << response.text.c_str();
    if (response.status_code == 200) {
        if (response.text.empty()) {
            qDebug() << "Error: Received empty response from server!";
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(response.text.c_str());
        if (doc.isNull() || !doc.isArray()) {
            qDebug() << "Invalid or unexpected JSON response! Expected an array.";
            return;
        }

        QJsonArray array = doc.array();
        qDebug() << "Number of power-ups: " << array.size();

        for (int i = 0; i < array.size(); ++i) {
            QString powerUpString = array[i].toString();
            qDebug() << "Power-up " << i + 1 << ": " << powerUpString;

            PowerUpType powerUp = stringToPowerUpType(powerUpString);
            powerUpQueue.push(powerUp);
        }
    }
    else {
        qDebug() << "Error: HTTP request failed with status code " << response.status_code;
    }
}

void GameWindow::fetchPlayerPosition() {
    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/player_position" });

    if (response.status_code == 200) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        if (!jsonDoc.isArray()) {
            qDebug() << "Error: Expected a JSON array but received different data!";
            qDebug() << "Parsed JSON:" << jsonDoc.toJson();
            return;
        }
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

QPointer<QLabel> positionLabel;

void GameWindow::displayPlayerPosition(int x, int y) {
    if (!(x >= 0 && x < m_mapHeight && y >= 0 && y < m_mapWidth)) {
        qDebug() << "Invalid coordinates: (" << x << ", " << y << ")";
        return;
    }

    if (!gridLayout) {
        qDebug() << "gridLayout is null. Initializing.";
        gridLayout = new QGridLayout(this);
        setLayout(gridLayout);
    }

    if (!positionLabel) {
        qDebug() << "positionLabel is null. Creating new QLabel.";
        positionLabel = new QLabel(this);
        gridLayout->addWidget(positionLabel, 0, 0);
    }

    if (positionLabel) {
        qDebug() << "Updating positionLabel with coordinates: (" << x << ", " << y << ")";
        positionLabel->setText(QString("(%1, %2)").arg(x).arg(y));
    }
    else {
        qDebug() << "Unexpected error: positionLabel is still null.";
    }
}

void GameWindow::updateBullets() {
   

    for (int i = 0; i < bullets.size(); ++i) {
        m_bulletData& bullet = bullets[i];


        switch (bullet.direction) {
        case 0: bullet.y -= m_bulletSpeed; break;
        case 1: bullet.y += m_bulletSpeed; break;
        case 2: bullet.x -= m_bulletSpeed; break;
        case 3: bullet.x += m_bulletSpeed; break;
        }

        if (i >= bulletLabels.size() || i >= bullets.size()) {
            continue;
        }
        if (i < bulletLabels.size()) {
            bulletLabels[i]->move(static_cast<int>(bullet.x), static_cast<int>(bullet.y));
        }
        int ix = std::clamp(static_cast<int>((bullet.y + 32) / 64), 0, m_mapHeight - 1);
        int iy = std::clamp(static_cast<int>((bullet.x + 32) / 64), 0, m_mapWidth - 1);

        if (bullet.x < -16 || bullet.y < -16 || bullet.x >= (m_mapWidth * 63 - 32) || bullet.y >= (m_mapHeight * 63 - 32)
            || (ix >= 0 && ix < m_mapHeight && iy >= 0 && iy < m_mapWidth &&
                (m_mapData[ix][iy] == "Wall" || m_mapData[ix][iy] == "Unbreakable" || m_mapData[ix][iy] == "Bomb" || m_mapData[ix][iy] == "Player")))
        {
            if (ix >= 0 && ix < m_mapHeight && iy >= 0 && iy < m_mapWidth) {
                destroyCells(ix, iy);
            }
            delete bulletLabels[i];
            bulletLabels.remove(i);
            bullets.remove(i);
            --i;
        }
        if (ix < 0 || iy < 0 || ix >= m_mapHeight || iy >= m_mapWidth) {
            delete bulletLabels[i];
            bulletLabels.remove(i);
            bullets.remove(i);
            --i;
            continue;
        }

        updateMap(m_mapArray);
    }
    //updateServerBulletsPosition();
}

void GameWindow::destroyCells(int x, int y)
{
    if (m_mapData[x][y] == "Unbreakable")
        return;
    if (m_mapData[x][y] == "Wall" || m_mapData[x][y] == "Player")
    {

        updateServerMapCell(x, y);
        fetchMap();
        return;
    }
    if (m_mapData[x][y] == "Bomb")
    {
        for (int i = x - 1; i <= x + 1; i++)
            for (int j = y - 1; j <= y + 1; j++)
                if (m_mapData[i][j] == "Wall" || m_mapData[i][j] == "Player")
                {
                    updateServerMapCell(i, j);

                }
        
        updateServerMapCell(x, y);
        fetchMap();
    }
}

void GameWindow::updateServerPlayerPosition() {
    static int lastX = -1;
    static int lastY = -1;

    int currentX = static_cast<int>(m_x / 64);
    int currentY = static_cast<int>(m_y / 64);

    if (currentX != lastX || currentY != lastY) {
        lastX = currentX;
        lastY = currentY;

        QtConcurrent::run([currentX, currentY]() {
            std::string payload = "{\"x\":" + std::to_string(currentX) + ",\"y\":" + std::to_string(currentY) + "}";

            cpr::Response response = cpr::Post(
                cpr::Url{ "http://localhost:18080/player_position" },
                cpr::Body{ payload },
                cpr::Header{ { "Content-Type", "application/json" } }
            );

            if (response.status_code != 200) {
                qDebug() << "Failed to update player position on server!";
            }
            });
    }
}



