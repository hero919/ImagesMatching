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
}

LevelModeWindow::~LevelModeWindow()
{
    delete ui;
}


void LevelModeWindow::initMap(){
    //Initialize the rawMap
    for(unsigned int i = 0; i < 10; i++){
        for(unsigned int j = 0 ; j < 16; j++){
            gameModel.rawMap[i][j] = gameModel.totalPic++ / (PIC_NUM + LEVEL * 2) + 1;
        }
    }

    //Initialize the map
    for(unsigned int i = 0; i < 10; i++){
        for(unsigned int j = 0; j < 16; j++){
            gameModel.map[i + 1][j + 1] = gameModel.rawMap[i][j];
        }
    }
}


void LevelModeWindow::timerUpDate(){
    totleTime -= speed;
    ui->NumberClock->display(totleTime);
}




void LevelModeWindow::startGame(){
    reset(true);
}

void LevelModeWindow::pauseGame(){
    if(timer->isActive()){
        timer->stop();
        ui->pushButton->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
        ui->pushButton_7->setDisabled(true);
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
        ui->pushButton_7->setDisabled(false);
        ui->pushButton_8->setDisabled(false);
        ui->picWidget->setDisabled(false);
        ui->pushButton_2->setText("Pause");
    }
}


void LevelModeWindow::findHint(){

}


void LevelModeWindow::resetMap(){

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
    if(flag){
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
    //Reorder the graph
    srand((int)time(nullptr));
    int x1, y1, x2, y2;
    //Shuffle the Numbers
    for(unsigned int i = 0; i < 300; i++){
        x1 = random() % 16;
        y1 = random() % 10;
        x2 = random() % 16;
        y2 = random() % 10;

        int temp = gameModel.rawMap[x1][y1];
        gameModel.rawMap[x1][y1] = gameModel.rawMap[x2][y2];
        gameModel.rawMap[x2][y2] = temp;
    }


    //Fill Images
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
     MapButton *sb = ui->picWidget->findChild<MapButton*>(msg);
      if (sb == NULL) {
          return;
      }
      QString selected = gameModel.selectedPic;
      if(selected  == sb->objectName()){
            sb->setChecked(false);
            gameModel.selectedPic = "";
            return;
      }

      if(selected  == ""){
          gameModel.selectedPic = sb->objectName();

      }


}

void LevelModeWindow::reStartGame(){

}

void LevelModeWindow::_changeSpeed(){

}


