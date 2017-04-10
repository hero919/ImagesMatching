#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QPainter>

/**
 *constructor a draw line layer
 */
class DrawLineLayer : public QWidget
{
    Q_OBJECT

public:
    /**
     * constructor
     * @param parent
     */
    DrawLineLayer(QWidget *parent = 0);

    /**
     * set posn
     * @param pos
     */
    void setPos1(QPoint pos);
    void setPos2(QPoint pos);
    void setPos3(QPoint pos);
    void setPos4(QPoint pos);

    /**
     * set posn values to 0
     */
    void clear();

    /**
     destructor
     */
    ~DrawLineLayer();

protected:

    /**
     * draw a line between two selected images
     */
    void paintEvent(QPaintEvent *);

private:
    /**
     * posns
     */
    QPoint pos1, pos2, pos3, pos4;

};

#endif // WIDGET_H
