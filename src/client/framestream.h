#pragma once

#include "common/concurrentsegment.h"
#include "common/symmetricpulsegenerator.h"

#include <QObject>
#include <QImage>

class FrameStream : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage frame MEMBER m_frame READ frame WRITE setFrame NOTIFY frameChanged)

    ///// MEMBERS /////
private:
    ConcurrentSegment           m_segment;
    SymmetricPulseGenerator*    m_pulseGenerator;
    QImage                      m_frame;

    ///// FUNCTIONS /////
public:
                                FrameStream(QObject *parent = nullptr);
    void                        setFrame(QImage const &image);
    QImage                      frame();

    ///// SIGNALS / SLOTS /////
signals:
    void                        frameChanged();

public slots:
    void                        onRequestRead();
};
