#pragma once

#include <QThread>

typedef unsigned long long u_llong;
typedef int Hz;

class SymmetricPulseGenerator : public QThread
{
    Q_OBJECT

    ///// MEMBERS /////
private:
    int             m_pulseWidth;

    ///// FUNCTIONS /////
public:
                    SymmetricPulseGenerator();
    bool            setFrequency(Hz frequency);

private:
    inline u_llong  usecSinceEpoch();
    inline u_llong  synchronizedTimeReference();

    void            run() override;

    ///// SIGNALS / SLOTS /////
signals:
    void            requestRead();
    void            requestWrite();
};
