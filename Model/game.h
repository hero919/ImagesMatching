#ifndef GAME_H
#define GAME_H

#include <QString>

/**
 * construct basic game model
 */
class Game {
public:
    //board frame
    int map[12][18];
    //track selected image
    QString selectedPic;
    //track images on the board
    int totalPic = 0;
    //track corners
    bool flagA = false, flagB = false, flagC = false;
    //board frame
    int rawMap[10][16];

    //track image connection
    bool linkWithNoCorner(QString pic1, QString pic2);
    bool linkWithOneCorner(QString pic1, QString pic2, QString& pos2);
    bool linkWithTwoCorner(QString pic1, QString pic2, QString& pos2, QString& pos3);

    //tool available to use
    void useTool(QString pic1, QString pic2);
    //decide whether the player wins the game
    bool isWin();
    //clear board frame
    void clearRawMap();
    //init board frame
    void init();

private:
    void getPosition(int &x1, int &y1, int &x2, int & y2, QString pic1, QString pic2);
    bool link(int x1, int y1, int x2, int y2);

};
#endif // GAME_H
