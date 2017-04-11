#ifndef RELAXEDMODEWINDOW_H
#define RELAXEDMODEWINDOW_H

#include <QGridLayout>
#include <QString>
#include <QMainWindow>
#include <QSound>
#include <QTimer>
#include <QPainter>
#include <HelpDialog/helpdialog.h>
#include <QSpinBox>

#include "Model/game.h"
#include "Model/DrawLineLayer.h"
#include "MainWindow/mainwindow.h"
#include "Model/mapbutton.h"


namespace Ui {
class RelaxedModeWindow;
}

/**
 * The relaxed mode not only realizes all of functions from the beginer mode,
 * but also adds a timer to limit the time the player uses to finish the matching game.
 * In addition, as the player conducts the game,
 * he will find some tools to connect any images without constraints.
 */
class RelaxedModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Construct a relaced window
     */
    explicit RelaxedModeWindow(QWidget *parent = 0);

    /**
     * Destructor
     */
    ~RelaxedModeWindow();

    /**
     * Construct a relaced window
     */
    void reset(bool flag);

    /**
     * Construct a relaced window
     */
    void initMap();

    /**
     * Events in terms of pictures selection
     * @param message
     */
    void select(const QString &msg);

    /**
     * Draw a line to connect two pictures
     * @param the first picture
     * @param the second picture
     * @param the position of the first picture
     * @param the position of the second picture
     */
    void drawLine(QString pic1, QString pic2, QString pos2, QString pos3);

private:

    /**
     * Ui for the relaxedModeWindow
     */
    Ui::RelaxedModeWindow *ui;

    /**
     * The current score
     */
    int credit = 0;

    /**
     * The accumulaive score
     */
    int creditIncrement = 0;

    /**
     * The number of tools
     */
    int toolsNum = 0;

    /**
     * Determine if the player is using a tool
     */
    bool isUsingTool = false;

    /**
     * The game model
     */
    Game gameModel;

    /**
     * The grid of layout
     */
    QGridLayout *grid;

    /**
     * The remaining time
     */
    double totleTime = 100;

    /**
     * the speed of progess bar
     */
    double speed = 0.5;

    /**
     * 8 picture patterns
     */
    int PIC_NUM = 8;

    /**
     * The timer of the relaxed mode
     */
    QTimer *timer;

    /**
     * The painter
     */
    QPainter* painter;

    /**
     * The line layer
     */
    DrawLineLayer* drawLineLayer;

    /**
     * The game instruction
     */
    HelpDialog *helpDialog;

    /**
     * The speed of progess bar
     */

    /**
     * The music track
     */
    QSound sound;

    /**
     * Increase the score
     */
    void increaseCredit();

    /**
     * Reduce the score give a number
     * @param the number of scores to be reduced
     */
    void decreaseCredit(int num);

    /**
     * Determine if the player has enough scores to reset
     */
    bool creditIsEnoughForReset();

    /**
     * Determine if the player has enough scores to use the hint
     */
    bool creditIsEnoughForHint();

public slots:

    /**
     * Start a game
     */
    void startGame();

    /**
     * Restart a game
     */
    void reStartGame();

    /**
     * Pause the game
     */
    void pauseGame();

    /**
     * Update the timer
     */
    void timerUpDate();

    /**
     * Reset the map
     */
    void resetMap();

    /**
     * Find a hint
     */
    void findHint();

    /**
     * Show the instruction
     */
    void showHelp();

    /**
     * Use a tool
     */
    void useTool();

    /**
     * Go back to the main page
     */
    void backToMainPage();
};

#endif // RELAXEDMODEWINDOW_H
