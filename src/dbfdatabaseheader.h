#ifndef DBFDATABASEHEADER_H
#define DBFDATABASEHEADER_H

#include <QByteArray>
#include <QDate>

class DbfDatabaseHeader
{
public:
    explicit DbfDatabaseHeader();
    static const int DbfHeaderInfoLength;

//    void setDataStream(QDataStream *dataStream);

    QByteArray getHeaderDataStream();
    void setHeaderDataStream(const QByteArray &bytes);

    int getHeaderDataLength();

    void setVersion(int version);
    int getVersion();

    void setModifyDate(QDate date);
    QDate getModifyDate();

    void setRecordNumber(int number);
    int getRecordNumber();

    void setHeaderLength(int len);
    int getHeaderLength();

    void setRecordLength(int len);
    int getRecordLength();

    void setUnfinished(bool isUnfinished);
    bool unfinished();

    void setDBaseIVFlag(QChar flag);
    QChar getDBaseIVFlage();

    void setMdxFlag(QChar flag);
    QChar getMdxFlag();

    void setLanguageId(int id);
    int getLanguageId();

private:
    void setFieldValue(const QByteArray &value, int pos, int len);
    QByteArray getFieldValue(int pos, int len);

private:
    QByteArray m_headerBytes;
//    QDataStream *m_dbf;

    static const int DbfVersionPos;
    static const int DbfVersionLen;
    static const int DbfModifyDatePos;
    static const int DbfModifyDateLen;
    static const int DbfRecordNumberPos;
    static const int DbfRecordNumberLen;
    static const int DbfHeaderLengthPos;
    static const int DbfHeaderLengthLen;
    static const int DbfRecordLengthPos;
    static const int DbfRecordLengthLen;
    static const int DbfUnfinishedFlagPos;
    static const int DbfUnfinishedFlagLen;
    static const int DbfDBaseIVFlagPos;
    static const int DbfDBaseIVFlagLen;
    static const int DbfMdxFlagPos;
    static const int DbfMdxFlagLen;
    static const int DbfLanguageIDPos;
    static const int DbfLanguageIDLen;
};

#endif // DBFDATABASEHEADER_H
