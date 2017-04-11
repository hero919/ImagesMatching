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
    ui(new Ui::BasicModeWindow),
    sound(":/music/res/Pokemon02.wav")
{
    ui->setupUi(this);
    setWindowTitle("Images Matching Game");
    sound.play();
    gameModel.init();
    helpDialog = new HelpDialog(ui->picWidget);

    // PicWidget is now blank

    grid = new QGridLayout(ui->picWidget);

    // Set basic game board

    timer = new QTimer(this);
    painter = new QPainter(this);
    drawLineLayer = new DrawLineLayer(this);
    drawLineLayer->hide();
    drawLineLayer->setGeometry(QRect(0, 0, 720, 480));
    ui->pauseButton->setEnabled(false);
    ui->hintButton->setEnabled(false);
    ui->resetButton->setEnabled(false);

    // Set Sinal and slots for buttons

    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startGame()));
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SLOT(pauseGame()));
    connect(ui->hintButton, SIGNAL(clicked(bool)), this, SLOT(findHint()));
    connect(ui->resetButton, SIGNAL(clicked(bool)), this, SLOT(resetMap()));
    connect(ui->helpButton, SIGNAL(clicked(bool)), this, SLOT(showHelp()));
    connect(ui->BackToMain, SIGNAL(clicked(bool)), this, SLOT(BackToMainPage()));
}

BasicModeWindow::~BasicModeWindow()
{
    delete ui;
    delete grid;
    delete painter;
}


void BasicModeWindow::BackToMainPage(){
    sound.stop();
    timer->stop();
    MainWindow *mw = new MainWindow(0, 1);
    mw->show();
    this->hide();
}



void BasicModeWindow::startGame() {
    initMap();
    totleTime = 1000000000;
    ui->pauseButton->setEnabled(true);
    ui->hintButton->setEnabled(true);
    ui->resetButton->setEnabled(true);
    ui->startButton->setText("Restart");

    // Set the different use for start game button

    if (disconnect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startGame())))
        connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(reStartGame()));
}

void BasicModeWindow::reStartGame() {
    auto children = ui->picWidget->children();

    // Delete all the images and restart

    for (int i = 1; i < 217; i++) {
        if (children[i]->objectName() != "") {
            grid->removeWidget((QWidget*)children[i]);
            children[i]->deleteLater();
        }
    }
    startGame();
}


void BasicModeWindow::resetMap() {

    // Reset map by deleting all the exsisting map and then reset

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

    // Disable some buttons when pause the game

    if (timer->isActive()) {
        ui->pauseButton->setText("Resume");
        timer->stop();
        ui->picWidget->setDisabled(true);
        ui->startButton->setDisabled(true);
        ui->hintButton->setDisabled(true);
        ui->resetButton->setDisabled(true);
    } else {
        ui->pauseButton->setText("Pause");
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

            // Set up different images

            gameModel.rawMap[i][j] = gameModel.totalPic++ % PIC_NUM + 1;
        }
    }

    // Shuffle rawMap

    reset(false);
}


void BasicModeWindow::select(const QString &msg) {
    QString pos2, pos3;
    MapButton *sb = ui->picWidget->findChild<MapButton*>(msg);
    if (sb != NULL) {
        if (gameModel.selectedPic == sb->objectName()) {
            sb->setChecked(false);
            gameModel.selectedPic = "";

            // If not choose the image

        } else if (gameModel.selectedPic == "") {
            gameModel.selectedPic = sb->objectName();

            // If the image can be deleted

        } else if (gameModel.linkWithNoCorner(gameModel.selectedPic, sb->objectName())
                   || gameModel.linkWithOneCorner(gameModel.selectedPic, sb->objectName(), pos2)
                   || gameModel.linkWithTwoCorner(gameModel.selectedPic, sb->objectName(), pos2, pos3)) {

            // Apply draw line function

            drawLine(gameModel.selectedPic, sb->objectName(), pos2, pos3);

            MapButton *p1 = ui->picWidget->findChild<MapButton*>(gameModel.selectedPic);
            MapButton *p2 = ui->picWidget->findChild<MapButton*>(sb->objectName());
            p1->setVisible(false);
            p1->setStyleSheet("background:transparent");
            p2->setVisible(false);
            p2->setStyleSheet("background:transparent");

            gameModel.selectedPic = "";

            // Check whether it is the last pair

            if (gameModel.isWin()){
                QMessageBox *box = new QMessageBox(this);
                box->setInformativeText("Congratulations！");
                box->show();
                ui->pauseButton->setEnabled(false);
                ui->hintButton->setEnabled(false);
                ui->resetButton->setEnabled(false);

            }

        } else {

            // If not match

            MapButton *p1 = ui->picWidget->findChild<MapButton*>(gameModel.selectedPic);
            p1->setChecked(false);
            gameModel.selectedPic = sb->objectName();

            // Set new image
            sb->setChecked(true);
        }
    }
}


void BasicModeWindow::findHint() {
    QString pos2, pos3;
    QString pic1, pic2;
    int tmp1, tmp2;
    bool success = false;
    for (int i = 0; i < 216 && !success; i++) {
        for (int j = 0; j < 216 && !success && j!=i; j++) {

            // Around image should be invisivle and set to 0

            if (i % 18 == 0 || i % 18 == 17 || i<18 || i>=198 || j % 18 == 0 || j % 18 == 17 || j<18 || j>=198)
                continue;
            pic1 = QString::number(i);
            pic2 = QString::number(j);

            tmp1 = gameModel.map[i/18][i%18];
            tmp2 = gameModel.map[j/18][j%18];

            // If it can be linked

            if (gameModel.linkWithNoCorner(pic1, pic2)
                    || gameModel.linkWithOneCorner(pic1, pic2, pos2)
                    || gameModel.linkWithTwoCorner(pic1, pic2, pos2, pos3)) {
                drawLine(pic1, pic2, pos2, pos3);

                success = true;
                gameModel.map[i/18][i%18] = tmp1;
                gameModel.map[j/18][j%18] = tmp2;
                gameModel.totalPic += 2;
            }

        }
    }
}

void BasicModeWindow::drawLine(QString pic1, QString pic2, QString pos2, QString pos3) {
    MapButton *p1 = ui->picWidget->findChild<MapButton*>(pic1);
    MapButton *p2 = ui->picWidget->findChild<MapButton*>(pic2);

    //Three cases: In the same line/ One corner/ Two corners respectively

    if (gameModel.flagA) {
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
    while(t.elapsed()<200) //Delay 0.2 seconds, can be chanegd
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

    //Shuffle the rawMap

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

            // Add surround images

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

            // If the image is deleted, then set the randimPicIndex to be 0. Means it is deleted.

            if (randomPicIndex == 0) {
                pic->setStyleSheet("background:transparent");

                // i means the number of rows and j is the number of columns.

                pic->setObjectName(QString::number(i * 18 + j));

                //Set the size of icons to fill the widge

                pic->setIconSize(QSize(40, 40));
                pic->setMinimumSize(40, 40);
                pic->setMaximumSize(40, 40);
                gameModel.map[i][j] = 0;
            } else {
                pic->setStyleSheet("background:white");
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



