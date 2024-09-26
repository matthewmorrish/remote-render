#include "framestreamview.h"

FrameStreamView::FrameStreamView(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    // ...
}

void FrameStreamView::paint(QPainter* painter)
{
    painter->drawImage(0, 0, m_frame);
}

void FrameStreamView::setFrame(const QImage& frame)
{
    m_frame = frame;
    this->update();
}
