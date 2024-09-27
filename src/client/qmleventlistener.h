#pragma once

#include "common/concurrentsegment.h"
#include "common/symmetricpulsegenerator.h"

#include <QtGui>
#include <QtQml>

class QmlEventListener : public QObject
{
    Q_OBJECT

    ///// MEMBERS /////
private:
    QPointF                     m_pos;

    SymmetricPulseGenerator*    m_generator;
    ConcurrentSegment           m_segment;

    ///// FUNCTIONS /////
public:
                                QmlEventListener(QObject* parent = nullptr);
    Q_INVOKABLE void            listenTo(QObject* object);
    QPointF                     positionSelectFilter(QEvent* event);
    bool                        eventFilter(QObject* object, QEvent* event) override;

    ///// SIGNALS / SLOTS /////
public slots:
    void                        writePos();
};
