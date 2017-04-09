#ifndef GAME_H
#define GAME_H

#include <QString>

class Game {
public:
    //blank board
    int map[12][18];
    //select by player
    QString selectedPic;
    //total pics on the board
    int totalPic = 0;
    bool flagA = false, flagB = false, flagC = false;
    //images
    int rawMap[10][16];

    bool linkWithNoCorner(QString pic1, QString pic2);
    bool linkWithOneCorner(QString pic1, QString pic2, QString& pos2);
    bool linkWithTwoCorner(QString pic1, QString pic2, QString& pos2, QString& pos3);
    void useTool(QString pic1, QString pic2);

    //decide whether player wins
    bool isWin();

    //clear board
    void clearRawMap();
    //init board
    void init();

    //convert image position
    void getPosition(int &x1, int &y1, int &x2, int & y2, QString pic1, QString pic2);
    void getOnePosition(int &x1,int&y1,QString pic1);


private:

    bool link(int x1, int y1, int x2, int y2);

};
#endif // GAME_H
