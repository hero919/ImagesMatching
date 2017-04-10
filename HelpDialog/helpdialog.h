#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QDesktopWidget>

class HelpDialog: QDialog
{
    Q_OBJECT
public:
    HelpDialog(QWidget *parent = 0) : QDialog(parent) {
        hd = new QDialog() ;
        //Set the size of the images
        hd->setMinimumSize(500, 500);
        hd->setMaximumSize(500, 500);
        QVBoxLayout *layout = new QVBoxLayout();
        QTextBrowser *tb = new QTextBrowser();
        layout->addWidget(tb);
        QString text = "";
        text = text + R"(<strong><span style="font-size:16px;">Instruction</span></strong><br />
                The basic functions of this images machting game include game start, game pause, hint, game resetting and timing. <br />
                <strong>1. Start</strong><br />
                When you start a new game, the map will be generated after clicking the "start" button. Please select two images by clicking the left button of your mouse. The two selected images will be sucessfully matched if those two images seem pressed after the selection.<br />
                <strong>2. Pause</strong><br />
                After the game starts, you can click on "Pause" button to suspend the game. Please click on the "Continue" button if you want to resume the game. <br />
                <strong>3. Hint</strong><br />
                After the game starts, you can click on "Hint" button if you get stuck. The map will show two images to be matched with a red line. <br />
                <strong>4. Reset</strong><br />
                After the game starts, you can click on "Hint" Reset if you want to shuffle the remaining images in the map. <br />
                <br />)";

        tb->setHtml(text);
        hd->setLayout(layout);
    }

    void showHelpDialog();

private:
    QDialog *hd;

};

#endif // HELPDIALOG_H
