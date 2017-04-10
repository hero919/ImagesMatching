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

/**
 * construct a relax mode
 */
class RelaxedModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RelaxedModeWindow(QWidget *parent = 0);
    void reset(bool flag);
    void initMap();
    void select(const QString &msg);
    void drawLine(QString pic1, QString pic2, QString pos2, QString pos3);
    ~RelaxedModeWindow();

private:
    Ui::RelaxedModeWindow *ui;
    int credit = 0;
    int creditIncrement = 0;
    int toolsNum = 0;
    bool isUsingTool = false;
    Game gameModel;
    QGridLayout *grid;
    double totleTime = 100;
    double speed = 0.5;
    int PIC_NUM = 8;
    QTimer *timer;
    QPainter* painter;
    DrawLineLayer* drawLineLayer;
    HelpDialog *helpDialog;
    QSpinBox *box;
    QSpinBox *box2;
    QDialog *changeSpeedDialog;
    MainWindow *mainWindow;
    QSound sound;
    QSet<QString> tracker;



    void increaseCredit();
    void decreaseCredit(int num);
    bool creditIsEnoughForReset();
    bool creditIsEnoughForHint();

public slots:
    void startGame();
    void reStartGame();
    void pauseGame();
    void timerUpDate();
    void resetMap();
    void findHint();
    void showHelp();
    void useTool();
    void backToMainPage();
};

#endif // RELAXEDMODEWINDOW_H
