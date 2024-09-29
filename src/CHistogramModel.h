#ifndef CHISTOGRAMMODEL_H
#define CHISTOGRAMMODEL_H


// Qt includes
#include <QtCore/QAbstractListModel>


class CHistogramModel: public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        WordRole = Qt::UserRole + 1,
        CountRole
    };

    struct Item
    {
        QString word;
        int count;
    };

    CHistogramModel(QObject* parent = nullptr);

    bool addWord(const QString& word);
    int getWordIndex(const QString& word) const;
    void resetModel();

    const QList<Item>& getWords() const;
    void setWords(QList<Item> wordList);

    // reimplemented (QAbstractListModel)
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

private:
    QList<Item> m_wordList;
    QHash<QString, int> m_hashMap;
};

#endif // CHISTOGRAMMODEL_H
