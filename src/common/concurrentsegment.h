#pragma once

#include <QSharedMemory>
#include <QByteArray>

#include <QImage>

class ConcurrentSegment
{

    ///// MEMBERS /////
private:
    QSharedMemory   m_sharedMemory;

    ///// FUNCTIONS /////
public:
                    ConcurrentSegment();
                   ~ConcurrentSegment();

    void            setHandle(const QString &handle);

    QImage          read();
    void            write(QImage img);
};
