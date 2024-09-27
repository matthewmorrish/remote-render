#include "qmlconcurrentsegment.h"
#include "common/symmetricpulsegenerator.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Create a concurrent segment
    QmlConcurrentSegment qmlConcurrentSegment;
    qmlConcurrentSegment.setHandle("img");

    // Create a pulse generator
    SymmetricPulseGenerator symmetricPulseGenerator;
    symmetricPulseGenerator.setFrequency(60);
    symmetricPulseGenerator.start();

    // Create an engine & expose the concurrent segment / pulse generator to it
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("qmlConcurrentSegment", &qmlConcurrentSegment);
    engine.rootContext()->setContextProperty("symmetricPulseGenerator", &symmetricPulseGenerator);

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
