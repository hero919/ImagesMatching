#include "basicmodewindow.h"
#include "ui_basicmodewindow.h"
#include "Model/game.h"
#include "MainWindow/mainwindow.h"
#include <random>
#include <iostream>
#include <QString>
#include "Model/mapbutton.h"
#include <QMessageBox>
#include <QTime>
#include <QDoubleSpinBox>


BasicModeWindow::BasicModeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BasicModeWindow)
{
    ui->setupUi(this);
    setWindowTitle("Images Matching Game");
    scoreDao = new ScoreDao();
    scoreDao->init();
    gameModel.init();
    helpDialog = new HelpDialog(ui->picWidget);
    //initialize progressBar
    ui->progressBar->setValue(totalTime);
    //picWidget is the blank area in main window
    //initialize game model
    grid = new QGridLayout(ui->picWidget);

    timer = new QTimer(this);
    painter = new QPainter(this);
    drawLineLayer = new DrawLineLayer(this);
    drawLineLayer->hide();
    drawLineLayer->setGeometry(QRect(0, 0, 720, 480));
    ui->pauseButton->setEnabled(false);
    ui->hintButton->setEnabled(false);
    ui->resetButton->setEnabled(false);
    //Set Signal and slots for buttons
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startGame()));
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SLOT(pauseGame()));
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    connect(ui->hintButton, SIGNAL(clicked(bool)), this, SLOT(findHint()));
    connect(ui->resetButton, SIGNAL(clicked(bool)), this, SLOT(resetMap()));
    connect(ui->settingButton, SIGNAL(clicked(bool)), this, SLOT(changeSpeed()));
    connect(ui->helpButton, SIGNAL(clicked(bool)), this, SLOT(showHelp()));
    connect(ui->BackToMain, SIGNAL(clicked(bool)), this, SLOT(BackToMainPage()));
}

BasicModeWindow::~BasicModeWindow()
{
    delete ui;
    delete timer;
    delete grid;
    delete painter;
}


void BasicModeWindow::BackToMainPage(){
      MainWindow *mainWindow = new MainWindow();
      mainWindow->show();
      this->hide();
}



void BasicModeWindow::startGame() {
    //initialize game board
    initMap();
    totalTime = 100;
    timer->start(1000);
    ui->pauseButton->setEnabled(true);
    ui->hintButton->setEnabled(true);
    ui->resetButton->setEnabled(true);
    ui->settingButton->setEnabled(false);
    ui->startButton->setText("Restart");
    //To connect Restart game, release startGame signal and then connect.
    if (disconnect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startGame())))
        connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(reStartGame()));
}

void BasicModeWindow::reStartGame() {
    //shuffle
    auto children = ui->picWidget->children();
    for (int i = 1; i < 217; i++) {
        if (children[i]->objectName() != "") {
            grid->removeWidget((QWidget*)children[i]);
            children[i]->deleteLater();
        }
    }
    startGame();
}


void BasicModeWindow::resetMap() {
    auto children = ui->picWidget->children();
    for (int i = 1; i < 217; i++) {
        if (children[i]->objectName() != "") {
            grid->removeWidget((QWidget*)children[i]);
            children[i]->deleteLater();
        }
    }

    reset(true);

}

void BasicModeWindow::pauseGame() {
    if (timer->isActive()) {
        ui->pauseButton->setText("Continue");
        timer->stop();
        ui->picWidget->setDisabled(true);
        ui->startButton->setDisabled(true);
        ui->hintButton->setDisabled(true);
        ui->resetButton->setDisabled(true);
    } else {
        ui->pauseButton->setText("Pause Game");
        timer->start();
        ui->picWidget->setDisabled(false);
        ui->startButton->setDisabled(false);
        ui->hintButton->setDisabled(false);
        ui->resetButton->setDisabled(false);
    }
}

void BasicModeWindow::initMap() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 16; j++) {
            gameModel.rawMap[i][j] = gameModel.totalPic++ % PIC_NUM + 1; //初始化未经打乱的棋盘
        }
    }

    reset(false); //shuffle rawMap
}


void BasicModeWindow::select(const QString &msg) {
    QString pos2, pos3;
    MapButton *sb = ui->picWidget->findChild<MapButton*>(msg);
    if (sb != NULL) {
        if (gameModel.selectedPic == sb->objectName()) { //连续点击同一个图片
            sb->setChecked(false);
            gameModel.selectedPic = "";

        } else if (gameModel.selectedPic == "") { //当前未选中任何一个图片
            gameModel.selectedPic = sb->objectName();
        } else if (gameModel.linkWithNoCorner(gameModel.selectedPic, sb->objectName())
                   || gameModel.linkWithOneCorner(gameModel.selectedPic, sb->objectName(), pos2)
                   || gameModel.linkWithTwoCorner(gameModel.selectedPic, sb->objectName(), pos2, pos3)) { //可消去

            drawLine(gameModel.selectedPic, sb->objectName(), pos2, pos3); //画线
            //让两个图片弹起来并消除
            MapButton *p1 = ui->picWidget->findChild<MapButton*>(gameModel.selectedPic);
            MapButton *p2 = ui->picWidget->findChild<MapButton*>(sb->objectName());
            p1->setVisible(false);
            p1->setStyleSheet("background:transparent");
            p2->setVisible(false);
            p2->setStyleSheet("background:transparent");

            gameModel.selectedPic = "";


            //在消子之后判断是否获胜
            if (gameModel.isWin()){
                QMessageBox *box = new QMessageBox(this);
                box->setInformativeText("Congratulations！");
                box->show();
                timer->stop();
                ui->pauseButton->setEnabled(false);
                ui->hintButton->setEnabled(false);
                ui->resetButton->setEnabled(false);

                //向排行榜插入一条记录
                QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
                QString name = time.toString("yyyyMMddhhmm"); //设置显示格式
                int s = (160 - gameModel.totalPic) * 5;
                if (s < 0)
                    s = 0;
                QString score = QString::number(s);
                if (score.length() == 2)
                    score = "0" + score;
                else if (score.length() == 1)
                    score = "00" + score;
                scoreDao->outputItem(name, score);
            }

        } else { //不可消去
            //让原来的pic1弹起来
            MapButton *p1 = ui->picWidget->findChild<MapButton*>(gameModel.selectedPic);
            p1->setChecked(false);
            gameModel.selectedPic = sb->objectName();
            //新的pic1按下去
            sb->setChecked(true);
        }
    }
}

void BasicModeWindow::timerUpDate() {
    totalTime -= speed; //timer每更新一次，总时间减去0.5s
    ui->progressBar->setValue(totalTime); //更新progressBar的值
//    QPalette p();
//    p.setColor(QPalette::Highlight, Qt::green);
//    setPalette(p);

    if (totalTime == 0) {
        //向排行榜插入一条记录
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString name = time.toString("yyyyMMddhhmm"); //设置显示格式
        int s = (160 - gameModel.totalPic) * 5;
        if (s < 0)
            s = 0;
        QString score = QString::number(s);
        if (score.length() == 2)
            score = "0" + score;
        else if (score.length() == 1)
            score = "00" + score;
        scoreDao->outputItem(name, score);

        QMessageBox *box = new QMessageBox(this);
        box->setInformativeText("时间到！");
        box->show();
        ui->startButton->setEnabled(true);
        ui->pauseButton->setEnabled(false);
        ui->hintButton->setEnabled(false);
        ui->resetButton->setEnabled(false);
    }
}


void BasicModeWindow::findHint() {
    QString pos2, pos3;
    QString pic1, pic2;
    int tmp1, tmp2;
    bool success = false;
    for (int i = 0; i < 216 && !success; i++) {
        for (int j = 0; j < 216 && !success && j!=i; j++) {
            if (i % 18 == 0 || i % 18 == 17 || i<18 || i>=198 || j % 18 == 0 || j % 18 == 17 || j<18 || j>=198)
                continue;
            pic1 = QString::number(i);
            pic2 = QString::number(j);

            tmp1 = gameModel.map[i/18][i%18];
            tmp2 = gameModel.map[j/18][j%18];

            if (gameModel.linkWithNoCorner(pic1, pic2)
                               || gameModel.linkWithOneCorner(pic1, pic2, pos2)
                               || gameModel.linkWithTwoCorner(pic1, pic2, pos2, pos3)) {//可消去
                drawLine(pic1, pic2, pos2, pos3);

                success = true;
                gameModel.map[i/18][i%18] = tmp1;
                gameModel.map[j/18][j%18] = tmp2;

                gameModel.totalPic += 2; //还原被减去的图片数
            }

        }
    }
}

void BasicModeWindow::drawLine(QString pic1, QString pic2, QString pos2, QString pos3) {
    MapButton *p1 = ui->picWidget->findChild<MapButton*>(pic1);
    MapButton *p2 = ui->picWidget->findChild<MapButton*>(pic2);
    //画线
    if (gameModel.flagA) { //没有转折点
        drawLineLayer->setPos1(p1->pos());
        drawLineLayer->setPos2(p2->pos());
        gameModel.flagA = false;
    } else if (gameModel.flagB) {
        drawLineLayer->setPos1(p1->pos());
        drawLineLayer->setPos2(ui->picWidget->findChild<MapButton*>(pos2)->pos());
        drawLineLayer->setPos3(p2->pos());
        gameModel.flagB = false;
    } else if (gameModel.flagC) {
        drawLineLayer->setPos1(p1->pos());
        QWidget *tmpP1;
        QWidget *tmpP2;
        tmpP1 = ui->picWidget->findChild<QWidget*>(pos2);
        tmpP2 = ui->picWidget->findChild<QWidget*>(pos3);
        if (tmpP1 != nullptr) {
            QPoint pp1 = tmpP1->pos();
            drawLineLayer->setPos2(pp1);
        }
        if (tmpP2 != nullptr) {
            QPoint pp2 = tmpP2->pos();
            drawLineLayer->setPos3(pp2);
        }
        QPoint pp4 = p2->pos();
        drawLineLayer->setPos4(pp4);
        gameModel.flagC = false;
    }
    drawLineLayer->show();
    QTime t;
    t.start();
    while(t.elapsed()<200) //连线延迟0.2s
        QCoreApplication::processEvents();
    drawLineLayer->clear();
}

void BasicModeWindow::reset(bool flag) {

    if (flag) {
        gameModel.clearRawMap();
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 18; j++) {
                //To avoid i - 1 or j - 1 out of bound
                if (i == 0 || i == 11 || j == 0 || j == 17) {
                    continue;
                }
                gameModel.rawMap[i-1][j-1] = gameModel.map[i][j];
            }
        }
    }
    srand((int)time(nullptr));
    int randx1, randx2, randy1, randy2;
    //将地图中的图片进行300次随机对调，从而打乱棋盘
    for (int k = 0; k < 300; k++) {
        randx1 = random() % 10;
        randx2 = random() % 10;
        randy1 = random() % 16;
        randy2 = random() % 16;
        int tmp = gameModel.rawMap[randx1][randy1];
        gameModel.rawMap[randx1][randy1] = gameModel.rawMap[randx2][randy2];
        gameModel.rawMap[randx2][randy2] = tmp;
    }
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 18; j++) {
            //Add Around Images
            if (i == 0 || i == 11 || j == 0 || j == 17) {
                MapButton *w = new MapButton();
                w->setStyleSheet("background:transparent");
                w->setObjectName(QString::number(i * 18 + j));
                w->setMinimumSize(40, 40);
                w->setMaximumSize(40, 40);
                w->setParent(ui->picWidget);
                grid->addWidget(w, i, j);
                continue;
            }

            int randomPicIndex = gameModel.rawMap[i-1][j-1];
            MapButton *pic = new MapButton();
            //If the image is deleted, then set the randimPicIndex to be 0. Means it is deleted.
            if (randomPicIndex == 0) {
                pic->setStyleSheet("background:transparent");
                //i means the number of rows and j is the number of columns.
                pic->setObjectName(QString::number(i * 18 + j));
                //Set the size of icons to fill the widge
                pic->setIconSize(QSize(40, 40));
                pic->setMinimumSize(40, 40);
                pic->setMaximumSize(40, 40);
                gameModel.map[i][j] = 0;
            } else {
                pic->setIcon(QIcon(":/icon/res/" + QString::number(randomPicIndex) + ".png"));
                pic->setObjectName(QString::number(i * 18 + j));
                pic->setIconSize(QSize(40, 40));
                pic->setMinimumSize(40, 40);
                pic->setMaximumSize(40, 40);
                pic->setCheckable(true);
                connect(pic, &MapButton::keyClicked, this, &BasicModeWindow::select);
                gameModel.map[i][j] = randomPicIndex;
            }
            grid->addWidget(pic, i, j);

        }
    }
}

void BasicModeWindow::showHelp() {
    helpDialog->showHelpDialog();
}

void BasicModeWindow::changeSpeed() {
    QGridLayout *layout = new QGridLayout();
    changeSpeedDialog = new QDialog();
    box = new QSpinBox();
    box->setMaximum(500);
    box->setMinimum(10);
    box->setValue(100/speed);
    box2 = new QSpinBox();
    box2->setMaximum(10);
    box2->setMinimum(5);
    box2->setValue(PIC_NUM);
    QLabel *label = new QLabel("设置总时间（单位s）,最大500, 最小10");
    QLabel *label2 = new QLabel("设置花色数，最小5，最大10");
    layout->addWidget(label,0, 0);
    layout->addWidget(box, 0, 1);
    layout->addWidget(label2, 1, 0);
    layout->addWidget(box2, 1, 1);
    QPushButton *button = new QPushButton("OK");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(_changeSpeed()));
    layout->addWidget(button, 2, 0);
    changeSpeedDialog->setLayout(layout);
    changeSpeedDialog->show();

}

void BasicModeWindow::_changeSpeed() {
    speed = 100.0 / box->value();
    PIC_NUM = box2->value();
    changeSpeedDialog->hide();
}


