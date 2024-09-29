#include "CHistogramModel.h"


// public methods

CHistogramModel::CHistogramModel(QObject* parent)
    :QAbstractListModel(parent)
{
}


bool CHistogramModel::addWord(const QString& word)
{
    if (m_hashMap.contains(word)){
        int index = m_hashMap[word];

        Item item = m_wordList[index];
        item.count++;
        m_wordList[index] = item;

        QModelIndex modelIndex = createIndex(index, 0);
        emit dataChanged(modelIndex, modelIndex, QVector<int>() << CountRole);
    }
    else{
        beginInsertRows(QModelIndex(), m_wordList.count(), m_wordList.count());

        Item item;
        item.word = word;
        item.count = 1;

        m_wordList.append(item);
        m_hashMap[word] = m_wordList.count() - 1;

        endInsertRows();
    }

    return true;
}


int CHistogramModel::getWordIndex(const QString& word) const
{
    for (int i = 0; i < m_wordList.size(); i++){
        Item item = m_wordList[i];
        if (item.word == word){
            return i;
        }
    }

    return -1;
}


void CHistogramModel::resetModel()
{
    beginResetModel();
    m_wordList.clear();
    m_hashMap.clear();
    endResetModel();
}


const QList<CHistogramModel::Item>& CHistogramModel::getWords() const
{
    return m_wordList;
}


void CHistogramModel::setWords(QList<Item> wordList)
{
    beginResetModel();
    m_wordList.clear();
    m_hashMap.clear();

    for (const Item& item : wordList){
        m_wordList.append(item);       
        m_hashMap[item.word] = m_wordList.count() - 1;
    }

    endResetModel();
}


// reimplemented (QAbstractListModel)

int CHistogramModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()){
        return 0;
    }

    return m_wordList.size();
}


QVariant CHistogramModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_wordList.count()){
        return QVariant();
    }

    Item item = m_wordList[index.row()];

    switch (role){
    case WordRole:
        return item.word;
    case CountRole:
        return item.count;
    default:
        return QVariant();
    }
}


QHash<int, QByteArray> CHistogramModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[WordRole] = "word";
    roles[CountRole] = "count";

    return roles;
}
