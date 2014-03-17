#include "dbfdatabaseheader.h"
#include "dataconvert.h"
#include <QDebug>

const int DbfDatabaseHeader::DbfHeaderInfoLength = 32;
const int DbfDatabaseHeader::DbfVersionPos = 0;
const int DbfDatabaseHeader::DbfVersionLen = 1;
const int DbfDatabaseHeader::DbfModifyDatePos = 1;
const int DbfDatabaseHeader::DbfModifyDateLen = 3;
const int DbfDatabaseHeader::DbfRecordNumberPos = 4;
const int DbfDatabaseHeader::DbfRecordNumberLen = 4;
const int DbfDatabaseHeader::DbfHeaderLengthPos = 8;
const int DbfDatabaseHeader::DbfHeaderLengthLen = 2;
const int DbfDatabaseHeader::DbfRecordLengthPos = 10;
const int DbfDatabaseHeader::DbfRecordLengthLen = 2;
const int DbfDatabaseHeader::DbfUnfinishedFlagPos = 14;
const int DbfDatabaseHeader::DbfUnfinishedFlagLen = 1;
const int DbfDatabaseHeader::DbfDBaseIVFlagPos = 15;
const int DbfDatabaseHeader::DbfDBaseIVFlagLen = 1;
const int DbfDatabaseHeader::DbfMdxFlagPos = 28;
const int DbfDatabaseHeader::DbfMdxFlagLen = 1;
const int DbfDatabaseHeader::DbfLanguageIDPos = 29;
const int DbfDatabaseHeader::DbfLanguageIDLen = 1;

DbfDatabaseHeader::DbfDatabaseHeader()
{
    m_headerBytes.resize(DbfHeaderInfoLength);
    m_headerBytes.fill(0);
}

QByteArray DbfDatabaseHeader::getHeaderDataStream()
{
    return m_headerBytes;
}

void DbfDatabaseHeader::setHeaderDataStream(const QByteArray &bytes)
{
    m_headerBytes.clear();

    QByteArray data(bytes);
    while (data.size() < DbfHeaderInfoLength)
        data.append(QChar(0));

    m_headerBytes = data.left(DbfHeaderInfoLength);
}

int DbfDatabaseHeader::getHeaderDataLength()
{
    return DbfHeaderInfoLength;
}

void DbfDatabaseHeader::setVersion(int version)
{
    setFieldValue(DataConvert::intToByte(version), DbfVersionPos, DbfVersionLen);
}

int DbfDatabaseHeader::getVersion()
{
    return DataConvert::bytesToInt(getFieldValue(DbfVersionPos, DbfVersionLen));
}

void DbfDatabaseHeader::setModifyDate(QDate date)
{
    QByteArray dateByte;

    dateByte.append(DataConvert::intToByte(date.toString("yy").toInt()).at(0));
    dateByte.append(DataConvert::intToByte(date.toString("M").toInt()).at(0));
    dateByte.append(DataConvert::intToByte(date.toString("d").toInt()).at(0));

    setFieldValue(dateByte, DbfModifyDatePos, DbfModifyDateLen);
}

QDate DbfDatabaseHeader::getModifyDate()
{
    QByteArray bytes(getFieldValue(DbfModifyDatePos, DbfModifyDateLen));

    QDate date;
    date.setYMD(DataConvert::bytesToInt(bytes.left(1)),
                DataConvert::bytesToInt(bytes.mid(1, 1)),
                DataConvert::bytesToInt(bytes.right(1)));

    return date;
}

void DbfDatabaseHeader::setRecordNumber(int number)
{
    setFieldValue(DataConvert::intToByte(number), DbfRecordNumberPos, DbfRecordNumberLen);
}

int DbfDatabaseHeader::getRecordNumber()
{
    return DataConvert::bytesToInt(getFieldValue(DbfRecordNumberPos, DbfRecordNumberLen));
}

void DbfDatabaseHeader::setHeaderLength(int len)
{
    setFieldValue(DataConvert::intToByte(len), DbfHeaderLengthPos, DbfHeaderLengthLen);
}

int DbfDatabaseHeader::getHeaderLength()
{
    return DataConvert::bytesToInt(getFieldValue(DbfHeaderLengthPos, DbfHeaderLengthLen));
}

void DbfDatabaseHeader::setRecordLength(int len)
{
    setFieldValue(DataConvert::intToByte(len), DbfRecordLengthPos, DbfRecordLengthLen);
}

int DbfDatabaseHeader::getRecordLength()
{
    return DataConvert::bytesToInt(getFieldValue(DbfRecordLengthPos, DbfRecordLengthLen));
}

void DbfDatabaseHeader::setUnfinished(bool isUnfinished)
{
    setFieldValue(DataConvert::intToByte(isUnfinished ? 1 : 0), DbfUnfinishedFlagPos, DbfUnfinishedFlagLen);
}

bool DbfDatabaseHeader::unfinished()
{
    return DataConvert::bytesToInt(getFieldValue(DbfUnfinishedFlagPos, DbfUnfinishedFlagLen)) == 0 ? false : true;
}

void DbfDatabaseHeader::setDBaseIVFlag(QChar flag)
{
    setFieldValue(QByteArray(1, flag.toAscii()), DbfDBaseIVFlagPos, DbfDBaseIVFlagLen);
}

QChar DbfDatabaseHeader::getDBaseIVFlage()
{
    return getFieldValue(DbfDBaseIVFlagPos, DbfDBaseIVFlagLen).at(0);
}

void DbfDatabaseHeader::setMdxFlag(QChar flag)
{
    setFieldValue(QByteArray(1, flag.toAscii()), DbfDBaseIVFlagPos, DbfDBaseIVFlagLen);
}

QChar DbfDatabaseHeader::getMdxFlag()
{
    return getFieldValue(DbfDBaseIVFlagPos, DbfDBaseIVFlagLen).at(0);
}

void DbfDatabaseHeader::setLanguageId(int len)
{
    setFieldValue(DataConvert::intToByte(len), DbfRecordLengthPos, DbfRecordLengthLen);
}

int DbfDatabaseHeader::getLanguageId()
{
    return DataConvert::bytesToInt(getFieldValue(DbfRecordLengthPos, DbfRecordLengthLen));
}

inline void DbfDatabaseHeader::setFieldValue(const QByteArray &value, int pos, int len)
{
    m_headerBytes.replace(pos, len, value, len);
}

inline QByteArray DbfDatabaseHeader::getFieldValue(int pos, int len)
{
    return m_headerBytes.mid(pos, len);
}
