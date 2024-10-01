#include "qmleventrelay.h"

#include <QMouseEvent>
#include <QGuiApplication>
#include <QDebug>

QmlEventRelay::QmlEventRelay(QObject* parent)
    : QObject(parent),
    m_observer(nullptr),
    m_generator(&SymmetricPulseGenerator::getInstance())
{
    m_segment.setHandle("pos");
    connect(m_generator, &SymmetricPulseGenerator::requestRead, this, &QmlEventRelay::readPos);
}

void QmlEventRelay::setObserver(QObject* object)
{
    if (object != nullptr)
    {
        m_observer = object;
    }
}

void QmlEventRelay::sendMouseEvent(QPointF pos)
{
    /* As per https://doc.qt.io/qt-6/qcoreapplication.html#postEvent,
     *
     *   "The event must be allocated on the heap since the post event
     *   queue will take ownership of the event and delete it once it
     *   has been posted."
     *
     * making cleanup unwarranted.
     */

    QMouseEvent* press   = new QMouseEvent(QMouseEvent::Type::MouseButtonPress,
                                           pos,
                                           Qt::LeftButton,
                                           Qt::LeftButton,
                                           Qt::KeyboardModifier::NoModifier);

    QMouseEvent* release = new QMouseEvent(QMouseEvent::Type::MouseButtonRelease,
                                           pos,
                                           Qt::LeftButton,
                                           Qt::LeftButton,
                                           Qt::KeyboardModifier::NoModifier);

    QCoreApplication::postEvent(m_observer, press);
    QCoreApplication::postEvent(m_observer, release);
}

QPointF QmlEventRelay::parseEncodedPosition(QByteArray pos)
{
    float x = 0.00f;
    float y = 0.00f;

    QList<QByteArray> xy = pos.split(';');
    if (xy.size() == 3)
    {
        // Throw away r ( = xy.at(0) )
        QByteArray x_decoded = QByteArray::fromHex(xy.at(1));
        QByteArray y_decoded = QByteArray::fromHex(xy.at(2));

        if (x_decoded.size() >= sizeof(x))
        {
            x = *reinterpret_cast<const float*>(x_decoded.data());
        }

        if (y_decoded.size() >= sizeof(y))
        {
            y = *reinterpret_cast<const float*>(y_decoded.data());
        }
    }

    return {x, y};
}

void QmlEventRelay::readPos()
{
    QByteArray current = m_segment.readByteArray();

    if (current != m_previous)
    {
        QPointF point = parseEncodedPosition(current);
        if (!point.isNull())
        {
            sendMouseEvent(point);
        }

        m_previous = current;
    }
}
