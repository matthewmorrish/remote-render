#include "concurrentsegment.h"

#include <QBuffer>
#include <QDebug>

ConcurrentSegment::ConcurrentSegment()
{
    // ...
}

ConcurrentSegment::~ConcurrentSegment()
{
    if (!m_sharedMemory.detach())
    {
        qWarning() << "The inferior"
                   << this
                   << "was unable to detach from segment"
                   << (void*)m_sharedMemory.constData();
    }
}

void ConcurrentSegment::setHandle(const QString& handle)
{
    m_sharedMemory.setKey(handle);
}

QImage ConcurrentSegment::read()
{
    QImage image;
    if (m_sharedMemory.attach())
    {
        m_sharedMemory.lock();

        QBuffer buffer;
        QDataStream in(&buffer);

        buffer.setData((char*)m_sharedMemory.constData(), m_sharedMemory.size());
        buffer.open(QBuffer::ReadOnly);
        in >> image;

        m_sharedMemory.unlock();
        m_sharedMemory.detach();
    }

    return image;
}


/* Try to create a segment. Since we are repeatedly reusing a
 * shared segment for our image, it must be created with a size
 * equal to the maximum possible size for an image of dimension
 * equal to our original images dimensions.
 *
 * It's hard to calculate what this will be in an optimal way, but
 * can be guaranteed not to exceed 2x the original images size.
 */

void ConcurrentSegment::write(QImage img)
{
    // Load into shared memory
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);

    QDataStream out(&buffer);
    out << img;
    int size = buffer.size();

    // Todo - fix *2
    if (!m_sharedMemory.create(size*2))
    {
        if (m_sharedMemory.error() == QSharedMemory::AlreadyExists)
        {
            m_sharedMemory.attach();
        }

        else
        {
            qDebug() << QString("Unable to create or attach to shared memory segment: %1").arg(m_sharedMemory.errorString());
            return;
        }
    }

    m_sharedMemory.lock();
    char* to = (char*)m_sharedMemory.data();
    const char* from = buffer.data().data();
    memcpy(to, from, qMin(m_sharedMemory.size(), size));
    m_sharedMemory.unlock();
}
