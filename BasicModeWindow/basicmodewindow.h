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

/**
 *Create a basic game mode
 */
namespace Ui {
class BasicModeWindow;
}

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
<<<<<<< HEAD
    //create basic game board
    Game gameModel;
    QGridLayout *grid;
    //time
    double totalTime = 100;
    double speed = 0.5;
    //pattern
    int PIC_NUM = 8;
    QTimer *timer;
    QPainter* painter;
    DrawLineLayer* drawLineLayer;
    HelpDialog *helpDialog;
    // Time box
    QSpinBox *box;
    // Pattern box
    QSpinBox *box2;
=======
    Game gameModel; //Game Model used for initialize the whole map and necessary parameters
    QGridLayout *grid;
    double totleTime = 100; //Total time used for timer
    double speed = 0.5; //Time descreasing each time
    int PIC_NUM = 8; //number of different images, it can only be the divisors of 160
    QTimer *timer;
    QPainter* painter;
    //For drawing the line, it has to be seperate because it has been used for many places
    DrawLineLayer* drawLineLayer;
    HelpDialog *helpDialog;
>>>>>>> origin/master
    QDialog *changeSpeedDialog;
    //ScoreDao *scoreDao;

    /**
     * initialize the game
     */
    void initMap();
    /**
     * shuffle images
     * @param flag
     */
    void reset(bool flag);
<<<<<<< HEAD
    /**
     * mouse event
     * @param msg
     */
    void select(const QString &msg);
=======
    void select(const QString &msg); //Connect mouse action
>>>>>>> origin/master

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
<<<<<<< HEAD
    /**
     * update timer
     */
    void timerUpDate();
    /**
     * shuffle images
     */
    void resetMap();
    /**
     * find images that can be eliminated
     */
    void findHint();
    /**
     * display help dialog
     */
    void showHelp();
    /**
     * settings
     */
    void changeSpeed();
    void _changeSpeed();
    /**
     * navigate player to main page
     */
=======
    void timerUpDate();
    void resetMap();
    void findHint();
    void showHelp();
>>>>>>> origin/master
    void BackToMainPage();

};

#endif // BASICMODEWINDOW_H
