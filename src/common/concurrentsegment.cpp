#include "concurrentsegment.h"

#include <QBuffer>
#include <QDebug>

ConcurrentSegment::ConcurrentSegment()
    : m_initialized(false)
{
    // ...
}

ConcurrentSegment::~ConcurrentSegment()
{
    m_sharedMemory.detach();
}

void ConcurrentSegment::initialize(int size)
{
    if (size != 0)
    {
        /* Try to create a segment. Since we are repeatedly reusing a
         * shared segment for our image, it must be created with a size
         * equal to the maximum possible size for an image with dimensions
         * equal to our original images dimensions.
         *
         * It's hard to calculate what this will be in an optimal way, but
         * can be guaranteed not to exceed 2x the original images size.
         */
        m_sharedMemory.create(size * 2);
    }

    if (m_sharedMemory.attach())
    {
        m_initialized = true;
    }
}

void ConcurrentSegment::setHandle(const QString& handle)
{
    m_sharedMemory.setKey(handle);
}

QImage ConcurrentSegment::read()
{
    QImage image;

    if (!m_initialized)
    {
        this->initialize(0);
    }

    else
    {
        QBuffer buffer;
        QDataStream in(&buffer);

        m_sharedMemory.lock();

        buffer.setData((char*)m_sharedMemory.constData(), m_sharedMemory.size());
        buffer.open(QBuffer::ReadOnly);
        in >> image;

        m_sharedMemory.unlock();
    }

    return image;
}

void ConcurrentSegment::write(const QImage& img)
{
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << img;

    if (!m_initialized)
    {
        this->initialize(buffer.size());
    }

    else
    {
        char* to            = (char*)m_sharedMemory.data();
        const char* from    = buffer.data().data();
        int size            = qMin(m_sharedMemory.size(), buffer.size());

        m_sharedMemory.lock();
        memcpy(to, from, size);
        m_sharedMemory.unlock();
    }
}
