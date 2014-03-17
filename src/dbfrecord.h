#ifndef DBFRECORD_H
#define DBFRECORD_H

#include <QByteArray>
#include <QHash>
#include <QVariant>
#include <QString>
#include "dbffield.h"
#include "DbfDatabase_global.h"

class DBFDATABASESHARED_EXPORT DbfRecord
{
public:
    DbfRecord(const QHash<QString, DbfField *> &fields, int recordLen);
    DbfRecord(const QHash<QString, DbfField *> &fields, const QByteArray &bytes);

    void setValue(const QHash<QString, QVariant> &record);
    void setValue(const QString &key, const QVariant &value);
    QVariant getValue(const QString &field);

    QByteArray getRecordDataStream();
    void setRecordDataStream(const QByteArray &bytes);

private:
    void setFieldValue(DbfField *field, const QByteArray &value);
    QByteArray getFieldValue(DbfField *field);

private:
    QHash<QString, DbfField *> m_dbfFields;
    QByteArray m_recordBytes;
    int m_recordLen;
};

#endif // DBFRECORD_H
