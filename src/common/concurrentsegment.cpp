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

void ConcurrentSegment::setHandle(const QString &handle)
{
    m_sharedMemory.setKey(handle);
}

QByteArray ConcurrentSegment::read()
{
    if (!m_sharedMemory.attach())
    {
        // qWarning() << "Unable to attach to shared memory segment, please render the remote buffer first";
        return QByteArray();
    }

    QBuffer buffer;
    QDataStream in(&buffer);
    QByteArray data;

    m_sharedMemory.lock();
    buffer.setData((char*)m_sharedMemory.constData(), m_sharedMemory.size());
    buffer.open(QBuffer::ReadOnly);
    in >> data;

    m_sharedMemory.unlock();
    m_sharedMemory.detach();

    return data;
}

QImage ConcurrentSegment::readImage()
{
    if (!m_sharedMemory.attach())
    {
        // qWarning() << "Unable to attach to shared memory segment, please render the remote buffer first";
        return QImage();
    }

    QBuffer buffer;
    QDataStream in(&buffer);
    QImage image;

    m_sharedMemory.lock();
    buffer.setData((char*)m_sharedMemory.constData(), m_sharedMemory.size());
    buffer.open(QBuffer::ReadOnly);
    in >> image;

    m_sharedMemory.unlock();
    m_sharedMemory.detach();

    return image;
}

void ConcurrentSegment::write(QByteArray data)
{
    // Load into shared memory
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);

    QDataStream out(&buffer);
    out << data;
    int size = buffer.size();

    if (!m_sharedMemory.create(size*2)) // Todo - fix me
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

void ConcurrentSegment::writeImage(QImage img)
{
    // Load into shared memory
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);

    QDataStream out(&buffer);
    out << img;
    int size = buffer.size();

    if (!m_sharedMemory.create(size*2)) // Todo - fix me
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
