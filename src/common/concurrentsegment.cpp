#include "concurrentsegment.h"

ConcurrentSegment::ConcurrentSegment()
    : m_initialized(false)
{
    // ...
}

ConcurrentSegment::~ConcurrentSegment()
{
    m_sharedMemory.detach();
}

void ConcurrentSegment::setHandle(const QString& handle)
{
    m_sharedMemory.setKey(handle);
}

QByteArray ConcurrentSegment::readByteArray()
{
    QByteArray data;

    if (m_sharedMemory.attach())
    {
        QBuffer buffer;
        QDataStream in(&buffer);

        m_sharedMemory.lock();

        buffer.setData((char*)m_sharedMemory.constData(), m_sharedMemory.size());
        buffer.open(QBuffer::ReadOnly);
        in >> data;

        m_sharedMemory.unlock();
        m_sharedMemory.detach();
    }

    return data;
}

void ConcurrentSegment::writeByteArray(const QByteArray& data)
{
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << data;

    this->write(buffer);
}

QImage ConcurrentSegment::readImage()
{
    QImage image;

    if (m_sharedMemory.attach())
    {
        QBuffer buffer;
        QDataStream in(&buffer);

        m_sharedMemory.lock();

        buffer.setData((char*)m_sharedMemory.constData(), m_sharedMemory.size());
        buffer.open(QBuffer::ReadOnly);
        in >> image;

        m_sharedMemory.unlock();
        m_sharedMemory.detach();
    }

    return image;
}

void ConcurrentSegment::writeImage(const QImage& img)
{
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << img;

    this->write(buffer);
}

void ConcurrentSegment::write(const QBuffer& buffer)
{
    /* Try to create a segment. Since we are repeatedly reusing a
     * shared segment for our image, it must be created with a size
     * equal to the maximum possible size for an image with dimensions
     * equal to our original images dimensions.
     *
     * It's hard to calculate what this will be in an optimal way, but
     * can be guaranteed not to exceed 2x the original images size. This
     * is also a pretty small price to pay.
     */

    if (!m_sharedMemory.create(buffer.size() * 2))
    {
        if (m_sharedMemory.error() == QSharedMemory::AlreadyExists)
        {
            m_sharedMemory.attach();

            char* to            = (char*)m_sharedMemory.data();
            const char* from    = buffer.data().data();
            int size            = qMin(m_sharedMemory.size(), buffer.size());

            m_sharedMemory.lock();
            memcpy(to, from, size);
            m_sharedMemory.unlock();
        }
    }
}
