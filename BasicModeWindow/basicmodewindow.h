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
#include <QSound>

namespace Ui {
class BasicModeWindow;
}

/**
 *Create a praactice mode. Some images will be shown when player
 *clicks on startGame().Select two of the images. If both images
 *are the same, those images will be removed.If they are different,
 *nothing will be removed;
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
      destructor
    */
    ~BasicModeWindow();

    /**
     * draw a line between images
     * @brief drawLine
     * @param pic1
     * @param pic2
     * @param pos2
     * @param pos3
     */
    void drawLine(QString pic1, QString pic2, QString pos2, QString pos3);


private:
    /**
     * Ui
     */
    Ui::BasicModeWindow *ui;

protected:
    /**
     * basic game logic model
     */
    Game gameModel;

    /**
     * grid for images
     */
    QGridLayout *grid;

    /**
     * Timer help to decide game status
     */
    double totleTime = 10000000000000;
    QTimer *timer;

    /**
     * 8 Image patterns
     */
    int PIC_NUM = 8;

    /**
     *Paint the line
     */
    QPainter* painter;

    /**
     * draw line between selected same images
     */
    DrawLineLayer* drawLineLayer;

    /**
     * To display help dialog
     */
    HelpDialog *helpDialog;

    /**
     * To disply the BGM
     */
    QSound sound;

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
    void select(const QString &msg);

public slots:
    /**
     * Start the game. Images is displayed.
     */
    void startGame();

    /**
     * Resart the game.
     */
    void reStartGame();

    /**
     * Pause the game.
     */
    void pauseGame();

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
     * navigate player to main page
     */
    void BackToMainPage();
};

#endif // BASICMODEWINDOW_H
