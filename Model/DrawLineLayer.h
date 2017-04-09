#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QPainter>

/**
 * construct a line between images that can be eliminated
 */
class DrawLineLayer : public QWidget
{
    Q_OBJECT

public:
    //constructor
    DrawLineLayer(QWidget *parent = 0);

    //set posn
    void setPos1(QPoint pos);
    void setPos2(QPoint pos);
    void setPos3(QPoint pos);
    void setPos4(QPoint pos);

    //hide images
    void clear();

    //destructor
    ~DrawLineLayer();

protected:
    //paint the line
    void paintEvent(QPaintEvent *);

private:
    //posns of two images
    QPoint pos1, pos2, pos3, pos4;

};

#endif // WIDGET_H
