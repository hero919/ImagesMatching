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
        hd->setMinimumSize(500, 500);
        hd->setMaximumSize(500, 500);
        QVBoxLayout *layout = new QVBoxLayout();
        QTextBrowser *tb = new QTextBrowser();
        layout->addWidget(tb);
        QString text = "";
        text = text + R"(<strong><span style="font-size:16px;"></span></strong>
                <strong>1. Play </strong><br />
                Click on "Start" button to load game map. Find the two identical objects as qucikly as possible.<br />
                <strong>2. Pause </strong><br />
                Click on "Pause" button to pause the game. Click on "Continue" to resume the game. <br />
                <strong>3. Hint</strong><br />
                Click on "Hint" button. A pair of objects that can be matched will be connected with a dotted red line.<br />
                <strong>4. Reset</strong><br />
                Click on "Reset" button to shuffle the rest of objects.<br />
                <strong>5. Back</strong><br />
                Click on "Back" button to be navigated to the main menu.<br />
                )";

        tb->setHtml(text);
        hd->setLayout(layout);
    }

    void showHelpDialog();

private:
    QDialog *hd;

};

#endif // HELPDIALOG_H
