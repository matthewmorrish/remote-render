#include "symmetricpulsegenerator.h"

#include <QEventLoop>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

SymmetricPulseGenerator::SymmetricPulseGenerator()
    : m_pulseWidth(0)
{
    // ...
}

bool SymmetricPulseGenerator::setPulseWidth(int pulseWidth)
{
    /* TODO
     * Setting a calculated pulse-width seems silly, this should
     * be set in Hz then calculated on start.
     */

    if (!m_pulseWidth)
    {
        m_pulseWidth = pulseWidth;
        return true;
    }

    return false;
}

inline u_llong SymmetricPulseGenerator::usecSinceEpoch()
{
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    return ( (u_llong)tp.tv_sec * 1000000 ) + ( tp.tv_nsec / 1000 );
}

u_llong SymmetricPulseGenerator::synchronizedTimeReference()
{
    u_llong estimate = usecSinceEpoch();
    while ( (estimate % m_pulseWidth) )
    {
        ++estimate;
    }

    return estimate;
}

void SymmetricPulseGenerator::run() [[noreturn]]
{
    /* TODO
     * This loop runs so fast that excessive compute resources
     * are unnecessarily consumed on start.  A calculated delay
     * should be added to drop resource consumption.
     */

    u_llong readEmitTime    = synchronizedTimeReference()   + m_pulseWidth;
    u_llong writeEmitTime   = readEmitTime                  + (m_pulseWidth * 0.5);

    while (1)
    {
        if (usecSinceEpoch() >= readEmitTime)
        {
            emit requestRead();
            readEmitTime += m_pulseWidth;
        }

        if (usecSinceEpoch() >= writeEmitTime)
        {
            emit requestWrite();
            writeEmitTime += m_pulseWidth;
        }
    }
}
