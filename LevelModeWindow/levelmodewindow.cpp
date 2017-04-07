#include "levelmodewindow.h"
#include "ui_levelmodewindow.h"
#include "Model/game.h"
#include "MainWindow/mainwindow.h"
#include <random>
#include <iostream>
#include <QString>
#include "Model/mapbutton.h"
#include <QMessageBox>
#include <QTime>
#include <QDoubleSpinBox>

LevelModeWindow::LevelModeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LevelModeWindow)
{
    ui->setupUi(this);
    setWindowTitle("Level Mode");
    gameModel.init();
    grid = new QGridLayout(ui->picWidget);
    timer = new QTimer(this);
    timer ->start(1000);
    painter = new QPainter(this);
    drawLineLayer = new DrawLineLayer(this);
    drawLineLayer->hide();
    drawLineLayer->setGeometry(QRect(0, 0, 720, 480));
    QFont font;
    font.setBold(true);
    font.setPointSize(36);
    ui->ScoresLabel->setFont(font);
    ui->ScoresLabel->setText("Scores: ");
    font.setBold(false);
    ui->Scores->setFont(font);
    ui->Scores->setText(QString::number(scores));
    font.setPointSize(24);
    QString showLevel = "";
    showLevel.append("Level: ");
    showLevel.append(QString::number(LEVEL));
    ui->ShowLevels->setFont(font);
    ui->ShowLevels->setText(showLevel);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(startGame()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(pauseGame()));
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate())); //Connect timer and update function
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(findHint()));
    connect(ui->pushButton_4, SIGNAL(clicked(bool)), this, SLOT(resetMap()));
    connect(ui->pushButton_5, SIGNAL(clicked(bool)), this, SLOT(changeSpeed()));
    connect(ui->pushButton_6, SIGNAL(clicked(bool)), this, SLOT(showHelp()));
    connect(ui->BackToMain, SIGNAL(clicked(bool)), this, SLOT(BackToMainPage()));
    startGame();
}

LevelModeWindow::~LevelModeWindow()
{
    delete ui;
}


void LevelModeWindow::initMap(){
//    //Initialize the rawMap
//    for(unsigned int i = 0; i < 10; i++){
//        for(unsigned int j = 0 ; j < 16; j++){
//            gameModel.rawMap[i][j] = gameModel.totalPic++ / (PIC_NUM + LEVEL * 2) + 1;
//        }
//    }


    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 16; j++) {
            gameModel.rawMap[i][j] = gameModel.totalPic++ % (PIC_NUM + LEVEL * 2) + 1; //初始化未经打乱的棋盘
        }
    }

    reset(false); //shuffle rawMap
}


void LevelModeWindow::timerUpDate(){
    totalTime -= speed;
    ui->NumberClock->display(totalTime);
    if(totalTime == 0){
        QMessageBox *box = new QMessageBox(this);
        box->setInformativeText("Time is UP!!!");
        box->setStyleSheet("QLabel{height: 100px;min-height: 100px; max-height: 100px; width: 100px;min-width: 100px; max-wdith: 100px;}");
        box->show();
        timer->stop();
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
    }
}




void LevelModeWindow::startGame(){
    initMap(); //初始化游戏棋盘
    totalTime = 100;
    timer->start(1000); //开始计时，时间间隔为1000ms
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton->setText("Restart");
    //如果pushButton之前绑定了startGame方法, 就先解除绑定，然后绑定reStartGame方法
    if (disconnect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(startGame())))
        connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(reStartGame()));
}


void LevelModeWindow::pauseGame(){
    if(timer->isActive()){
        timer->stop();
        ui->pushButton->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
        //ui->pushButton_7->setDisabled(true);
        ui->pushButton_8->setDisabled(true);
        ui->picWidget->setDisabled(true);
        ui->pushButton_2->setText("Consume");
    }else{
        timer->start();
        ui->pushButton->setDisabled(false);
        ui->pushButton_3->setDisabled(false);
        ui->pushButton_4->setDisabled(false);
        ui->pushButton_5->setDisabled(false);
        ui->pushButton_6->setDisabled(false);
        //ui->pushButton_7->setDisabled(false);
        ui->pushButton_8->setDisabled(false);
        ui->picWidget->setDisabled(false);
        ui->pushButton_2->setText("Pause");
    }
}


void LevelModeWindow::findHint(){
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


void LevelModeWindow::resetMap(){
    auto children = ui->picWidget->children();
    for (int i = 1; i < 217; i++) {
        if (children[i]->objectName() != "") {
            grid->removeWidget((QWidget*)children[i]);
            children[i]->deleteLater();
        }
    }

    reset(true);
}


void LevelModeWindow::changeSpeed(){

}

void LevelModeWindow::showHelp(){
    helpDialog->showHelpDialog();
}

void LevelModeWindow::BackToMainPage(){
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->hide();
}

//If flag is true, it means totally restarted the whole game,
//If it is false, it only reset the images but keep the deleted images.
void LevelModeWindow::reset(bool flag){
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
                connect(pic, &MapButton::keyClicked, this, &LevelModeWindow::select);
                gameModel.map[i][j] = randomPicIndex;
            }
            grid->addWidget(pic, i, j);
        }
    }
}

void LevelModeWindow::select(const QString &msg){
    QString pos2, pos3;
    MapButton *sb = ui->picWidget->findChild<MapButton*>(msg);
    if (sb != NULL) {
        if (gameModel.selectedPic == sb->objectName()) {
            sb->setChecked(false);
            gameModel.selectedPic = "";

        } else if (gameModel.selectedPic == "") {
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
            scores += 10;
            ui->Scores->setText(QString::number(scores));

            gameModel.selectedPic = "";


            //在消子之后判断是否获胜
            if (gameModel.isWin()){
                QMessageBox *box = new QMessageBox(this);
                box->setInformativeText("Congratulations！");
                box->show();
                timer->stop();
                ui->pushButton_2->setEnabled(false);
                ui->pushButton_3->setEnabled(false);
                ui->pushButton_4->setEnabled(false);
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

void LevelModeWindow::reStartGame(){
    auto children = ui->picWidget->children();
    for (int i = 1; i < 217; i++) {
        if (children[i]->objectName() != "") {
            grid->removeWidget((QWidget*)children[i]);
            children[i]->deleteLater();
        }
    }
    startGame();
}

void LevelModeWindow::_changeSpeed(){

}


void LevelModeWindow::drawLine(QString pic1, QString pic2, QString pos2, QString pos3) {
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


