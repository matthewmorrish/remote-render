#pragma once

#include <QImage>
#include <QQuickPaintedItem>
#include <QPainter>

class FrameStreamView : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage frame MEMBER m_frame WRITE setFrame)

    ///// MEMBERS /////
private:
    QImage  m_frame;

    ///// FUNCTIONS /////
public:
            FrameStreamView(QQuickItem* parent = nullptr);
    void    setFrame(const QImage& frame);
    void    paint(QPainter* painter) override;
};
