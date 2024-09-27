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
    m_observer = object;
}

void QmlEventRelay::sendMouseEvent(QPointF pos, int button)
{
    Qt::MouseButton type = Qt::MouseButton(button);

    auto down = new QMouseEvent(QMouseEvent::Type::MouseButtonPress, pos, type, type, Qt::KeyboardModifier::NoModifier);
    QCoreApplication::postEvent(m_observer, down);

    auto up = new QMouseEvent(QMouseEvent::Type::MouseButtonRelease, pos, type, type, Qt::KeyboardModifier::NoModifier);
    QCoreApplication::postEvent(m_observer, up);
}

void QmlEventRelay::readPos()
{
    static QByteArray previous;
    QByteArray current = m_segment.readByteArray();

    if (current != previous)
    {
        previous = current;

        QList<QByteArray> xy = current.split(';');
        if (xy.size() == 2)
        {
            float x = 0.00f;
            float y = 0.00f;

            if (xy.at(0).size() >= sizeof(x))
            {
                x = *reinterpret_cast<const float*>(xy.at(0).data());
            }

            if (xy.at(1).size() >= sizeof(y))
            {
                y = *reinterpret_cast<const float*>(xy.at(1).data());
            }

            QPointF point = {x, y};

            sendMouseEvent(point, 1);
            sendMouseEvent(point, 2);
        }
    }
}
