#pragma once

#include "common/concurrentsegment.h"

#include <QObject>
#include <QImage>

/**
 * @brief The QmlConcurrentSegment class is a QML wrapping
 * compositor for the ConcurrentSegment type.
 */
class QmlConcurrentSegment : public QObject
{
    Q_OBJECT

    ///// MEMBERS /////
private:
    ConcurrentSegment   m_segment;

    ///// FUNCTIONS /////
public:
                        QmlConcurrentSegment(QObject* parent = nullptr);

    void                setHandle(const QString& handle);

    QImage              readImage();
    Q_INVOKABLE void    writeImage(const QImage& img);
};
