#include "game.h"
#include <iostream>
#include "BasicModeWindow/basicmodewindow.h"
#include <QList>
#include <QDebug>

using namespace std;

//Test
bool Game::isWin() {
    return totalPic == 0;
}

void Game::getPosition(int &x1, int &y1, int &x2, int & y2, QString pic1, QString pic2) {
    flagA = false;
    flagB = false;
    flagC = false;
    x1 = pic1.toInt() / 18;
    y1 = pic1.toInt() % 18;
    x2 = pic2.toInt() / 18;
    y2 = pic2.toInt() % 18;
}

//Link without corner
bool Game::link(int x1, int y1, int x2, int y2) {
//    int k = 0;
    //If one of x, y is not in the same line
    if (x1 != x2 && y1 != y2) {
        return false;
    }

    //Two Images with different contents
    if (map[x1][y1] != map[x2][y2]) {
        return false;
    }

    //Map to determing wether two images are the same image
    if (map[x1][y1] == 0 || map[x2][y2] == 0) {
        return false;
    }

    if (x1 == x2) {
        for(int y = qMin(y1 + 1, y2 + 1); y < qMax(y1, y2); y++){
             if(map[x1][y] != 0){
                 return false;
             }
        }
    }
    if (y1 == y2) {
        for(int x = qMin(x1 + 1, x2 + 1); x < qMax(x1, x2); x++){
             if(map[x][y1] != 0){
                 return false;
             }
        }
    }
    return true;
}



bool Game::linkWithNoCorner(QString pic1, QString pic2) {
    int x1, x2, y1, y2;
    //Put value to different variables.
    getPosition(x1, y1, x2, y2, pic1, pic2);

    bool result = link(x1, y1, x2, y2);
    if (result == true) {
        map[x1][y1] = 0;
        map[x2][y2] = 0;
        totalPic -= 2;
    }
    flagA = true;
    return result;
}

//There are two possible cases in one corner
//Top corner and bot corner
bool Game::linkWithOneCorner(QString pic1, QString pic2, QString& pos2) {
    int x1, x2, y1, y2;
    getPosition(x1, y1, x2, y2, pic1, pic2);

    //If it has corner, it can't be in the same line
    if (x1 == x2 || y1 == y2) {
        return false;
    }
    //If they are not the same image
    if (map[x1][y1] != map[x2][y2]) {
        return false;
    }


    bool planA = false, planB = false;
    //top corner
    //Plan A
    //  A ----
    //       |
    //       |
    //       A
    if (map[x1][y2] == 0) {
        //To use link function, use the corner image for tracking whether two images can be lineked
        map[x1][y2] = map[x1][y1];
        if (link(x1, y2, x1, y1) && link(x1, y2, x2, y2)) {
            planA = true;
            pos2 = QString::number(x1*18 + y2);
            map[x1][y1] = 0;
            map[x2][y2] = 0;
        }
        //Push the value back
        map[x1][y2] = 0;
    }

    //Bottom corner
    //Plan B
    //B
    //|
    //|
    //------- B
    if (map[x2][y1] == 0) {
        map[x2][y1] = map[x1][y1];
        if (link(x2, y1, x1, y1) && link(x2, y1, x2, y2)) {
            planB = true;
            pos2 = QString::number(x2*18 + y1);
            map[x1][y1] = 0;
            map[x2][y2] = 0;
        }
        map[x2][y1] = 0;
    }

    if (planA || planB) {
        totalPic -= 2;
        flagB = true;
        return true;
    }
    else{
        //Images not matchingss
        return false;
    }
}


//Link two images with two corners
bool Game::linkWithTwoCorner(QString pic1, QString pic2, QString& pos2, QString& pos3) {
    //Set values for integer
    int x1, x2, y1, y2;
    getPosition(x1, y1, x2, y2, pic1, pic2);

    //Basic condition check
    if (map[x1][y1] != map[x2][y2]) {
        return false;
    }


    bool planA = false, planB = false, planC = false, planD = false;
    //--------
    //|      |
    //|      |
    //C      |
    //       |
    //       C
    for (int i = x1 - 1; i >= 0; i--) {
        //Doing top match and check one by one
        if (map[i][y1] != 0){
            break;
        }

        map[i][y1] = map[x1][y1];
        QString p1 = QString::number(i*18 + y1);
        QString p2 = QString::number(x2*18 + y2);

        //Applied Linekd with one corner to check the upper case

        if (linkWithOneCorner(p1, p2, pos2)) {
            flagB = false;
            planA = true;
            map[x1][y1] = 0;
            map[x2][y2] = 0;
            totalPic += 2;
            QString tmp = pos2;
            pos2 = p1;
            pos3 = tmp;
            break;
        }
        map[i][y1] = 0;
    }

    // ---------- C
    // |
    // |
    // |
    // |
    // -------C
    for (int i = y1 - 1; i >= 0; i--) {
        if (map[x1][i] != 0)
            break;

        map[x1][i] = map[x1][y1];
        QString p1 = QString::number(x1*18 + i);
        QString p2 = QString::number(x2*18 + y2);

        //Applied linkWithOneCorner helper function
        if (linkWithOneCorner(p1, p2, pos2)) {
            flagB = false;
            planA = true;
            map[x1][y1] = 0;
            map[x2][y2] = 0;
            totalPic += 2;

            QString tmp = pos2;
            pos2 = p1;
            pos3 = tmp;
            break;
        }
        map[x1][i] = 0;
    }

    // C --------
    //          |
    //          |
    //   C-------
    for (int i = y1 + 1; i < 18; i++) {
        if (map[x1][i] != 0)
            break;
        map[x1][i] = map[x1][y1];
        QString p1 = QString::number(x1*18 + i);
        QString p2 = QString::number(x2*18 + y2);
        if (linkWithOneCorner(p1, p2, pos2)) {
            flagB = false;
            planA = true;
            map[x1][y1] = 0;
            map[x2][y2] = 0;
            totalPic += 2;

            QString tmp = pos2;
            pos2 = p1;
            pos3 = tmp;
            break;
        }
        map[x1][i] = 0;
    }

    // C
    // |
    // |             C
    // |             |
    // |             |
    // |-------------|
    for (int i = x1 + 1; i < 12; i++) {
        if (map[i][y1] != 0)
            break;
        map[i][y1] = map[x1][y1];
        QString p1 = QString::number(i*18 + y1);
        QString p2 = QString::number(x2*18 + y2);
        if (linkWithOneCorner(p1, p2, pos2)) {
            flagB = false;
            planA = true;
            map[x1][y1] = 0;
            map[x2][y2] = 0;
            totalPic += 2;
            QString tmp = pos2;
            pos2 = p1;
            pos3 = tmp;
            break;
        }
        map[i][y1] = 0;
    }

    if (planA || planB || planC || planD) {
        totalPic -= 2;
        flagC = true;
        return true;
    }
    else {
        return false;
    }

}


void Game::useTool(QString pic1, QString pic2) {
    int x1, x2, y1, y2;
    getPosition(x1, y1, x2, y2, pic1, pic2);
    map[x1][y1] = 0;
    map[x2][y2] = 0;
    totalPic -= 2;
}

void Game::clearRawMap() {
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 16; j++){
            rawMap[i][j] = 0;
        }
    }
}

void Game::init() {
    for(int i = 0; i < 12; i++){
        for(int j = 0; j < 18; j++){
            map[i][j] = 0;
        }
    }
}
