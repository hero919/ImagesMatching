#ifndef TOOLNOTIFICATION_H
#define TOOLNOTIFICATION_H

#include <QDialog>
#include <QGridLayout>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QDesktopWidget>

class ToolNotification: QDialog
{
    Q_OBJECT
public:
    ToolNotification(QWidget *parent = 0) : QDialog(parent) {
        hd = new QDialog() ;
        hd->setMinimumSize(100, 100);
        hd->setMaximumSize(100, 100);
        QVBoxLayout *layout = new QVBoxLayout();
        QTextBrowser *tb = new QTextBrowser();
        layout->addWidget(tb);
        QString text = "";
        text = text + R"(<strong><span style="font-size:16px;">Congrats!</span></strong><br />
                <strong>You GOT a Tool!</strong><br />)";

        tb->setHtml(text);
        hd->setLayout(layout);
    }

    void showToolNotification();

private:
    QDialog *hd;

};

#endif // TOOLNOTIFICATION_H
