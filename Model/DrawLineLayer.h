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
    /**
     * constructor
     * @param parent
     */
    DrawLineLayer(QWidget *parent = 0);
    /**
     * set posns of two images
     * @param pos
     */
    void setPos1(QPoint pos);
    void setPos2(QPoint pos);
    void setPos3(QPoint pos);
    void setPos4(QPoint pos);
    /**
     * set posns to 0
     */
    void clear();
    /**
     * destructor
     */
    ~DrawLineLayer();

protected:
    /**
     * rewrite paintEvent
     */
    void paintEvent(QPaintEvent *);

private:
    /**
     * Posns to be set for images
     */
    QPoint pos1, pos2, pos3, pos4;

};

#endif // WIDGET_H
