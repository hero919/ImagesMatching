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
#include "Dao/scoredao.h"

namespace Ui {
class LevelModeWindow;
}

class LevelModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LevelModeWindow(QWidget *parent = 0);
    void drawLine(QString pic1, QString pic2, QString pos2, QString pos3);
    ~LevelModeWindow();

private:
    Ui::LevelModeWindow *ui;


protected:
    Game gameModel; //model层，用来进行逻辑处理
    QGridLayout *grid;
    double totalTime = 100; //总时间
    double speed = 1; //速率系数
    int PIC_NUM = 5; //花色数
    int LEVEL = 1;
    QTimer *timer;
    QPainter* painter;
    DrawLineLayer* drawLineLayer; //用于画线的layer
    HelpDialog *helpDialog;
//    QSpinBox *box; // 设置时间
//    QSpinBox *box2; // 设置花色数
//    QDialog *changeSpeedDialog;
    ScoreDao *scoreDao;

    void initMap();
    void reset(bool flag);
    void select(const QString &msg); //鼠标点击图片时触发的事件

public slots:
    void startGame();
    void reStartGame();
    void pauseGame();
    void timerUpDate(); //计时器更新
    void resetMap(); //图片重排
    void findHint(); //提示
    void showHelp(); //显示帮助
    void changeSpeed();
    void _changeSpeed();
    void BackToMainPage();

};

#endif // LEVELMODEWINDOW_H
