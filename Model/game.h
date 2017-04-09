#ifndef GAME_H
#define GAME_H

#include <QString>

/**
 * construct basic game model
 */
class Game {
public:
    int map[12][18];
    QString selectedPic;

    int totalPic = 0;
    bool flagA = false, flagB = false, flagC = false;
    int rawMap[10][16];

    bool linkWithNoCorner(QString pic1, QString pic2);
    bool linkWithOneCorner(QString pic1, QString pic2, QString& pos2);
    bool linkWithTwoCorner(QString pic1, QString pic2, QString& pos2, QString& pos3);
    void useTool(QString pic1, QString pic2);
    bool isWin();
    void clearRawMap();
    void init();


protected:
    bool linkWithNoCorner(int &x1, int &y1, int &x2, int & y2);

private:
    void getPosition(int &x1, int &y1, int &x2, int & y2, QString pic1, QString pic2);
    bool link(int x1, int y1, int x2, int y2);

};
#endif // GAME_H
