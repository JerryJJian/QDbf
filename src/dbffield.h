#ifndef DBFFIELD_H
#define DBFFIELD_H

#include <QHash>
#include "DbfDatabase_global.h"

class DBFDATABASESHARED_EXPORT DbfField
{
public:
    explicit DbfField();
    explicit DbfField(const QByteArray &bytes);

    static const int FieldStreamSize;

    enum FieldType
    {
        Character = 0x0,
        Date,
        FloatingPoint,
        Logical,
        Memo,
        Integer,
        Image
    };

    QByteArray getFieldDataStream();
    void setFieldDataStream(const QByteArray &bytes);

    int getFieldDataLength();

    QString getFieldName();
    void setFieldName(const QString &name);

    void setFieldType(FieldType type);
    FieldType getFieldType();

    void setFieldPos(int pos);
    int getFieldPos();

    void setFieldLen(int len);
    int getFieldLen();

    void setFieldDotPos(int pos);
    int getFieldDotPos();

    void setDBaseIVId(int id);
    int getDBaseIVId();

    void setFieldAsIndex(bool isIndex);
    bool fieldIsIndex();

    bool operator==(DbfField &field);

signals:
    void errorOccured(int errorNo);

private:
    void initFieldMap();
    QChar fieldTypeValue(FieldType type);

    void setFieldValue(const QByteArray &value, int pos, int len);
    QByteArray getFieldValue(int pos, int len);

private:
    QByteArray m_fieldBytes;
    QHash<QChar, FieldType> m_fieldMap;

    static const int FieldNamePos;
    static const int FieldNameLen;
    static const int FieldTypePos;
    static const int FieldTypeLen;
    static const int FieldPositionPos;
    static const int FieldPositionLen;
    static const int FieldLengthPos;
    static const int FieldLengthLen;
    static const int FieldDotPos;
    static const int FieldDotLen;
    static const int FieldDBaseIVIDPos;
    static const int FieldDBaseIVIDLen;
    static const int FieldIsIndexPos;
    static const int FieldIsIndexLen;
};

#endif // DBFFIELD_H
