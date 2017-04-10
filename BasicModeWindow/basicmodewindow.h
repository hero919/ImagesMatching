#ifndef BASICMODEWINDOW_H
#define BASICMODEWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QString>
#include "Model/game.h"
#include <QTimer>
#include <QPainter>
#include "Model/DrawLineLayer.h"
#include <HelpDialog/helpdialog.h>
#include <QSpinBox>
#include "Dao/scoredao.h"

namespace Ui {
class BasicModeWindow;
}

/**
 *Create a basic game mode
 */
class BasicModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     *Widget
     * @brief BasicModeWindow
     * @param parent
     */
    explicit BasicModeWindow(QWidget *parent = 0);
    /**
     * draw a line between images
     * @brief drawLine
     * @param pic1
     * @param pic2
     * @param pos2
     * @param pos3
     */
    void drawLine(QString pic1, QString pic2, QString pos2, QString pos3);
    /**
      destructor
    */
    ~BasicModeWindow();

private:
    /**
     * Ui
     */
    Ui::BasicModeWindow *ui;

protected:
    //Game Model used for initialize the whole map and necessary parameters
    Game gameModel;
    QGridLayout *grid;
    double totleTime = 100; //Total time used for timer
    double speed = 0.5; //Time descreasing each time
    int PIC_NUM = 8; //number of different images, it can only be the divisors of 160
    QTimer *timer;
    QPainter* painter;
    //For drawing the line, it has to be seperate because it has been used for many places
    DrawLineLayer* drawLineLayer;
    HelpDialog *helpDialog;
    QDialog *changeSpeedDialog;
    //ScoreDao *scoreDao;

    /**
     * initialize the game board
     */
    void initMap();
    /**
     * shuffle images
     * @param flag
     */
    void reset(bool flag);
    /**
     * mouse event
     * @param msg
     */
    void select(const QString &msg); //Connect mouse action

public slots:
    /**
     * start the game
     */
    void startGame();
    /**
     * resart the game
     */
    void reStartGame();
    /**
     * pause the game
     */
    void pauseGame();
    /**
     * update timer
     */
    void timerUpDate();
    /**
     * shuffle images
     */
    void resetMap();

    void findHint();
    void showHelp();
    void BackToMainPage();

};

#endif // BASICMODEWINDOW_H
