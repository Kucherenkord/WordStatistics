#ifndef CFILECONTROLLER_H
#define CFILECONTROLLER_H


// Qt includes
#include <QObject>
#include <QFutureWatcher>
#include <QFile>

// Project includes
#include "src/CHistogramModel.h"


class CFileController: public QObject
{
    Q_OBJECT
public:
    CFileController(CHistogramModel& model, QObject* object, QObject* parent = nullptr);
    ~CFileController();

    void fileProcessing();
    void fileOpening(const QString& fileName);

public slots:
    void open(QString fileName);
    void start();
    void pause();
    void resume();
    void cancel();
    void onFileProcessingfinished();
    void onFileOpeningfinished();

signals:
    void progressChanged(int progress);
    void fileOpened();

private:
    QObject* m_objectPtr;
    CHistogramModel& m_model;
    QByteArray m_fileData;
    QFile m_file;
    QFutureWatcher<void> m_fileProcessingFutureWather;
    QFutureWatcher<void> m_fileOpeningFutureWatcher;
    QList<CHistogramModel::Item> m_fileProcessingResult;
};

#endif // CFILECONTROLLER_H
