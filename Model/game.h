#ifndef GAME_H
#define GAME_H

#include <QString>

/**
 * construct a basic game logic
 */
class Game {
public:
    //game board map
    int map[12][18];
    int rawMap[10][16];
    //selected image posn
    QString selectedPic;
    //total images on the board
    int totalPic = 0;
    //determing connecting strategies
    bool flagA = false, flagB = false, flagC = false;

    /**
     * @brief linkWithNoCorner
     * @param pic1
     * @param pic2
     * @return
     */
    bool linkWithNoCorner(QString pic1, QString pic2);
    /**
     * @brief linkWithOneCorner
     * @param pic1
     * @param pic2
     * @param pos2
     * @return
     */
    bool linkWithOneCorner(QString pic1, QString pic2, QString& pos2);
    /**
     * @brief linkWithTwoCorner
     * @param pic1
     * @param pic2
     * @param pos2
     * @param pos3
     * @return
     */
    bool linkWithTwoCorner(QString pic1, QString pic2, QString& pos2, QString& pos3);
    /**
     * @brief useTool
     * @param pic1
     * @param pic2
     */
    void useTool(QString pic1, QString pic2);
    /**
     *deciding if player wins
     */
    bool isWin();
    /**
     * clear inner map
     */
    void clearRawMap();
    /**
     * init game board
     */
    void init();

private:
    /**
     * get posns of two images
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param pic1
     * @param pic2
     */
    void getPosition(int &x1, int &y1, int &x2, int & y2, QString pic1, QString pic2);
    /**
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @return true if no corner between two images
     */
    bool link(int x1, int y1, int x2, int y2);

};
#endif // GAME_H
