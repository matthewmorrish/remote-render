#pragma once

#include "common/concurrentsegment.h"
#include "common/symmetricpulsegenerator.h"

#include <QObject>

class QmlEventRelay : public QObject
{
    Q_OBJECT

    ///// MEMBERS /////
private:
    QObject*                    m_observer;
    SymmetricPulseGenerator*    m_generator;
    ConcurrentSegment           m_segment;
    QByteArray                  m_previous;

    ///// FUNCTIONS /////
public:
                                QmlEventRelay(QObject* parent = nullptr);
    void                        sendMouseEvent(QPointF pos);

private:
    QPointF                     parseEncodedPosition(QByteArray pos);

    ///// SIGNALS / SLOTS /////
public slots:
    void                        readPos();
    void                        setObserver(QObject* object);
};
