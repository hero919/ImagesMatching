#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDialog>
#include <QTableView>
#include <QSound>

namespace Ui {
class MainWindow;
}

/**
 * construct a main window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow
     * @param parent
     * @param isBack
     */
    explicit MainWindow(QWidget *parent = 0, bool isBack = 0);
    /**
     * set up welcome page
     */
    void setUpWelcomeWindow();
    /**
     * display welcome window
     */
    void showMainWindow();
    /**
     * destructor
     */
    ~MainWindow();
    /**
     * music player
     */
    QSound sound;

public slots:
    /**
     * Display a basic mode game
     */
    void showBasicModeWindow();

    /**
     * Display a relax mode game
     */
    void showRelaxedModeWindow();

    /**
     * Display a level mode game
     */
    void showLevelModeWindow();

    /**
     * Display Help Dialog
     */
    void showHelp();

    /**
     * Play backgroud music
     */
    void playMusic();

    /**
     * Pause background music
     */
    void stopMusic();

private:
    /**
     * UI
     */
    Ui::MainWindow *ui;

    /**
     * Deciding if player gets navigated to main menu
     */
    bool isBack_;
};

#endif // MAINWINDOW_H
