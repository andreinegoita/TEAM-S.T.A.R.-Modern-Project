#include "GameWindow.h"
#include<QGraphicsPixmapItem>
#include<QTimer>
#include <QPainter>  
#include <QPaintEvent>
#include<qmessagebox.h>




GameWindow::GameWindow(const QString& player,QWidget* parent)
    :QMainWindow(parent), m_x(0), m_y(m_mapWidth), m_targetX(0),
    m_targetY(0), m_speed(0.05f), m_currentSpeedX(0), m_currentSpeedY(0), canShoot(true),m_bulletSpeed(5.0f),m_fireRate(500),m_playerName(player.toStdString()) {
    setupUI();
    resize(400, 400);
   
    playerLabel = new QLabel(this);
    playerLabel->setPixmap(QPixmap("Empty.png")); 
    playerLabel->setScaledContents(true);
    playerLabel->resize(64, 64);

    playerLabel->setAttribute(Qt::WA_TranslucentBackground);
    playerLabel->setStyleSheet("background: transparent;");
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
    setPlayerStartPosition(0,0);
    QTimer* showLives = new QTimer(this);
    QTimer* powerUpTimer = new QTimer(this);
    connect(powerUpTimer, &QTimer::timeout, this, &GameWindow::fetchPowerUpQueue);
    powerUpTimer->start(5000);



    visibilityTimer = new QTimer(this);
    connect(visibilityTimer, &QTimer::timeout, this, &GameWindow::increaseVisibility);
    visibilityTimer->start(5000); 
    qDebug() << "Updated";

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &GameWindow::fetchMap);
    updateTimer->start(200);

    messageLabel = new QLabel(this);
    messageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    messageLabel->setStyleSheet("QLabel { color : red; font-size: 30px; }");
    messageLabel->hide();

    QTimer* syncTimer = new QTimer(this);
    connect(syncTimer, &QTimer::timeout, this, &GameWindow::fetchAllPlayersPositions);
    syncTimer->start(200);

    
    QTimer* syncLivesTimer = new QTimer(this);
    connect(syncLivesTimer, &QTimer::timeout, this, &GameWindow::updatePlayerLivesFromServer);
    syncLivesTimer->start(1000);
    QTimer* pointsTimer = new QTimer(this);
    connect(pointsTimer, &QTimer::timeout, this, &GameWindow::fetchPlayerPoints);
    pointsTimer->start(5000);


    qDebug() << "GAME AT " << m_playerName;
}


void GameWindow::setPlayerStartPosition(int x, int y) {
   
    QList<QPair<int, int>> possiblePositions = {
        {0, 0},                                 
        {0, m_mapWidth - 1},                     
        {m_mapHeight - 1, 0},                    
        {m_mapHeight - 1, m_mapWidth - 1}     
    };


    for (const auto& position : possiblePositions) {
        int px = position.first;
        int py = position.second;

        if (m_mapData[px][py] != "Player") { 
            x = px;
            y = py;
            m_mapData[px][py] = "Player";
            break;
        }
    }

    m_x = y * 64;
    m_y = x * 64;

    playerLabel->move(m_x, m_y);
    playerLabel->show();
    sendPlayerSpawn(x, y);
    qDebug() << "Player set at position: (" << x << ", " << y << ")";
}

void GameWindow::updatePlayerLivesFromServer() {
    cpr::Response response = cpr::Get(cpr::Url{ base_url + "/get_player_lives" });

    if (response.status_code == 200) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();

            for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it) {
                QString playerName = it.key();
                int lives = it.value().toInt();

                if (playerName.toStdString() == m_playerName) {
                   
                    if (m_playerLives != lives) {
                        m_playerLives = lives;
                        displayPlayerLives();
                        qDebug() << "Updated local lives for player" << playerName << ":" << lives;
                    }
                }
                else {
                   
                    playerLives[playerName.toStdString()] = lives;
                    qDebug() << "Updated lives for other player" << playerName << ":" << lives;
                }
            }
        }
        else {
            qDebug() << "Failed to parse JSON object from server response.";
        }
    }
    else {
        qDebug() << "Failed to fetch player lives from server. Status code:" << response.status_code;
    }
}
void GameWindow::displayPlayerLives() {
    if (!playerLabel) {
        return;
    }

    playerLabel->setText(QString::fromStdString(m_playerName) + ": " + QString::number(m_playerLives) + " Lives");
    playerLabel->setStyleSheet("color: black; font-size: 16px;");
    playerLabel->show();
}
void GameWindow::keyPressEvent(QKeyEvent* event)
{
    if (lives == 0) {
        return;
    }
        if (event->key() == Qt::Key_W)
        {
            m_targetY -= 0.1f;
            m_currentSpeedY = -m_speed;
            updatePlayerTexture("Up");

            m_direction = 0;
            if (m_shield)
            {
                updatePlayerTexture("UpShield");
            }
        }
        else if (event->key() == Qt::Key_S)
        {
            m_targetY += 0.1f;
            m_currentSpeedY = m_speed;
            updatePlayerTexture("Down");

            m_direction = 1;
            if (m_shield)
            {
                updatePlayerTexture("DownShield");
            }
        }
        else if (event->key() == Qt::Key_A)
        {
            m_targetX -= 0.1f;
            m_currentSpeedX = -m_speed;
            updatePlayerTexture("Left");

            m_direction = 2;
            if (m_shield)
            {
                updatePlayerTexture("LeftShield");
            }
        }
        else if (event->key() == Qt::Key_D)
        {
            m_targetX += 0.1f;
            m_currentSpeedX = m_speed;
            updatePlayerTexture("Right");

            m_direction = 3;
            if (m_shield)
            {
                updatePlayerTexture("RightShield");
            }
        }
        if (event->key() == Qt::Key_Space) {
            shootBullet();
        }
        if (event->key() == Qt::Key_E) {
            qDebug() << "Press E!";

            applyNextPowerUp();
        }
        if (event->key() == Qt::Key_I)
        {
        lives--;

        }
        if (event->key() == Qt::Key_L)
        {

        QTimer::singleShot(3000, this, [this]() {
            lives--;
            QMessageBox::information(this, "Game Over", "You have 0 lives. You lose.");
            });
        
        }
        if (event->key() == Qt::Key_V)
        {
        QTimer::singleShot(3000, this, [this]() {
            QMessageBox::information(this, "Game Over", "You are the last one on map.You Win");
            });

         }
    if (event->key() == Qt::Key_P)
    {
        addPlayerPoints(100);
    }
}

void GameWindow::updatePlayerUI(double speed, int lives, bool hasShield,double fireRate) {

    m_bulletSpeed = speed;
    qDebug() << "Bulled speed updated to:" << speed;


    m_playerLives = lives;
    qDebug() << "Player lives updated to:" << lives;

    if (hasShield) {

        m_shield = true;

        qDebug() << "Shield effect activated!";
    }
    else {

        m_shield = false;
        qDebug() << "No Shield!";
    }
    m_fireRate = fireRate;
    qDebug() << "Fire Rate updated to:" << m_fireRate;
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

    cpr::Response response = cpr::Get(cpr::Url{ base_url+"/powerUpQueue" });

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
void GameWindow::applyNextPowerUp() {
    if (powerUpQueue.empty()) {
        qDebug() << "No power-ups in the queue!";
        return;
    }

    PowerUpType nextPowerUp = powerUpQueue.front();
    powerUpQueue.pop();
    if (nextPowerUp == PowerUpType::ExtraLife) {
        m_playerLives++;
        qDebug() << "Extra life granted! Lives now:" << m_playerLives;
    }
    std::string payload = "{\"powerUpType\": " + std::to_string(static_cast<int>(nextPowerUp)) + "}";

  
    cpr::Response response = cpr::Post(
        cpr::Url{ base_url+"/applyPowerUp" },
        cpr::Body{ payload },
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    if (response.status_code == 200) {
        qDebug() << "Power-up applied successfully!";

        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        QJsonObject jsonObj = jsonDoc.object();

        double newBulletSpeed = jsonObj["bulletSpeed"].toDouble();
        int newLives = jsonObj["lives"].toInt();
        bool hasShield = jsonObj["hasShield"].toBool();
        double newFireRate = jsonObj["fireRate"].toDouble();

        updatePlayerUI(newBulletSpeed, newLives, hasShield,newFireRate);

        if (jsonObj.contains("map")) {
            QJsonArray mapArray = jsonObj["map"].toArray();
            updateMap(mapArray);
        }
        QTimer::singleShot(10000, this, &GameWindow::resetPowerOffEfects);
    }
    else {
        qDebug() << "Failed to apply power-up. Status code:" << response.status_code;
        qDebug() << "Error response:" << QString::fromStdString(response.text);
    }
}

void GameWindow::FetchPlayersFromServer() {
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);


    QNetworkRequest request(QUrl(base_url_Q + "/get_players"));
    QNetworkReply* reply = manager->get(request);


    connect(reply, &QNetworkReply::finished, this, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {

            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

            if (jsonDoc.isObject()) {
                QJsonObject jsonObj = jsonDoc.object();
                QJsonArray playersArray = jsonObj["players"].toArray();


                for (const QJsonValue& playerValue : playersArray) {
                    QJsonObject playerObj = playerValue.toObject();
                    int playerId = playerObj["id"].toInt();
                    QString playerName = playerObj["name"].toString();
                    int playerPoints = playerObj["points"].toInt();


                    qDebug() << "Received player: " << playerName
                        << ", ID: " << playerId
                        << ", Points: " << playerPoints;
                }
            }
        }
        else {
            qDebug() << "Error fetching players: " << reply->errorString();
        }

        reply->deleteLater();
        });
}

void GameWindow::resetPowerOffEfects()
{
    m_bulletSpeed = 5.0f;
    qDebug() << "Bullet speed reset to default value: 5.0";

    m_shield = false;
    qDebug() << "Shield effect deactivated!";
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
    cpr::Response response = cpr::Get(cpr::Url{ base_url+"/map" });
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

        
            m_mapHeight = m_mapArray.size();
            if (m_mapHeight > 0)
            {
                m_mapWidth = m_mapArray[0].toArray().size();
            }
        
    }
    else {
        QLabel* errorLabel = new QLabel("Failed to fetch map from server!", this);
        gridLayout->addWidget(errorLabel, 0, 0);
    }
}

void GameWindow::fetchPlayerPosition() {
    cpr::Response response = cpr::Get(cpr::Url{ base_url + "/player_position" });

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
    }
    else {
        QLabel* errorLabel = new QLabel("Failed to fetch player position from server!", this);
        gridLayout->addWidget(errorLabel, 0, 0);
    }
}
void GameWindow::updateServerPlayerPosition() {
    static int lastX = -1;
    static int lastY = -1;

    int currentX = static_cast<int>((m_x + 10) / 64);
    int currentY = static_cast<int>((m_y + 10) / 64);

    if (currentX != lastX || currentY != lastY) {
        int direction = m_direction; 
        std::string payload = "{\"name\":\"" + m_playerName + "\","
            "\"x\":" + std::to_string(currentX) + ","
            "\"y\":" + std::to_string(currentY) + ","
            "\"direction\":" + std::to_string(direction) + ","
            "\"lx\":" + std::to_string(lastX) + ","
            "\"ly\":" + std::to_string(lastY) + "}";

        QtConcurrent::run([payload, this]() {
            cpr::Response response = cpr::Post(
                cpr::Url{ base_url + "/player_position" },
                cpr::Body{ payload },
                cpr::Header{ {"Content-Type", "application/json"} }
            );

            if (response.status_code != 200) {
                qDebug() << "Failed to update player position on server! Status code:" << response.status_code;
            }
            });

        lastX = currentX;
        lastY = currentY;
    }
}

void GameWindow::updateGraphics() {

    float newX = m_x + m_currentSpeedX * 64;
    float newY = m_y + m_currentSpeedY * 64;
    if (canMoveTo(newX, newY))
    {
        m_x = newX;
        m_y = newY;
        int playerGridX = static_cast<int>((m_x + 32) / 64);
        int playerGridY = static_cast<int>((m_y + 32) / 64);
        updateServerPlayerPosition();


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
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);

    m_mapData.clear();

    int blockSize = 64;
    QPixmap wallTexture("Breakable.png");
    QPixmap unbreakableTexture("Unbreakable.png");
    QPixmap emptyTexture("Empty.png");
    QPixmap bulletTexture("BulletUpDown.png");

    wallTexture = wallTexture.scaled(blockSize, blockSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    unbreakableTexture = unbreakableTexture.scaled(blockSize, blockSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    emptyTexture = emptyTexture.scaled(blockSize, blockSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    bulletTexture = bulletTexture.scaled(blockSize, blockSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    for (int row = 0; row < mapArray.size(); ++row) {
        QJsonArray rowArray = mapArray[row].toArray();
        QVector<QString> rowData;

        for (int col = 0; col < rowArray.size(); ++col) {
            QString cellType = rowArray[col].toString();
            QLabel* cellLabel = new QLabel(this);

            QPixmap texture;
            if (cellType == "Wall") {
                texture = wallTexture;
            }
            else if (cellType == "Unbreakable") {
                texture = unbreakableTexture;
            }
            else if (cellType == "Bullet") {
                texture = bulletTexture;
            }
            else if (cellType == "Empty") {
                texture = emptyTexture;
            }

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
        displayMap(mapArray);
        return;
    }

    int blockSize = 64;
    QPixmap wallTexture("Breakable.png");
    QPixmap unbreakableTexture("Unbreakable.png");
    QPixmap emptyTexture("Empty.png");
    QPixmap bulletTexture("BulletUpDown.png");

    wallTexture = wallTexture.scaled(blockSize, blockSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    unbreakableTexture = unbreakableTexture.scaled(blockSize, blockSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    emptyTexture = emptyTexture.scaled(blockSize, blockSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    bulletTexture = bulletTexture.scaled(blockSize, blockSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    
    int playerGridX = static_cast<int>(m_x / blockSize);
    int playerGridY = static_cast<int>(m_y / blockSize);

    for (int row = 0; row < mapArray.size(); ++row) {
        QJsonArray rowArray = mapArray[row].toArray();

        for (int col = 0; col < rowArray.size(); ++col) {
            QString cellType = rowArray[col].toString();

           
            int distance = std::max(abs(row - playerGridY), abs(col - playerGridX));

            QLabel* cellLabel = qobject_cast<QLabel*>(gridLayout->itemAtPosition(row, col)->widget());
            if (!cellLabel) continue;

            if (distance <= visibilityRadius) {
               
                QPixmap texture;
                if (cellType == "Wall") {
                    texture = wallTexture;
                    m_mapData[row][col] = "Wall";
                }
                else if (cellType == "Unbreakable") {
                    texture = unbreakableTexture;
                    m_mapData[row][col] = "Unbreakable";
                }
                else if (cellType == "Bullet") {
                    texture = bulletTexture;
                    m_mapData[row][col] = "Bullet";

                    qDebug() << "Found Bullet at (" << row << ", " << col << ")";

                   
                    QTimer::singleShot(5000, this, [=]() {
                        qDebug() << "Timer triggered for Bullet at (" << row << ", " << col << ")";
                        if (m_mapData[row][col] == "Bullet") {
                            m_mapData[row][col] = "Empty";

                            QLabel* cellLabel = qobject_cast<QLabel*>(gridLayout->itemAtPosition(row, col)->widget());
                            if (cellLabel) {
                                cellLabel->setPixmap(emptyTexture);
                                qDebug() << "Cell updated to Empty at (" << row << ", " << col << ")";
                            }

                            updateServerMapCell(row, col);
                        }
                        else {
                            qDebug() << "Bullet already removed at (" << row << ", " << col << ")";
                        }
                        });
                }
                else if (cellType == "Empty") {
                    texture = emptyTexture;
                    m_mapData[row][col] = "Empty";
                }

                cellLabel->setPixmap(texture);
            }
            else {
             
                cellLabel->setPixmap(emptyTexture);
                m_mapData[row][col] = "Unknown"; 
            }
        }
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

    auto isBlocked = [this](int x, int y) {
        return m_mapData[x][y] == "Wall" ||
            m_mapData[x][y] == "Unbreakable" ||
            m_mapData[x][y] == "Bomb";
        };

    if (isBlocked(gridY1, gridX1) || isBlocked(gridY1, gridX2) ||
        isBlocked(gridY2, gridX1) || isBlocked(gridY2, gridX2)) {
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

void GameWindow::addBullet(float x, float y, int direction) {
    m_bulletData bullet;

    
    bullet.direction = direction;
    bullet.owner = QString::fromStdString(m_playerName);
    QPixmap bulletPixmap;

   
    switch (direction) {
    case 0: 
        bullet.x = x;
        bullet.y = y - 64;
        bulletPixmap.load("BulletUpDown.png");
        break;
    case 1: 
        bullet.x = x;
        bullet.y = y + 64;
        bulletPixmap.load("BulletUpDown.png");
        break;
    case 2: 
        bullet.x = x - 64;
        bullet.y = y;
        bulletPixmap.load("BulletLeftRight.png");
        break;
    case 3: 
        bullet.x = x + 64;
        bullet.y = y;
        bulletPixmap.load("BulletLeftRight.png");
        break;
    }

    bullet.previousCellX = static_cast<int>((bullet.y + 32) / 64);
    bullet.previousCellY = static_cast<int>((bullet.x + 32) / 64);

  
    bullets.append(bullet);

   
    QLabel* bulletLabel = new QLabel(this);
    bulletLabel->setPixmap(bulletPixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    bulletLabel->setGeometry(static_cast<int>(bullet.x), static_cast<int>(bullet.y), 64, 64); 
    bulletLabel->show();
    bulletLabels.append(bulletLabel);
}



void GameWindow::shootBullet() {
    if (canShoot)
    {
        float bulletStartX = m_x;
        float bulletStartY = m_y;      
          addBullet(bulletStartX, bulletStartY, m_direction);
            canShoot = false;
            bulletCooldownTimer->start(m_fireRate);
        
    }
}

void GameWindow::updateServerBulletsPosition() {
    static int lastX = -1;
    static int lastY = -1;



    QJsonArray bulletArray;

    for (const m_bulletData& bullet : bullets) {
        QJsonObject bulletObject;
        int currentX = static_cast<int>((bullet.x + 32) / 64);
        int currentY = static_cast<int>((bullet.y + 32) / 64);
        if (currentX != lastX || currentY != lastY)
        {
            if (lastX != -1 && lastY != -1)
            {
                bulletObject["x"] = currentX;
                bulletObject["y"] = currentY;
                bulletObject["direction"] = bullet.direction;
                bulletObject["prev_x"] = lastX;
                bulletObject["prev_y"] = lastY;
                bulletArray.append(bulletObject);
            }
            lastX = currentX;
            lastY = currentY;
        }


    }


    QString payload = QJsonDocument(bulletArray).toJson(QJsonDocument::Compact);

    QtConcurrent::run([payload,this]() {
        cpr::Response response = cpr::Post(
            cpr::Url{base_url+"/bullets_position" },
            cpr::Body{ payload.toStdString() },
            cpr::Header{ { "Content-Type", "application/json" } }
        );

        if (response.status_code != 200) {
            qDebug() << "Failed to update bullet positions on server!";
        }
        });
}


void GameWindow::updateServerMapCell(int row, int col) {
    cpr::Response response = cpr::Post(
        cpr::Url{ base_url + "/map/empty/" + std::to_string(row) + "/" + std::to_string(col) },
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    if (response.status_code != 200) {
        qDebug() << "Error: Failed to update server for cell (" << row << ", " << col << ")";
    }
    else {
        qDebug() << "Server updated cell (" << row << ", " << col << ") to Empty";
    }
}
QPointer<QLabel> positionLabel;


void GameWindow::updateBullets() {
    for (int i = 0; i < bullets.size(); ++i) {
        m_bulletData& bullet = bullets[i];

        
        if (bullet.inCollision) {
            continue;
        }

      
        switch (bullet.direction) {
        case 0: bullet.y -= m_bulletSpeed; break;
        case 1: bullet.y += m_bulletSpeed; break; 
        case 2: bullet.x -= m_bulletSpeed; break; 
        case 3: bullet.x += m_bulletSpeed; break; 
        }

       
        if (i < bulletLabels.size()) {
            bulletLabels[i]->move(static_cast<int>(bullet.x), static_cast<int>(bullet.y));
        }

        
        int ix = std::clamp(static_cast<int>((bullet.y + 32) / 64), 0, m_mapHeight - 1);
        int iy = std::clamp(static_cast<int>((bullet.x + 32) / 64), 0, m_mapWidth - 1);

        
        for (int j = 0; j < bullets.size(); ++j) {
            if (i != j && !bullets[j].inCollision) { 
                m_bulletData& otherBullet = bullets[j];

                
                int otherIx = std::clamp(static_cast<int>((otherBullet.y + 32) / 64), 0, m_mapHeight - 1);
                int otherIy = std::clamp(static_cast<int>((otherBullet.x + 32) / 64), 0, m_mapWidth - 1);

                if (ix == otherIx && iy == otherIy) {
                    qDebug() << "Coliziune între gloanțe la (" << ix << ", " << iy << ")";

                    
                    bullet.inCollision = true;
                    otherBullet.inCollision = true;

                    
                    QTimer::singleShot(5000, this, [this, i, j]() {
                        if (i < bullets.size()) {
                            if (i < bulletLabels.size()) {
                                delete bulletLabels[i];
                                bulletLabels.remove(i);
                            }
                            bullets.remove(i);
                        }
                        if (j < bullets.size()) {
                            if (j < bulletLabels.size()) {
                                delete bulletLabels[j];
                                bulletLabels.remove(j);
                            }
                            bullets.remove(j);
                        }

                        
                        updateServerBulletsPosition();
                        });
                    break;
                }
            }
        }

       
        if (bullet.x < 0 || bullet.y < 0 || bullet.x >= m_mapWidth * 64 || bullet.y >= m_mapHeight * 64) {
            if (i < bulletLabels.size()) {
                delete bulletLabels[i];
                bulletLabels.remove(i);
            }
            bullets.remove(i);
            --i;
            continue;
        }

        
        if (ix >= 0 && ix < m_mapHeight && iy >= 0 && iy < m_mapWidth) {
            QString cellContent = m_mapData[ix][iy];
            if (cellContent != "Empty") {
                qDebug() << "Bullet hit: " << cellContent << " at (" << ix << ", " << iy << ")";

                
                destroyCells(ix, iy);

                
                updateMap(m_mapArray);

                
                if (i < bulletLabels.size()) {
                    delete bulletLabels[i];
                    bulletLabels.remove(i);
                }
                bullets.remove(i);
                --i;
                continue;
            }
        }
        if (ix >= 0 && ix < m_mapHeight && iy >= 0 && iy < m_mapWidth) {
            QString cellContent = m_mapData[ix][iy];
            if (cellContent == "Player") {
                qDebug() << "Bullet hit a player at (" << ix << ", " << iy << ")";
                handleBulletCollision(ix, iy); 
                delete bulletLabels[i];
                bulletLabels.remove(i);
                bullets.remove(i);
                --i;
                continue;
            }
        }
    }
    updateServerBulletsPosition();
}
void GameWindow::updatePlayerLivesOnServer(const std::string& playerName, int lives) {
    std::string payload = "{\"name\": \"" + playerName + "\", \"lives\": " + std::to_string(lives) + "}";
    QtConcurrent::run([payload,this]() {
        cpr::Response response = cpr::Post(
            cpr::Url{ base_url+"/update_player_lives" },
            cpr::Body{ payload },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (response.status_code != 200) {
        }
        });
}
void GameWindow::destroyCells(int x, int y)
{
    if (m_mapData[x][y] == "Unbreakable")
        return;

    if (m_mapData[x][y] == "Wall")
    {
        m_mapData[x][y] = "Empty";
        updateServerMapCell(x, y);
        fetchMap();
        return;
    }

    if (m_mapData[x][y] == "Player")
    {
        if (!m_shield)
        {
            m_playerLives--;
            displayPlayerLives();

          
            updatePlayerLivesOnServer(m_playerName, m_playerLives);

            if (m_playerLives == 0)
            {
                displayPlayerDeathMessage(m_playerName);
                m_mapData[x][y] = "Empty";

                QString m_playerNameQString = QString::fromStdString(m_playerName);
                if (otherPlayerLabels.contains(m_playerNameQString))
                {
                    otherPlayerLabels[m_playerNameQString]->deleteLater();
                    otherPlayerLabels.remove(m_playerNameQString);
                }

                updateServerMapCell(x, y);
                fetchMap();
                return;
            }
        }
    }

    if (m_mapData[x][y] == "Bomb")
    {
        m_mapData[x][y] = "Empty";

        for (int i = x - 1; i <= x + 1; i++)
        {
            for (int j = y - 1; j <= y + 1; j++)
            {
                if (m_mapData[i][j] == "Wall")
                {
                    m_mapData[i][j] = "Empty";
                    updateServerMapCell(i, j);
                }
                else if (m_mapData[i][j] == "Player")
                {
                    if (!m_shield)
                    {
                        m_playerLives--;
                        displayPlayerLives();

                       
                        updatePlayerLivesOnServer(m_playerName, m_playerLives);

                        if (m_playerLives == 0)
                        {
                            displayPlayerDeathMessage(m_playerName);
                            m_mapData[i][j] = "Empty";

                            QString m_playerNameQString = QString::fromStdString(m_playerName);
                            if (otherPlayerLabels.contains(m_playerNameQString))
                            {
                                otherPlayerLabels[m_playerNameQString]->deleteLater();
                                otherPlayerLabels.remove(m_playerNameQString);
                            }

                            updateServerMapCell(i, j);
                        }
                    }
                }
            }
        }

        updateServerMapCell(x, y);
        fetchMap();
    }
}

void GameWindow::increaseVisibility() {
    visibilityRadius++;
    updateMap(m_mapArray); 
    fetchAllPlayersPositions();
   
}
void GameWindow::sendPlayerSpawn(int row, int col)
{
    cpr::Response response = cpr::Post(
        cpr::Url{ base_url + "/map/player/" + std::to_string(row) + "/" + std::to_string(col) },
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    if (response.status_code != 200) {
    }
}
void GameWindow::fetchAllPlayersPositions() {
    cpr::Response response = cpr::Get(cpr::Url{ base_url + "/get_all_players_positions" });

    if (response.status_code == 200) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        if (!jsonDoc.isObject()) {
            return;
        }

        QJsonObject positionsObj = jsonDoc.object();
        int playerGridX = static_cast<int>(m_x / 64);
        int playerGridY = static_cast<int>(m_y / 64);

        for (auto it = positionsObj.begin(); it != positionsObj.end(); ++it) {
            QString playerName = it.key();
            QJsonObject position = it.value().toObject();
            int x = position["x"].toInt();
            int y = position["y"].toInt();
            int direction = position["direction"].toInt();

            int distance = std::max(abs(playerGridX - x), abs(playerGridY - y));
            if (distance <= visibilityRadius) {
                if (!otherPlayerLabels.contains(playerName)) {
                    QLabel* playerLabel = new QLabel(this);
                    playerLabel->setPixmap(QPixmap(getPlayerTexture(direction)).scaled(64, 64));
                    playerLabel->setGeometry(x * 64, y * 64, 64, 64);
                    playerLabel->show();
                    otherPlayerLabels[playerName] = playerLabel;
                }
                else {
                    otherPlayerLabels[playerName]->setPixmap(QPixmap(getPlayerTexture(direction)).scaled(64, 64));
                    otherPlayerLabels[playerName]->move(x * 64, y * 64);
                    otherPlayerLabels[playerName]->show();
                }
            }
            else {
                if (otherPlayerLabels.contains(playerName)) {
                    otherPlayerLabels[playerName]->hide();
                }
            }
        }

        removeDisconnectedPlayers(positionsObj);
    }
}



void GameWindow::removeDisconnectedPlayers(const QJsonObject& currentPlayers) {
    for (auto it = otherPlayerLabels.begin(); it != otherPlayerLabels.end();) {
        if (!currentPlayers.contains(it.key())) {
            it.value()->deleteLater();
            it = otherPlayerLabels.erase(it);
        }
        else {
            ++it;
        }
    }
}
QString GameWindow::getPlayerTexture(uint8_t direction) {
    switch (direction) {
    case 0: return "PlayerUp.png";
    case 1: return "PlayerDown.png";
    case 2: return "PlayerLeft.png";
    case 3: return "PlayerRight.png";
    default: return "PlayerLeft.png";
    }
}

void GameWindow::handleBulletCollision(int x, int y) {
    QString hitPlayerName = getPlayerAtPosition(x, y);
    if (hitPlayerName.isEmpty()) {
        return;
    }

    if (hitPlayerName == QString::fromStdString(m_playerName)) {
        
        if (!m_shield) {
            m_playerLives--;
            updatePlayerLivesOnServer(m_playerName, m_playerLives);
            displayPlayerLives();

            if (m_playerLives <= 0) {
                displayPlayerDeathMessage(m_playerName);
            }
        }
    }
    else {
        
        std::string playerKey = hitPlayerName.toStdString();
        if (playerLives.find(playerKey) != playerLives.end() && playerLives[playerKey] > 0) {
            playerLives[playerKey]--;
            updatePlayerLivesOnServer(playerKey, playerLives[playerKey]);
        }
    }
}


QString GameWindow::getPlayerAtPosition(int x, int y) {
    for (const auto& player : otherPlayerLabels) {
        QRect playerRect = player->geometry(); 
        int gridX = playerRect.x() / 64;
        int gridY = playerRect.y() / 64;

        if (gridX == x && gridY == y) {
            return otherPlayerLabels.key(player); 
        }
    }
    return "";
}
void GameWindow::displayPlayerDeathMessage(const std::string& playerName) {
    messageLabel->setText(QString::fromStdString(playerName + " died"));
    messageLabel->show();

    
    if (playerName == m_playerName) {
        QTimer::singleShot(5000, this, [this]() {
            });
    }
    else {
        QTimer::singleShot(5000, messageLabel, &QLabel::hide); 
    }
}

void GameWindow::addPlayerPoints(int pointsToAdd) {
    std::string payload = "{\"name\": \"" + m_playerName + "\", \"points\": " + std::to_string(pointsToAdd) + "}";
    cpr::Response response = cpr::Post(
        cpr::Url{ base_url + "/add_player_points" },
        cpr::Body{ payload },
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    if (response.status_code == 200) {
        fetchPlayerPoints(); 
    }
}

void GameWindow::fetchPlayerPoints() {
    cpr::Response response = cpr::Get(cpr::Url{ base_url + "/get_player_points/" + m_playerName });

    if (response.status_code == 200) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            int points = jsonObj["points"].toInt();
            m_playerPoints = points;          
        }
    }
    
}