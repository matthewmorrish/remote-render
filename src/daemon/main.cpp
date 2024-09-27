#include "qmlconcurrentsegment.h"
#include "common/symmetricpulsegenerator.h"
#include "qmleventrelay.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QProcessEnvironment>

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));

    QGuiApplication app(argc, argv);

    // Create a concurrent segment
    QmlConcurrentSegment qmlConcurrentSegment;
    qmlConcurrentSegment.setHandle("img");

    // Create a pulse generator
    SymmetricPulseGenerator* symmetricPulseGenerator = &SymmetricPulseGenerator::getInstance();
    symmetricPulseGenerator->setFrequency(60);
    symmetricPulseGenerator->start();

    // Create a QmlEventRelay
    QmlEventRelay eventRelay;

    // Create an engine & expose the concurrent segment / pulse generator to it
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("qmlConcurrentSegment", &qmlConcurrentSegment);
    engine.rootContext()->setContextProperty("symmetricPulseGenerator", symmetricPulseGenerator);
    engine.rootContext()->setContextProperty("eventRelay", &eventRelay);

    // Start up the app
    const QUrl url(u"qrc:/daemon/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
