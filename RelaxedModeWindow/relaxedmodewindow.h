#ifndef RELAXEDMODEWINDOW_H
#define RELAXEDMODEWINDOW_H

#include "MainWindow/mainwindow.h"
#include <QGridLayout>
#include <QString>
#include "Model/game.h"
#include <QTimer>
#include <QPainter>
#include "Model/DrawLineLayer.h"
#include <HelpDialog/helpdialog.h>
#include <QSpinBox>
#include "Dao/scoredao.h"
#include "Model/mapbutton.h"
#include <QSet>
#include <QMainWindow>
#include <QSound>

namespace Ui {
class RelaxedModeWindow;
}

class RelaxedModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RelaxedModeWindow(QWidget *parent = 0);
    void reset(bool flag);
    void initMap();
    void select(const QString &msg); //鼠标点击图片时触发的事件
    void drawLine(QString pic1, QString pic2, QString pos2, QString pos3);
    ~RelaxedModeWindow();

private:
    Ui::RelaxedModeWindow *ui;
    int credit = 0;
    int creditIncrement = 0;
    int toolsNum = 0;
    bool isUsingTool = false;
    Game gameModel; //model层，用来进行逻辑处理
    QGridLayout *grid;
    double totleTime = 100; //总时间
    double speed = 0.5; //速率系数
    int PIC_NUM = 8; //花色数
    QTimer *timer;
    QPainter* painter;
    DrawLineLayer* drawLineLayer; //用于画线的layer
    HelpDialog *helpDialog;
    QSpinBox *box; // 设置时间
    QSpinBox *box2; // 设置花色数
    QDialog *changeSpeedDialog;
    ScoreDao *scoreDao;
    MainWindow *mainWindow;
    QSound sound;
   // QSet<QString> tracker;


    void increaseCredit();
    void decreaseCredit(int num);
    bool creditIsEnoughForReset();
    bool creditIsEnoughForHint();

public slots:
    void startGame();
    void reStartGame();
    void pauseGame();
    void timerUpDate(); //计时器更新
    void resetMap(); //图片重排
    void findHint(); //提示
    void showHelp(); //显示帮助
    //void changeSpeed();
    //void _changeSpeed();
    void useTool(); //使用道具
    void backToMainPage();
};

#endif // RELAXEDMODEWINDOW_H
