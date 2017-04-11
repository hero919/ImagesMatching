#include "relaxedmodewindow.h"
#include "ui_relaxedmodewindow.h"
#include <QMessageBox>
#include <QTime>
#include "MainWindow/mainwindow.h"

RelaxedModeWindow::RelaxedModeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RelaxedModeWindow),
    sound(":/music/res/Pokemon02.wav")
{
    // Set up the initial window and the map, and start the music and timer

    ui->setupUi(this);
    setWindowTitle("IMages Matching Game");
    sound.play();
    gameModel.init();
    helpDialog = new HelpDialog(ui->picWidget);
    ui->progressBar->setValue(totleTime);
    grid = new QGridLayout(ui->picWidget);
    timer = new QTimer(this);
    painter = new QPainter(this);
    drawLineLayer = new DrawLineLayer(this);
    drawLineLayer->hide();
    drawLineLayer->setGeometry(QRect(0, 0, 720, 480));

    // Disable all of necessary buttons

    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);

    // Set Sinal and slots for buttons

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(startGame()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(pauseGame()));
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(findHint()));
    connect(ui->pushButton_4, SIGNAL(clicked(bool)), this, SLOT(resetMap()));
    connect(ui->pushButton_5, SIGNAL(clicked(bool)), this, SLOT(useTool()));
    connect(ui->pushButton_8, SIGNAL(clicked(bool)), this, SLOT(showHelp()));
    connect(ui->BackToMain, SIGNAL(clicked(bool)), this, SLOT(backToMainPage()));
}

RelaxedModeWindow::~RelaxedModeWindow()
{
    delete ui;
}

void RelaxedModeWindow::startGame() {

    // Initial a map

    initMap();
    totleTime = 100;

    // Initial the progress bar

    ui->progressBar->setValue(totleTime);

    // The timer starts and the duration is 1000ms

    timer->start(1000);

    // Clear the score

    credit = 0;
    ui->label_2->setText(QString::number(credit));
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setText("Restart");

    // Set the different use for start game button

    if (disconnect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(startGame())))
        connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(reStartGame()));
}

void RelaxedModeWindow::reStartGame() {

    // Clear the game board

    auto children = ui->picWidget->children();
    for (int i = 1; i < 217; i++) {
        if (children[i]->objectName() != "") {
            grid->removeWidget((QWidget*)children[i]);
            children[i]->deleteLater();
        }
    }

    // Start a new game

    startGame();
}

void RelaxedModeWindow::pauseGame() {

    // Resume the game if the timer is active

    if (timer->isActive()) {
        ui->pushButton_2->setText("Resume");
        timer->stop();
        ui->picWidget->setDisabled(true);
        ui->pushButton->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
    } else {
        ui->pushButton_2->setText("Pause");
        timer->start();
        ui->picWidget->setDisabled(false);
        ui->pushButton->setDisabled(false);
        ui->pushButton_3->setDisabled(false);
        ui->pushButton_4->setDisabled(false);
    }
}

void RelaxedModeWindow::timerUpDate() {

    // Update the time by the speed

    totleTime -= speed;

    // Update the progress bar

    ui->progressBar->setValue(totleTime);

    // Show the message if the time is up

    if (totleTime == 0) {
        QMessageBox *box = new QMessageBox(this);
        box->setInformativeText("Time Up！");
        box->show();
        this->backToMainPage();
    }
}

void RelaxedModeWindow::resetMap() {

    // Clear the existing map

    auto children = ui->picWidget->children();
    for (int i = 1; i < 217; i++) {
        if (children[i]->objectName() != "") {
            grid->removeWidget((QWidget*)children[i]);
            children[i]->deleteLater();
        }
    }

    // Reset the map

    reset(true);

    // Consume 50 scores

    decreaseCredit(50);
}

void RelaxedModeWindow::findHint() {
    QString pos2, pos3;
    QString pic1, pic2;
    int tmp1, tmp2;
    bool success = false;

    // Keep looking for find a pair of pictures that can be connected until successful

    for (int i = 0; i < 216 && !success; i++) {
        for (int j = 0; j < 216 && !success && j!=i; j++) {
            if (i % 18 == 0 || i % 18 == 17 || i<18 || i>=198 || j % 18 == 0 || j % 18 == 17 || j<18 || j>=198)
                continue;
            pic1 = QString::number(i);
            pic2 = QString::number(j);
            tmp1 = gameModel.map[i/18][i%18];
            tmp2 = gameModel.map[j/18][j%18];

            // If find those two pictures can be connected, draw a line

            if (gameModel.linkWithNoCorner(pic1, pic2)
                               || gameModel.linkWithOneCorner(pic1, pic2, pos2)
                               || gameModel.linkWithTwoCorner(pic1, pic2, pos2, pos3)) {//可消去
                drawLine(pic1, pic2, pos2, pos3);
                success = true;
                gameModel.map[i/18][i%18] = tmp1;
                gameModel.map[j/18][j%18] = tmp2;

                // Cover the reduced pictures that should be there

                gameModel.totalPic += 2;
            }
        }
    }

    decreaseCredit(20);
}


void RelaxedModeWindow::showHelp() {
    helpDialog->showHelpDialog();
}

void RelaxedModeWindow::backToMainPage(){
    sound.stop();
    timer->stop();

    // Create a new main page

    MainWindow *mw = new MainWindow(0, 1);
    mw->show();
    this->hide();
}

void RelaxedModeWindow::reset(bool flag) {

    // Clear the raw map and then create a new one

    if (flag) {
        gameModel.clearRawMap();
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 18; j++) {
                if (i == 0 || i == 11 || j == 0 || j == 17) {
                    continue;
                }
                gameModel.rawMap[i-1][j-1] = gameModel.map[i][j];
            }
        }
    }
    srand((int)time(0));
    int randx1, randx2, randy1, randy2;

    // Exchange the pictures in the map 300 times to shuffle the map

    for (int k = 0; k < 300; k++) {
        randx1 = random() % 10;
        randx2 = random() % 10;
        randy1 = random() % 16;
        randy2 = random() % 16;
        int tmp = gameModel.rawMap[randx1][randy1];
        gameModel.rawMap[randx1][randy1] = gameModel.rawMap[randx2][randy2];
        gameModel.rawMap[randx2][randy2] = tmp;
    }

    // Add surround images

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 18; j++) {
            if (i == 0 || i == 11 || j == 0 || j == 17) {
                MapButton *w = new MapButton();
                w->setStyleSheet("background: transparent");
                w->setObjectName(QString::number(i * 18 + j));
                w->setMinimumSize(40, 40);
                w->setMaximumSize(40, 40);
                w->setParent(ui->picWidget);
                grid->addWidget(w, i, j);
                continue;
            }

            MapButton *pic = new MapButton();
            int randomPicIndex = gameModel.rawMap[i-1][j-1];

            // If the image is deleted, then the randimPicIndex should be 0, and set the background as transparent.

            if (randomPicIndex == 0) {
                pic->setStyleSheet("background: transparent");
                pic->setObjectName(QString::number(i * 18 + j));
                pic->setIconSize(QSize(40, 40));
                pic->setMinimumSize(40, 40);
                pic->setMaximumSize(40, 40);
                gameModel.map[i][j] = 0;
            } else {

                // If the image exists, then set the background as white and add the picture.

                pic->setStyleSheet("background: white");
                pic->setIcon(QIcon(":/icon/res/" + QString::number(randomPicIndex) + ".png"));                
                pic->setObjectName(QString::number(i * 18 + j));
                pic->setIconSize(QSize(40, 40));
                pic->setMinimumSize(40, 40);
                pic->setMaximumSize(40, 40);
                pic->setCheckable(true);
                connect(pic, &MapButton::keyClicked, this, &RelaxedModeWindow::select);
                gameModel.map[i][j] = randomPicIndex;
            }
            grid->addWidget(pic, i, j);
        }
    }
}

void RelaxedModeWindow::initMap() {

    // Initial the map

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 16; j++) {
            gameModel.rawMap[i][j] = gameModel.totalPic++ % PIC_NUM + 1;
        }
    }

    // Shuffle the raw map

    reset(false);
}

void RelaxedModeWindow::select(const QString &msg) {
    QString pos2, pos3;
    MapButton *sb = ui->picWidget->findChild<MapButton*>(msg);
    if (sb != NULL) {

        // Click one picture multiple times.

        if (gameModel.selectedPic == sb->objectName()) {
            sb->setChecked(false);
            gameModel.selectedPic = "";

            // There is no picture selected

        } else if (gameModel.selectedPic == "") {
            gameModel.selectedPic = sb->objectName();
        } else if (isUsingTool || gameModel.linkWithNoCorner(gameModel.selectedPic, sb->objectName())
                   || gameModel.linkWithOneCorner(gameModel.selectedPic, sb->objectName(), pos2)
                   || gameModel.linkWithTwoCorner(gameModel.selectedPic, sb->objectName(), pos2, pos3)) { //可消去
            if (isUsingTool) {
               gameModel.useTool(gameModel.selectedPic, sb->objectName());
               ui->pushButton_5->setEnabled(true);
            }

            // Draw a line

            drawLine(gameModel.selectedPic, sb->objectName(), pos2, pos3);

            // Two pictures match up

            MapButton *p1 = ui->picWidget->findChild<MapButton*>(gameModel.selectedPic);
            MapButton *p2 = ui->picWidget->findChild<MapButton*>(sb->objectName());
            p1->setVisible(false);
            p1->setStyleSheet("background:transparent");
            p2->setVisible(false);
            p2->setStyleSheet("background:transparent");
            increaseCredit();

            // Determine if the player can use hint or reset

            if (creditIsEnoughForHint()) {
                ui->pushButton_3->setEnabled(true);
            }  
            if (creditIsEnoughForReset()) {
                ui->pushButton_4->setEnabled(true);
            }

            isUsingTool = false;
            gameModel.selectedPic = "";

            // Determine if the player wins

            if (gameModel.isWin()){
                QMessageBox *box = new QMessageBox(this);
                box->setInformativeText("Congratulations！You are the winner!");
                box->show();
                timer->stop();
                ui->pushButton_2->setEnabled(false);
                ui->pushButton_3->setEnabled(false);
                ui->pushButton_4->setEnabled(false);
            }

        } else {

            // If the two pictures cannot be connected, the first picture should pop up

            MapButton *p1 = ui->picWidget->findChild<MapButton*>(gameModel.selectedPic);
            p1->setChecked(false);
            gameModel.selectedPic = sb->objectName();

            // The original first picture is pressed

            sb->setChecked(true);
        }
    }
}

void RelaxedModeWindow::drawLine(QString pic1, QString pic2, QString pos2, QString pos3) {
    MapButton *p1 = ui->picWidget->findChild<MapButton*>(pic1);
    MapButton *p2 = ui->picWidget->findChild<MapButton*>(pic2);

    // No turning points

    if (gameModel.flagA) {
        drawLineLayer->setPos1(p1->pos());
        drawLineLayer->setPos2(p2->pos());
        gameModel.flagA = false;

        // Only one turning points

    } else if (gameModel.flagB) {
        drawLineLayer->setPos1(p1->pos());
        drawLineLayer->setPos2(ui->picWidget->findChild<MapButton*>(pos2)->pos());
        drawLineLayer->setPos3(p2->pos());
        gameModel.flagB = false;

        // More than one turning points

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

    // Delay 2 seconds

    while(t.elapsed()<200)
        QCoreApplication::processEvents();
    drawLineLayer->clear();
}

void RelaxedModeWindow::increaseCredit() {
    credit += 10;
    creditIncrement += 10;

    // Get a tool when the player successfully connects 20 pairs of pictures, and then show the message

    if (creditIncrement % 200 == 0 && gameModel.totalPic != 0) {
        QMessageBox *box = new QMessageBox(this);
        box->setInformativeText("Get a Tool！");
        box->setStyleSheet(QString::fromUtf8("color: red;"
                                             "background-color: white;"));
        box->setStandardButtons(0);
        box->show();
        QTime t;
        t.start();
        while(t.elapsed()<1000)
            QCoreApplication::processEvents();
        box->hide();
        toolsNum++;
        ui->label_5->setText(QString::number(toolsNum));
        ui->pushButton_5->setEnabled(true);
    }
    ui->label_2->setText(QString::number(credit));
}

void RelaxedModeWindow::decreaseCredit(int num) {

    // Decrease the credits and then check if the player can use hints or resets

    credit -= num;
    ui->label_2->setText(QString::number(credit));
    if (credit < 20) {
        ui->pushButton_3->setEnabled(false);
    }
    if (credit < 50) {
        ui->pushButton_4->setEnabled(false);
    }
}

bool RelaxedModeWindow::creditIsEnoughForHint() {
    return credit >= 20;
}

bool RelaxedModeWindow::creditIsEnoughForReset() {
   return credit >= 50;
}

void RelaxedModeWindow::useTool() {

    // Notify the player if he runs out of his tools

    if (toolsNum == 0) {
        QMessageBox *box = new QMessageBox(this);
        box->setInformativeText("Sorry, you have no tools！");
        box->setStyleSheet(QString::fromUtf8("color: red;"
                                             "background-color: white;"));
        box->setStandardButtons(0);
        box->show();
        QTime t;
        t.start();
        while(t.elapsed()<1000)
            QCoreApplication::processEvents();
        box->hide();
        ui->pushButton_5->setEnabled(false);

        // Otherwise reduce one number of tool

    } else {
        toolsNum--;
        ui->label_5->setText(QString::number(toolsNum));
        isUsingTool = true;
        ui->pushButton_5->setEnabled(false);
    }
}


