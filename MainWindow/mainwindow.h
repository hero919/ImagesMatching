#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDialog>
#include <QTableView>
#include <QSound>
#include "Dao/scoredao.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, bool isBack = 0);
    void setUpWelcomeWindow();
    void showMainWindow();
 //   void backToMainWindow();
    ~MainWindow();
    QSound sound;

public slots:
    //Different Modes
    void showBasicModeWindow();
    void showRelaxedModeWindow();
    void showLevelModeWindow();
    void showHelp();
    void showRankingList();
    void playMusic();
    void stopMusic();

private:
    Ui::MainWindow *ui;
    QTableView *rankTableView;
<<<<<<< HEAD
    ScoreDao *scoreDao;
    bool isBack_;
=======
    //ScoreDao *scoreDao;
    QSound sound;


>>>>>>> master
};

#endif // MAINWINDOW_H
