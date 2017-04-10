#ifndef MAPBUTTON_H
#define MAPBUTTON_H

#include <QPushButton>
#include <QTimer>

/**
 * construct MapButton to track clicked images
 */
class MapButton : public QPushButton
{
    Q_OBJECT
public:
    /**
     * MapButton track down clicked image objectName with click signals
     * @param parent
     */
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
    /**
     * keyClicked signal
     * @param msg
     */
    void keyClicked(const QString &msg);

public slots:
    /**
     * having objectName of clicked image
     */
    void on_clicked() { emit this->keyClicked(this->objectName()); }
};
#endif // MAPBUTTON_H
