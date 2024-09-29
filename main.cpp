// Qt includes
#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>

// Project includes
#include "src/CFileController.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/WordStatistics/qml/Main.qml"_qs);

    QObject* objectPtr = nullptr;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, [&objectPtr](QObject* obj, QUrl Url){
        if (obj != nullptr){
            objectPtr = obj;
        }
        else
            qCritical() << "Failed to create qml root object!";
    });
    engine.load(url);

    CHistogramModel model;
    CFileController controller(model, objectPtr);

    if (objectPtr != nullptr){
        objectPtr->setProperty("model", QVariant::fromValue(&model));
    }

    return app.exec();
}
