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
 * The relaxed mode
 */
class RelaxedModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     *
     */
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
