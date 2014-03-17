#include "dbffield.h"
#include "dataconvert.h"
#include <QDebug>

const int DbfField::FieldStreamSize   = 32;
const int DbfField::FieldNamePos      = 0;
const int DbfField::FieldNameLen      = 10;
const int DbfField::FieldTypePos      = 11;
const int DbfField::FieldTypeLen      = 1;
const int DbfField::FieldPositionPos  = 12;
const int DbfField::FieldPositionLen  = 3;
const int DbfField::FieldLengthPos    = 16;
const int DbfField::FieldLengthLen    = 1;
const int DbfField::FieldDotPos       = 17;
const int DbfField::FieldDotLen       = 1;
const int DbfField::FieldDBaseIVIDPos = 20;
const int DbfField::FieldDBaseIVIDLen = 1;
const int DbfField::FieldIsIndexPos   = 31;
const int DbfField::FieldIsIndexLen   = 1;

DbfField::DbfField()
{
    initFieldMap();
    m_fieldBytes.resize(FieldStreamSize);
    m_fieldBytes.fill(0);
}

DbfField::DbfField(const QByteArray &bytes)
{
    initFieldMap();
    m_fieldBytes = bytes;
}

QByteArray DbfField::getFieldDataStream()
{
    return m_fieldBytes;
}

void DbfField::setFieldDataStream(const QByteArray &bytes)
{
    m_fieldBytes.clear();
    m_fieldBytes = bytes;
}

int DbfField::getFieldDataLength()
{
    return FieldStreamSize;
}

void DbfField::initFieldMap()
{
    m_fieldMap.insert('C', Character);
    m_fieldMap.insert('D', Date);
    m_fieldMap.insert('F', FloatingPoint);
    m_fieldMap.insert('L', Logical);
    m_fieldMap.insert('M', Memo);
    m_fieldMap.insert('N', Integer);
    m_fieldMap.insert('P', Image);
    m_fieldMap.insert('c', Character);
    m_fieldMap.insert('d', Date);
    m_fieldMap.insert('f', FloatingPoint);
    m_fieldMap.insert('l', Logical);
    m_fieldMap.insert('m', Memo);
    m_fieldMap.insert('n', Integer);
    m_fieldMap.insert('p', Image);
}


QChar DbfField::fieldTypeValue(FieldType type)
{
    switch (type)
    {
        case Character:     return 'C';
        case Date:          return 'D';
        case FloatingPoint: return 'F';
        case Logical:       return 'L';
        case Memo:          return 'M';
        case Integer:       return 'N';
        case Image:         return 'P';
    }

    return 0;
}

void DbfField::setFieldType(FieldType type)
{
    QByteArray typeBytes;
    typeBytes.append(fieldTypeValue(type).toAscii());
    setFieldValue(typeBytes, FieldTypePos, FieldTypeLen);
}

DbfField::FieldType DbfField::getFieldType()
{
    return m_fieldMap.value(getFieldValue(FieldTypePos, FieldTypeLen).at(0));
}

QString DbfField::getFieldName()
{
    return QString::fromUtf8(getFieldValue(FieldNamePos, FieldNameLen));
}

void DbfField::setFieldName(const QString &name)
{
    QByteArray fieldname(name.toUtf8());

    setFieldValue(fieldname, FieldNamePos, FieldNameLen);
}

void DbfField::setFieldPos(int pos)
{
    setFieldValue(DataConvert::intToByte(pos),
                  FieldPositionPos,
                  FieldPositionLen);
}

int DbfField::getFieldPos()
{
    return DataConvert::bytesToInt(getFieldValue(FieldPositionPos, FieldPositionLen));
}

void DbfField::setFieldLen(int len)
{
    setFieldValue(DataConvert::intToByte(len), FieldLengthPos, FieldLengthLen);
}

int DbfField::getFieldLen()
{
    return DataConvert::bytesToInt(getFieldValue(FieldLengthPos, FieldLengthLen));
}

void DbfField::setFieldDotPos(int pos)
{
    setFieldValue(DataConvert::intToByte(pos), FieldDotPos, FieldDotLen);
}

int DbfField::getFieldDotPos()
{
    return DataConvert::bytesToInt(getFieldValue(FieldDotPos, FieldDotLen));
}

void DbfField::setDBaseIVId(int id)
{
    setFieldValue(DataConvert::intToByte(id), FieldDBaseIVIDPos, FieldDBaseIVIDLen);
}

int DbfField::getDBaseIVId()
{
    return DataConvert::bytesToInt(getFieldValue(FieldDBaseIVIDPos, FieldDBaseIVIDLen));
}

void DbfField::setFieldAsIndex(bool isIndex)
{
    setFieldValue(DataConvert::intToByte(isIndex ? 1 : 0),
                  FieldIsIndexPos, FieldIsIndexLen);
}

bool DbfField::fieldIsIndex()
{
    return DataConvert::bytesToInt(getFieldValue(FieldIsIndexPos, FieldIsIndexLen)) == 0 ? false : true;
}

bool DbfField::operator==(DbfField &field)
{
    return getFieldName().compare(field.getFieldName()) == 0 ? true : false;
}

inline void DbfField::setFieldValue(const QByteArray &value, int pos, int len)
{
    QByteArray bytes(value);
    while (bytes.size() < len)
        bytes.append(QChar(0));

    m_fieldBytes.replace(pos, len, bytes, len);
}

inline QByteArray DbfField::getFieldValue(int pos, int len)
{
    return m_fieldBytes.mid(pos, len);
}
