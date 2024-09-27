#include "qmlconcurrentsegment.h"

#include <QBuffer>
#include <QDebug>

QmlConcurrentSegment::QmlConcurrentSegment(QObject* parent)
    : QObject(parent)
{
    // ...
}

void QmlConcurrentSegment::setHandle(const QString& handle)
{
    m_segment.setHandle(handle);
}

QImage QmlConcurrentSegment::readImage()
{
    return m_segment.readImage();
}

void QmlConcurrentSegment::writeImage(const QImage& img)
{
    m_segment.writeImage(img);
}
