#ifndef MAPBUTTON_H
#define MAPBUTTON_H

#include <QPushButton>
#include <QTimer>


/**
 * construct mapbutton to track selected images
 */
class MapButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MapButton(QWidget *parent = 0) : QPushButton(parent),
        pauseMsecs(400), intervalMsecs(30)
    {
        tm = new QTimer(this);
        connect(this, SIGNAL(clicked()), this, SLOT(on_clicked()));
    }

private:
    QTimer *tm;
    long pauseMsecs;
    long intervalMsecs;

signals:
    void keyClicked(const QString &msg);

public slots:
    void on_clicked() { emit this->keyClicked(this->objectName()); }
};
#endif // MAPBUTTON_H
