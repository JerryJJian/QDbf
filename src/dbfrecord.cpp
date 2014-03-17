#include "dbfrecord.h"
#include <QDebug>

DbfRecord::DbfRecord(const QHash<QString, DbfField *> &fields, int recordLen)
    : m_dbfFields(fields),
      m_recordLen(recordLen)
{
    m_recordBytes.resize(m_recordLen);
    m_recordBytes.fill(0x20);
}

DbfRecord::DbfRecord(const QHash<QString, DbfField *> &fields, const QByteArray &bytes)
    : m_dbfFields(fields),
      m_recordBytes(bytes),
      m_recordLen(bytes.size())
{
}

void DbfRecord::setValue(const QHash<QString, QVariant> &record)
{
    foreach (DbfField *field, m_dbfFields.values())
    {
        QByteArray valueBytes;
        QVariant value(record.value(field->getFieldName()));

        if (value.isNull())
            continue ;

        switch (field->getFieldType())
        {
            case DbfField::Character:
            case DbfField::Date:
            case DbfField::Integer:
            case DbfField::FloatingPoint:
            case DbfField::Memo:
            case DbfField::Image:
                valueBytes = value.toByteArray();
                break;
            case DbfField::Logical:
                valueBytes = value.toBool() ? QByteArray("T") : QByteArray("F");
                break;
        }

        if (valueBytes.size() > field->getFieldLen())
            valueBytes = valueBytes.left(field->getFieldLen());

        setFieldValue(field, valueBytes);
    }
}

void DbfRecord::setValue(const QString &key, const QVariant &value)
{
    DbfField *dbfField = m_dbfFields.value(key, NULL);

    if (dbfField == NULL)
        return ;

    QByteArray valueBytes;
    switch (dbfField->getFieldType())
    {
        case DbfField::Character:
        case DbfField::Date:
        case DbfField::Integer:
        case DbfField::FloatingPoint:
        case DbfField::Memo:
        case DbfField::Image:
            valueBytes = value.toByteArray();
            break;
        case DbfField::Logical:
            valueBytes = value.toBool() ? QByteArray("T") : QByteArray("F");
            break;
    }

    setFieldValue(dbfField, valueBytes);
}

QVariant DbfRecord::getValue(const QString &field)
{
    DbfField *dbfField = m_dbfFields.value(field, NULL);

    if (dbfField == NULL)
        return QVariant();

    return QVariant(getFieldValue(dbfField));
}

QByteArray DbfRecord::getRecordDataStream()
{
//    qDebug() << "DbfRecord::getRecordDataStream() > m_recordBytes =" << m_recordBytes;
    return m_recordBytes;
}

void DbfRecord::setRecordDataStream(const QByteArray &bytes)
{
    m_recordBytes.clear();
    m_recordBytes = bytes;
//    qDebug() << "DbfRecord::setRecordDataStream(" << m_recordBytes << ")";
}

inline void DbfRecord::setFieldValue(DbfField *field, const QByteArray &value)
{
    QByteArray bytes(value);

    while (bytes.size() < field->getFieldLen())
        bytes.append(QChar(0x20));

    m_recordBytes.replace(field->getFieldPos(),
                          field->getFieldLen(),
                          bytes,
                          field->getFieldLen());
}

inline QByteArray DbfRecord::getFieldValue(DbfField *field)
{
    return m_recordBytes.mid(field->getFieldPos(), field->getFieldLen());
}
