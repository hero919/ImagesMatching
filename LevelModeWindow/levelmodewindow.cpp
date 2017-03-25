#include "levelmodewindow.h"
#include "ui_levelmodewindow.h"

LevelModeWindow::LevelModeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LevelModeWindow)
{
    ui->setupUi(this);
    setWindowTitle("Level Mode");
    gameModel.init();
    grid = new QGridLayout(ui->picWidget);
    timer = new QTimer(this);
    painter = new QPainter(this);
    drawLineLayer = new DrawLineLayer(this);
    drawLineLayer->hide();
    drawLineLayer->setGeometry(QRect(0, 0, 720, 480));
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(startGame()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(pauseGame()));
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate())); //将timer和timerUpDate方法关联
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



}


void LevelModeWindow::timerUpDate(){
    timer->start(1000);
    totleTime -= speed;
    ui->NumberClock->display(totleTime);


}




void LevelModeWindow::startGame(){

}

void LevelModeWindow::pauseGame(){

}


void LevelModeWindow::findHint(){

}


void LevelModeWindow::resetMap(){

}


void LevelModeWindow::changeSpeed(){

}

void LevelModeWindow::showHelp(){

}

void LevelModeWindow::BackToMainPage(){

}


void LevelModeWindow::reset(bool flag){

}

void LevelModeWindow::select(const QString &msg){

}

void LevelModeWindow::reStartGame(){

}

void LevelModeWindow::_changeSpeed(){

}


