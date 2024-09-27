#pragma once

#include <QSharedMemory>
#include <QByteArray>
#include <QImage>
#include <QBuffer>

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

    QByteArray      readByteArray();
    void            writeByteArray(const QByteArray& data);

    QImage          readImage();
    void            writeImage(const QImage& img);

private:
    void            write(const QBuffer& buffer);
};
