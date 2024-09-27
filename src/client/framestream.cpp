#include "framestream.h"

#include <QTime>

FrameStream::FrameStream(QObject *parent)
    : QObject(parent)
{
    m_segment.setHandle("img");
    m_pulseGenerator.setFrequency(60);

    connect(&m_pulseGenerator, &SymmetricPulseGenerator::requestRead, this, &FrameStream::onRequestRead);
    m_pulseGenerator.start();
}

void FrameStream::setFrame(const QImage& frame)
{
    m_frame = frame;
    emit frameChanged();
}

QImage FrameStream::frame()
{
    return m_segment.read();
}

void FrameStream::onRequestRead()
{
    this->setFrame(this->frame());
}
