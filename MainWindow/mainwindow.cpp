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

    // Add signals and its corresponding slots

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

//Play Music
void MainWindow::playMusic(){
    //Create icons for paly music
    QPushButton *musicButton = ui->MusicButton;
    musicButton->setIcon(QIcon(":/icon/res/pause.png"));
    musicButton->setIconSize(QSize(30,30));
    sound.play();
    sound.setLoops(-1);
    //Set the same button for different uses
    if(disconnect(ui->MusicButton, SIGNAL(clicked(bool)), this, SLOT(playMusic()))){
        connect(ui->MusicButton, SIGNAL(clicked(bool)), this, SLOT(stopMusic()));
    }
}

void MainWindow::stopMusic(){
    //Set stop button style and size
    QPushButton *musicButton = ui->MusicButton;
    musicButton->setIcon(QIcon(":/icon/res/play.png"));
    musicButton->setIconSize(QSize(30,30));
    sound.stop();
    //Set button different functionality
    if(disconnect(ui->MusicButton, SIGNAL(clicked(bool)), this, SLOT(stopMusic()))){
        connect(ui->MusicButton, SIGNAL(clicked(bool)), this, SLOT(playMusic()));
    }
}

void MainWindow::showBasicModeWindow() {
    //Create a basic mode for showing
    BasicModeWindow *bmw = new BasicModeWindow();
    bmw->setWindowTitle("Basic Mode");
    bmw->show();
    this->hide();
    this->stopMusic();
}

void MainWindow::showRelaxedModeWindow() {
    //Create relaxed mode
    RelaxedModeWindow *rmw = new RelaxedModeWindow();
    rmw->setWindowTitle("Relaxed Mode");
    rmw->show();
    this->hide();
    this->stopMusic();
}

//Create level mode
void MainWindow::showLevelModeWindow() {
    LevelModeWindow *lmw = new LevelModeWindow();
    lmw->setWindowTitle("Level Mode");
    lmw->show();
    this->hide();
    this->stopMusic();
}

//Set up the begin image
void MainWindow::setUpWelcomeWindow() {
    QMainWindow *window = new QMainWindow();
    window->setFixedSize(800,600);
    QCoreApplication::processEvents();

    //Using widget for start image and set label to show the sentences
    QWidget *cenWidget = new QWidget(window);
    window->setCentralWidget(cenWidget);
    QLabel *emptyLabel = new QLabel(cenWidget);
    QLabel *welcomeLabel = new QLabel(cenWidget);
    welcomeLabel->setText("CS3650 Project: Images Matching Game");
    QLabel *nameLabel = new QLabel(cenWidget);
    nameLabel->setText("\t\t by Zeqing Zhang, Jiameng Wang, Zhaoxing Hong");
    QGridLayout *gridLayout = new QGridLayout(cenWidget);
    //Add font style
    QFont ft;
    ft.setPointSize(40);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::black);
    //Set style
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

    //Delay 4 seconds for starting page

    while(t.elapsed()<4000)
        QCoreApplication::processEvents();
    window->close();
}

void MainWindow::showHelp() {
    //Using HelpDialog for showing help dialog
    HelpDialog *helpDialog = new HelpDialog(this);
    helpDialog->showHelpDialog();
}

void MainWindow::showMainWindow() {
    //Check whether this is the first time to open the page
    if(isBack_) {
        //If we entered the page before, we don't need to set up the start page
        ui->setupUi(this);
        playMusic();
    } else {
        setUpWelcomeWindow();
        ui->setupUi(this);
        playMusic();
    }
}

