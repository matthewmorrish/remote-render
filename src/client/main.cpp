#include "framestream.h"
#include "framestreamview.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Create frame stream
    FrameStream stream;

    // Create an engine & expose the framestream
    QQmlApplicationEngine engine;
    {
        engine.rootContext()->setContextProperty("FrameStream", &stream);
    }

    // Make FrameStreamView available to Qml
    qmlRegisterType<FrameStreamView>("Client.Frame", 1, 0, "FrameStreamView");

    // Start up the app
    const QUrl url(u"qrc:/client/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
