#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Model/game.h"
#include "BasicModeWindow/basicmodewindow.h"
#include "RelaxedModeWindow/relaxedmodewindow.h"
#include "LevelModeWindow/levelmodewindow.h"
#include <QDialog>
#include <QToolBar>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDateTime>
#include <HelpDialog/helpdialog.h>
#include <QStandardItemModel>
#include <vector>
#include <fstream>
#include <QSound>

MainWindow::MainWindow(QWidget *parent, bool isBack) :
    QMainWindow(parent),
    sound(":/music/res/Pokemon01.wav"),
    ui(new Ui::MainWindow),
    isBack_(isBack)
{
    showMainWindow();

    //Add signals and its corresponding slots
    connect(ui->button3, SIGNAL(clicked(bool)), this, SLOT(showBasicModeWindow()));
    connect(ui->button2, SIGNAL(clicked(bool)), this, SLOT(showRelaxedModeWindow()));
    connect(ui->button1, SIGNAL(clicked(bool)), this, SLOT(showLevelModeWindow()));
    connect(ui->HelpButton, SIGNAL(clicked(bool)), this, SLOT(showHelp()));
    connect(ui->MusicButton, SIGNAL(clicked(bool)), this, SLOT(stopMusic()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playMusic(){
    QPushButton *musicButton = ui->MusicButton;
    musicButton->setIcon(QIcon(":/icon/res/pause.png"));
    musicButton->setIconSize(QSize(30,30));
    sound.play();
    if(disconnect(ui->MusicButton, SIGNAL(clicked(bool)), this, SLOT(playMusic()))){
        connect(ui->MusicButton, SIGNAL(clicked(bool)), this, SLOT(stopMusic()));
    }
}

void MainWindow::stopMusic(){
    QPushButton *musicButton = ui->MusicButton;
    musicButton->setIcon(QIcon(":/icon/res/play.png"));
    musicButton->setIconSize(QSize(30,30));
    sound.stop();
    if(disconnect(ui->MusicButton, SIGNAL(clicked(bool)), this, SLOT(stopMusic()))){
        connect(ui->MusicButton, SIGNAL(clicked(bool)), this, SLOT(playMusic()));
    }
}

void MainWindow::showBasicModeWindow() {
    BasicModeWindow *bmw = new BasicModeWindow();
    bmw->setWindowTitle("Basic Mode");
       bmw->show();
       this->hide();
       this->stopMusic();
}

void MainWindow::showRelaxedModeWindow() {
    RelaxedModeWindow *rmw = new RelaxedModeWindow();
    rmw->setWindowTitle("Relaxed Mode");
    rmw->show();
    this->hide();
    this->stopMusic();
}

void MainWindow::showLevelModeWindow() {
    LevelModeWindow *lmw = new LevelModeWindow();
    lmw->setWindowTitle("Level Mode");
    lmw->show();
    this->hide();
    this->stopMusic();
}

void MainWindow::setUpWelcomeWindow() { //Create Welcome Page
    QMainWindow *window = new QMainWindow();
    window->setFixedSize(800,600);
    QCoreApplication::processEvents();

    QWidget *cenWidget = new QWidget(window);
    window->setCentralWidget(cenWidget);
    QLabel *emptyLabel = new QLabel(cenWidget);
    QLabel *welcomeLabel = new QLabel(cenWidget);
    welcomeLabel->setText("CS3650 Project: Images Matching Game");
    QLabel *nameLabel = new QLabel(cenWidget);
    nameLabel->setText("\t\t by Zeqing Zhang, Jiameng Wang, Zhaoxing Hong");
    QGridLayout *gridLayout = new QGridLayout(cenWidget);
    QFont ft;
    ft.setPointSize(40);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    welcomeLabel->setFont(ft);
    welcomeLabel->setPalette(pa);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    ft.setPointSize(24);
    nameLabel->setFont(ft);
    nameLabel->setPalette(pa);
    nameLabel->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(emptyLabel);
    gridLayout->addWidget(welcomeLabel);
    gridLayout->addWidget(nameLabel);

    window->show();
    QTime t;
    t.start();
    while(t.elapsed()<4000) //Delay 4 seconds for starting page
        QCoreApplication::processEvents();
    window->close();
}

void MainWindow::showHelp() {
    HelpDialog *helpDialog = new HelpDialog(this);
    helpDialog->showHelpDialog();
}

void MainWindow::showMainWindow() {
    if(isBack_) {
        ui->setupUi(this);
        playMusic();
    } else {
        setUpWelcomeWindow();
        ui->setupUi(this);
        playMusic();
    }
}

