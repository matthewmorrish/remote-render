#pragma once

#include <QThread>

typedef int Hz;
typedef unsigned long long u_llong;

class SymmetricPulseGenerator : public QThread
{
    Q_OBJECT

    ///// MEMBERS /////
private:
    int                             m_pulseWidth;

    ///// FUNCTIONS /////
public:
    static SymmetricPulseGenerator& getInstance();
                                    SymmetricPulseGenerator(SymmetricPulseGenerator const&) = delete;
    void                            operator=(SymmetricPulseGenerator const&)               = delete;

    bool                            setFrequency(Hz frequency);

private:
                                    SymmetricPulseGenerator();

    inline u_llong                  usecSinceEpoch();
    inline u_llong                  synchronizedTimeReference();

    void                            run() override;

    ///// SIGNALS / SLOTS /////
signals:
    void                            requestRead();
    void                            requestWrite();
};
