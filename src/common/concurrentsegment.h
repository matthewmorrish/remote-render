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

    QByteArray      read();
    void            write(QByteArray data);

    QImage          readImage();
    void            writeImage(QImage img);
};
