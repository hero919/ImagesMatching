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

class BasicModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BasicModeWindow(QWidget *parent = 0);
    void drawLine(QString pic1, QString pic2, QString pos2, QString pos3);
    ~BasicModeWindow();

private:
    Ui::BasicModeWindow *ui;

protected:
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
    QDialog *changeSpeedDialog;
    //ScoreDao *scoreDao;

    void initMap();
    void reset(bool flag);
    void select(const QString &msg); //Connect mouse action

public slots:
    void startGame();
    void reStartGame();
    void pauseGame();
    void timerUpDate();
    void resetMap();
    void findHint();
    void showHelp();
    void BackToMainPage();

};

#endif // BASICMODEWINDOW_H
