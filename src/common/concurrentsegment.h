#pragma once

#include <QSharedMemory>
#include <QByteArray>

#include <QImage>

class ConcurrentSegment
{

    ///// MEMBERS /////
private:
    QSharedMemory   m_sharedMemory;
    bool            m_initialized;

    ///// FUNCTIONS /////
public:
                    ConcurrentSegment();
                   ~ConcurrentSegment();

    void            initialize(int size);
    void            setHandle(const QString &handle);

    QImage          read();
    void            write(const QImage& img);
};
