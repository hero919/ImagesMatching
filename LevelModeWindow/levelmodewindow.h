#ifndef LEVELMODEWINDOW_H
#define LEVELMODEWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QString>
#include "Model/game.h"
#include <QTimer>
#include <QPainter>
#include "Model/DrawLineLayer.h"
#include <HelpDialog/helpdialog.h>
#include <QSpinBox>
#include <QSound>

namespace Ui {
class LevelModeWindow;
}

/**
 *construc a level mode
 */
class LevelModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief LevelModeWindow
     * @param parent
     */
    explicit LevelModeWindow(QWidget *parent = 0);
    /**
     * draw line between selected images
     * @param pic1
     * @param pic2
     * @param pos2
     * @param pos3
     */
    void drawLine(QString pic1, QString pic2, QString pos2, QString pos3);
    /**
     * destructor
     */
    ~LevelModeWindow();

private:
    /**
     * UI
     */
    Ui::LevelModeWindow *ui;


protected:
    /**
     * basic game logic
     */
    Game gameModel;
   /**
    * basic game board
    */
    QGridLayout *grid;
    /**
     * Time Settings
     */
    double totalTime = 400;
    double speed = 1;
    /**
     * Init Level
     */
    int LEVEL = 1;
    /**
     * Timer
     */
    QTimer *timer;
    /**
     * painter used for drawing lines
     */
    QPainter* painter;
    /**
     * DrawLineLayer is used for drawLineLayer
     */
    DrawLineLayer* drawLineLayer;
    /**
     * Create DrawLineLayer for drawing
     */
    HelpDialog *helpDialog;
    /**
     *
     *Scores used for calculate the scores
     */
    int scores = 0;
    QSound sound;
    /**
     * init game board
     */
    void initMap();
    /**
     * shuffle images
     * @param flag
     */
    void reset(bool flag);
    /**
     * image selection logic
     * @param msg
     */
    void select(const QString &msg);

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
    /**
     * find images that can be eliminated for player
     */
    void findHint();
    /**
     * display help dialog
     */
    void showHelp();


    /**
     * navigate user back to main page
     */
    void BackToMainPage();

};

#endif // LEVELMODEWINDOW_H
