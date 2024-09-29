#include "CFileController.h"


// Qt includes
#include <QtCore/QFile>
#include <QtConcurrent/QtConcurrent>


CFileController::CFileController(CHistogramModel& model, QObject* object, QObject* parent)
    :QObject{parent},
    m_objectPtr(object),
    m_model(model)

 {
    if (object != nullptr){
        connect(object, SIGNAL(open(QString)), this, SLOT(open(QString)));
        connect(object, SIGNAL(start()), this, SLOT(start()));
        connect(object, SIGNAL(pause()), this, SLOT(pause()));
        connect(object, SIGNAL(resume()), this, SLOT(resume()));
        connect(object, SIGNAL(cancel()), this, SLOT(cancel()));
    }

    connect(&m_fileProcessingFutureWather, &QFutureWatcher<void>::finished, this, &CFileController::onFileProcessingfinished);
    connect(&m_fileOpeningFutureWatcher, &QFutureWatcher<void>::finished, this, &CFileController::onFileOpeningfinished);
}


CFileController::~CFileController()
{
    m_fileProcessingFutureWather.cancel();
    m_fileProcessingFutureWather.waitForFinished();

    m_fileOpeningFutureWatcher.cancel();
    m_fileOpeningFutureWatcher.waitForFinished();
}


void CFileController::fileProcessing()
{
    QDateTime start = QDateTime::currentDateTime();

    if (m_objectPtr != nullptr){
        m_objectPtr->setProperty("progressValue", 0);
        m_objectPtr->setProperty("progressMax", m_fileData.size());
    }

    QTextStream stream(&m_fileData);
    stream.setEncoding(QStringConverter::System);

    CHistogramModel tempModel;

    QRegularExpression regExp("[$&+,:;=?@#|'<>.^*()%!-]");

    while (!stream.atEnd()){
        QString line = stream.readLine();
        line = line.replace(regExp, "");
        line = line.remove("\n");
        line = line.remove("\t");

        QStringList words = line.split(" ");
        words.removeAll("");

        for (const QString& word : words){
            while (m_fileProcessingFutureWather.isSuspending()){
                QThread::msleep(100);
            }

            if (m_fileProcessingFutureWather.isCanceled()){
                return;
            }

            tempModel.addWord(word.toLower());
        }

        qDebug() << line;

        if (m_objectPtr != nullptr){
            m_objectPtr->setProperty("progressValue", stream.pos());
        }
    }

    QList<CHistogramModel::Item> itemList = tempModel.getWords();
    std::sort(itemList.begin(), itemList.end(), [](const CHistogramModel::Item& item1, const CHistogramModel::Item& item2){
        return item1.count > item2.count;
    });

    if (itemList.size() > 15){
        itemList.resize(15);
    }

    int maxCount = 0;
    if (!itemList.isEmpty()){
        maxCount = itemList[0].count;
    }

    if (m_objectPtr != nullptr){
        m_objectPtr->setProperty("maxCount", maxCount);
    }

    QDateTime finish = QDateTime::currentDateTime();

    int msecs = start.time().msecsTo(finish.time());
    qDebug () << "Time execution: " << msecs;

    m_fileProcessingResult = itemList;
}


void CFileController::fileOpening(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)){
        return;
    }

    m_fileData = file.readAll();

    file.close();
}


// public slots

void CFileController::open(QString fileName)
{
    if (m_objectPtr != nullptr){
        m_objectPtr->setProperty("state", "loading");
    }

    m_fileOpeningFutureWatcher.setFuture(QtConcurrent::run(&CFileController::fileOpening, this, fileName));
}


void CFileController::start()
{
    if (m_objectPtr != nullptr){
        m_objectPtr->setProperty("state", "processing");
    }

    if (m_fileProcessingFutureWather.isRunning()){
        return;
    }

    m_fileProcessingResult.clear();

    m_fileProcessingFutureWather.setFuture(QtConcurrent::run(&CFileController::fileProcessing, this));
}


void CFileController::pause()
{
    if (m_fileProcessingFutureWather.isRunning()){
        m_fileProcessingFutureWather.suspend();
    }

    if (m_objectPtr != nullptr){
        m_objectPtr->setProperty("state", "paused");
    }
}


void CFileController::resume(){
    if (m_fileProcessingFutureWather.isSuspending()){
        m_fileProcessingFutureWather.resume();

        if (m_objectPtr != nullptr){
            m_objectPtr->setProperty("state", "processing");
        }
    }
}


void CFileController::cancel()
{
    m_fileData.clear();

    if (m_fileProcessingFutureWather.isRunning()){
        m_fileProcessingFutureWather.cancel();

        m_fileProcessingFutureWather.waitForFinished();
    }

    m_model.resetModel();

    if (m_objectPtr != nullptr){
        m_objectPtr->setProperty("state", "default");
    }
}


void CFileController::onFileProcessingfinished()
{
    if (m_objectPtr != nullptr && !m_fileProcessingFutureWather.isCanceled()){
        m_objectPtr->setProperty("state", "processed");
    }

    m_model.setWords(m_fileProcessingResult);
}


void CFileController::onFileOpeningfinished()
{
    if (m_objectPtr != nullptr){
        m_objectPtr->setProperty("state", "loaded");
    }
}
