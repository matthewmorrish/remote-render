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

QByteArray QmlConcurrentSegment::read()
{
    return m_segment.read();
}

void QmlConcurrentSegment::write(const QImage& img)
{
    m_segment.writeImage(img);
}
