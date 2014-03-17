#include "dbfdatabase.h"
#include <QDebug>
#include <QDataStream>

const QByteArray DbfDatabase::DbfHeaderEndFlag(QByteArray::fromHex("0D"));
const QByteArray DbfDatabase::DbfDatabaseEndFlag(QByteArray::fromHex("1A"));
const int DbfDatabase::DbfRecordStartIndex = 1;
const int DbfDatabase::DbfDatabaseVersion = 3;

DbfDatabase *DbfDatabase::m_instance = NULL;

DbfDatabase::DbfDatabase(QObject *parent)
    : QObject(parent),
      m_dbfHeader(new DbfDatabaseHeader()),
      m_recordLen(DbfRecordStartIndex)
{
    m_dbfHeader->setVersion(DbfDatabaseVersion);
    m_dbfHeader->setModifyDate(QDate::currentDate());

    m_instance = this;
}

DbfDatabase::~DbfDatabase()
{
    delete m_dbfHeader;

    QHashIterator<QString, DbfRecord *> record(m_dbfRecords);
    while (record.hasNext())
    {
        record.next();
        delete record.value();
    }
    m_dbfRecords.clear();

    QHashIterator<QString, DbfField *> item(m_dbfFields);
    while (item.hasNext())
    {
        item.next();
        delete item.value();
    }
    m_dbfFields.clear();
}

DbfDatabase *DbfDatabase::getInstance()
{
    return m_instance;
}

void DbfDatabase::setDbfHeaderFromDataStream(const QByteArray &bytes)
{
    m_dbfHeader->setHeaderDataStream(bytes);

    int index = m_dbfHeader->getHeaderDataLength();
    while (bytes.size() - index >= DbfField::FieldStreamSize)
    {
        DbfField *field = new DbfField(bytes.mid(index, DbfField::FieldStreamSize));
        index += DbfField::FieldStreamSize;

        setField(field);
    }
}

void DbfDatabase::setDbfRecordFromDataStream(const QByteArray &bytes)
{
    int recordLen = m_dbfHeader->getRecordLength();
#if 1
    int index = 0;
    while (bytes.size() - index >= recordLen)
    {
        qDebug() << "   >" << index << recordLen << bytes.mid(index, recordLen);
        addRecord(new DbfRecord(m_dbfFields, bytes.mid(index, recordLen)));
        index += recordLen + 1;
    }
#else

    while (bytes.size() - index >= recordLen)
    {
        QByteArray recordBytes(bytes.mid(index, recordLen));

        QString indexFieldName(recordBytes.mid(field->getFieldPos(), field->getFieldLen()));

        if (m_dbfRecords.contains(indexFieldName))
        {
            DbfRecord *dbfRecord = m_dbfRecords.value(indexFieldName);
            foreach (const QString &item, m_dbfFieldList)
            {
                DbfField *fielditem = m_dbfFields.value(item);
                dbfRecord->setValue(item,
                                    recordBytes.mid(fielditem->getFieldPos(),
                                                    fielditem->getFieldLen()));
            }
        }
        else
        {
            DbfRecord *dbfRecord = new DbfRecord(m_dbfFields, recordLen);
            foreach (const QString &item, m_dbfFieldList)
            {
                DbfField *fielditem = m_dbfFields.value(item);
                dbfRecord->setValue(item,
                                    recordBytes.mid(fielditem->getFieldPos(),
                                                    fielditem->getFieldLen()));
            }
        }

        index += recordLen;
    }

#endif

}

int DbfDatabase::getRecordLength()
{
    return m_recordLen;
}

void DbfDatabase::setIndexField(const QString &field)
{
    m_indexField = field;

    DbfField *dbfField = m_dbfFields.value(m_indexField, NULL);

    if (dbfField == NULL)
        return ;

    dbfField->setFieldAsIndex(true);
}

bool DbfDatabase::loadFormFile(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "DbfDatabase::loadFormFile(" << filename << ") > file open failed";
        return false;
    }

    QDataStream dataStream(&file);

    QByteArray data;
    int filesize = file.size();
    data.resize(filesize);

    if (filesize != dataStream.readRawData(data.data(), filesize))
        return false;

    reloadDataStream(data);

    return true;
}

bool DbfDatabase::saveToFile(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "DbfDatabase::saveToFile(" << filename << ") > file open failed";
        return false;
    }

    QDataStream dataStream(&file);

    QByteArray data(createDataStream());

    if (data.size() != dataStream.writeRawData(data.data(), data.size()))
        return false;

    return true;
}

void DbfDatabase::setField(DbfField *field)
{
    m_dbfFieldList.append(field->getFieldName());

    m_dbfFields.insert(field->getFieldName(), field);
    field->setFieldPos(m_recordLen);

    m_recordLen += field->getFieldLen();

    if (m_indexField.isEmpty() && field->fieldIsIndex())
        m_indexField = field->getFieldName();
    else if (!m_indexField.isEmpty() && m_indexField == field->getFieldName())
        field->setFieldAsIndex(true);


}

QHash<QString, DbfField *> DbfDatabase::getFields()
{
    return m_dbfFields;
}

void DbfDatabase::addRecord(DbfRecord *record)
{
    bool isUpdate = false;
    DbfRecord *updatedRecord = NULL;
    if (!m_indexField.isEmpty())
    {
        QString newRecord(record->getValue(m_indexField).toByteArray());

        DbfRecord *item = m_dbfRecords.value(newRecord, NULL);

        if (item != NULL)
        {
            item->setRecordDataStream(record->getRecordDataStream());
            isUpdate = true;
            updatedRecord = item;
        }

    }

    if (!isUpdate)
    {
        m_dbfRecords.insert(record->getValue(m_indexField).toString(), record);
        updatedRecord = record;
    }
    else if (record != NULL)
    {
        delete record;
        record = NULL;
    }

    m_dbfHeader->setRecordNumber(m_dbfRecords.size());

    emit recordNumberChanged(m_dbfRecords.size());
    emit recordUpdated(updatedRecord->getRecordDataStream());
}

QByteArray DbfDatabase::createDataStream()
{
    // get Dbf database header size = headerSize + AllFieldSize
    int size = DbfDatabaseHeader::DbfHeaderInfoLength
            + m_dbfFields.size() * DbfField::FieldStreamSize
            + DbfHeaderEndFlag.size();

    // append fields bytes
    QByteArray fieldBytes;
    m_recordLen = DbfRecordStartIndex;

    foreach (QString fieldName, m_dbfFieldList)
    {
        DbfField *field = m_dbfFields.value(fieldName);
        fieldBytes.append(field->getFieldDataStream());
        m_recordLen += field->getFieldLen();
    }

    // set Header information
    m_dbfHeader->setHeaderLength(size);
    m_dbfHeader->setModifyDate(QDate::currentDate());
    m_dbfHeader->setRecordLength(m_recordLen);

    QByteArray dataBytes;

    // append header bytes
    qDebug() << "DbfDatabase::getDataStream() > header size ="
             << m_dbfHeader->getHeaderDataStream().size();
    dataBytes.append(m_dbfHeader->getHeaderDataStream());

    // append fields bytes
    dataBytes.append(fieldBytes);

    // append header end flag bytes
    dataBytes.append(DbfHeaderEndFlag);

    // append records bytes
    QList<DbfRecord *> dbfRecords = m_dbfRecords.values();
    foreach (DbfRecord *record, dbfRecords)
        dataBytes.append(record->getRecordDataStream());

    // append dbf file end flag bytes
    if (m_dbfRecords.size() > 0)
        dataBytes.append(DbfDatabaseEndFlag);

    return dataBytes;
}

void DbfDatabase::reloadDataStream(const QByteArray &bytes)
{
    qDebug() << "DbfDatabase::reloadDataStream(" << bytes.size()<<")";
    int skip = 0;
    // load header data stream
    qDebug(bytes.left(m_dbfHeader->getHeaderDataLength()).toHex());
    m_dbfHeader->setHeaderDataStream(bytes.left(m_dbfHeader->getHeaderDataLength()));
    skip += m_dbfHeader->getHeaderDataLength();

    // load field
    int headerLen = m_dbfHeader->getHeaderLength();

    // load dbf fields
    headerLen -= skip;
    while (headerLen > DbfField::FieldStreamSize)
    {
        DbfField *field = new DbfField(bytes.mid(skip, DbfField::FieldStreamSize));
        skip += DbfField::FieldStreamSize;
        headerLen -= DbfField::FieldStreamSize;
        setField(field);
    }
    skip += DbfHeaderEndFlag.size();

    // load dbf records
    int recordLen = m_dbfHeader->getRecordLength();
    int leftByteNum = bytes.size() - skip;

    while (leftByteNum > recordLen)
    {
        DbfRecord *dbfRecord = new DbfRecord(m_dbfFields, bytes.mid(skip, recordLen));
        m_dbfRecords.insert(dbfRecord->getValue(m_indexField).toString(), dbfRecord);
        leftByteNum -= recordLen;
        skip += recordLen;
    }
}
