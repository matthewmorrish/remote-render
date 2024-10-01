#include "qmleventlistener.h"
#include <QRandomGenerator>

QmlEventListener::QmlEventListener(QObject* parent)
    : QObject(parent),
    m_generator(&SymmetricPulseGenerator::getInstance())
{
    m_segment.setHandle("pos");
    connect(m_generator, &SymmetricPulseGenerator::requestWrite, this, &QmlEventListener::writePos);
}

Q_INVOKABLE void QmlEventListener::listenTo(QObject* object)
{
    if (object)
    {
        object->installEventFilter(this);
    }
}

QPointF QmlEventListener::positionSelectFilter(QEvent* event)
{
    QPointF pos;

    switch( event->type() )
    {
    case QEvent::TouchEnd:
    {
        QTouchEvent* te = static_cast<QTouchEvent*>(event);
        QList<QEventPoint> points = te->touchPoints();
        if (points.size() != 0)
        {
            pos = points.at(0).pos();
            break;
        }
    }

    case QEvent::MouseButtonRelease:
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            pos = QPointF(mouseEvent->pos());
            break;
        }
    }

    default:
        (void)0;
    }

    return pos;
}

bool QmlEventListener::eventFilter(QObject* object, QEvent* event)
{
    (void)object;

    QPointF pos = positionSelectFilter(event);
    if (!pos.isNull())
    {
        m_pos = pos;
        return true;
    }

    return false;
}

void QmlEventListener::writePos()
{
    if (!m_pos.isNull())
    {
        // Ensures each event is unique so the buffer is always read
        uint r  = QRandomGenerator::global()->generate();

        float x = m_pos.x();
        float y = m_pos.y();

        QByteArray rxy = QByteArray(reinterpret_cast<const char*>(&r), sizeof(r)).toHex()
                         + ';'
                         + QByteArray(reinterpret_cast<const char*>(&x), sizeof(x)).toHex()
                         + ';'
                         + QByteArray(reinterpret_cast<const char*>(&y), sizeof(y)).toHex();

        m_segment.writeByteArray(rxy);
        m_pos = {0, 0};
    }
}
