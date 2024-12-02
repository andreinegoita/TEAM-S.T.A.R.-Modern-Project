#include "Celltype.h"
#include<vector>
#include<qwidget.h>
#include<qpainter.h>

class GameMap {
public:
    GameMap(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {
        m_map.resize(rows, std::vector<CellType>(cols, CellType::EMPTY));
    }

    size_t m_rows;
    size_t m_cols;
    std::vector<std::vector<CellType>> m_map;
};

class GameWidget : public QWidget {
    GameMap& gameMap;

public:
    GameWidget(GameMap& map, QWidget* parent = nullptr) : QWidget(parent), gameMap(map) {}

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        int cellSize = 40;

        for (size_t row = 0; row < gameMap.m_rows; ++row) {
            for (size_t col = 0; col < gameMap.m_cols; ++col) {
                QRect cellRect(col * cellSize, row * cellSize, cellSize, cellSize);
                QPixmap texture;

                switch (gameMap.m_map[row][col]) {
                case CellType::EMPTY:
                    texture.load("textures/empty.png");
                    break;
                case CellType::BREAKABLE_WALL:
                    texture.load("Breakable.png");
                    break;
                case CellType::UNBREAKABLE_WALL:
                    texture.load("Unbreakable.png");
                    break;
                case CellType::Player:
                    texture.load("textures/player.png");
                    break;
                case CellType::Bullet:
                    texture.load("textures/bullet.png");
                    break;
                case CellType::Bomb:
                    texture.load("textures/bomb.png");
                    break;
                }

                if (!texture.isNull()) {
                    painter.drawPixmap(cellRect, texture);
                }
            }
        }
    }
};
