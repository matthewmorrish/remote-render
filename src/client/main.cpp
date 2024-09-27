#include "framestream.h"
#include "framestreamview.h"
#include "qmleventlistener.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

static QObject* qmlEventListenerInstance(QQmlEngine*, QJSEngine* engine)
{
    return new QmlEventListener(engine);
}

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

    // Make FrameStreamView && QmlEventFilter available to Qml
    qmlRegisterType<FrameStreamView>("Client.Frame", 1, 0, "FrameStreamView");
    qmlRegisterSingletonType<QmlEventListener>("EventListener", 1, 0, "QmlEventListener", qmlEventListenerInstance);

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
