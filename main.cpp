#include <MainWindow/mainwindow.h>
#include <QSpinBox>
#include <QSlider>
#include <QHBoxLayout>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    w.setWindowTitle("Images Matching Game");

    w.show();

    return a.exec();
}
