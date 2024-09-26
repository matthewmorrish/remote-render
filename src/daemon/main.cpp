#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "common/concurrentsegment.h"
#include "common/symmetricpulsegenerator.h"

void test()
{
    return (void)0;
}

int main(int argc, char *argv[])
{
    test();

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
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
