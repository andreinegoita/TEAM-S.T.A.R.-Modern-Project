#include "GUI.h"
#include <QtWidgets/QApplication>
#include<qlabel.h>
#include "GameMapQt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GameMap gameMap(10, 10);
    gameMap.m_map[2][2] = CellType::Player;
    gameMap.m_map[4][4] = CellType::BREAKABLE_WALL;
    gameMap.m_map[6][6] = CellType::UNBREAKABLE_WALL;

    GameWidget gameWidget(gameMap);
    gameWidget.resize(400, 400);
    gameWidget.setWindowTitle("Harta Jocului cu Texturi");
    gameWidget.show();



    GUI w;
    w.show();
    return a.exec();
}
