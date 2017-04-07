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
    Game gameModel; //model层，用来进行逻辑处理
    QGridLayout *grid;
    double totalTime = 100; //Time
    double speed = 0.5; //velocity
    int PIC_NUM = 8; //pattern
    QTimer *timer;
    QPainter* painter;
    DrawLineLayer* drawLineLayer;
    HelpDialog *helpDialog;
    QSpinBox *box; // 设置时间
    QSpinBox *box2; // 设置花色数
    QDialog *changeSpeedDialog;
    ScoreDao *scoreDao;

    void initMap();
    void reset(bool flag);
    void select(const QString &msg); //MouseEvent

public slots:
    void startGame();
    void reStartGame();
    void pauseGame();
    void timerUpDate();
    void resetMap();
    void findHint();
    void showHelp();
    void changeSpeed();
    void _changeSpeed();
    void BackToMainPage();

};

#endif // BASICMODEWINDOW_H
