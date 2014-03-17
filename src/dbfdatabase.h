#ifndef DBFDATABASE_H
#define DBFDATABASE_H

#include <QFile>
#include <QObject>
#include <QStringList>
#include "dbffield.h"
#include "dbfrecord.h"
#include "dbfdatabaseheader.h"
#include "DbfDatabase_global.h"

class DBFDATABASESHARED_EXPORT DbfDatabase : public QObject
{
    Q_OBJECT
public:
    explicit DbfDatabase(QObject *parent = 0);
    ~DbfDatabase();

    static DbfDatabase *getInstance();

    enum OpenMode
    {
        ReadOnly,
        ReadWrite,
        Truncate
    };

    int getRecordLength();

    void setIndexField(const QString &field);

    void setField(DbfField *field);
    QHash<QString, DbfField *> getFields();

    void addRecord(DbfRecord *record);
    QByteArray createDataStream();

signals:
    void recordNumberChanged(int number);
    void loadDbfDatabaseError();
    void saveDbfDatabaseError();
    void headerUpdated(const QByteArray &bytes);
    void recordUpdated(const QByteArray &bytes);

public slots:
    void setDbfHeaderFromDataStream(const QByteArray &bytes);
    void setDbfRecordFromDataStream(const QByteArray &bytes);
    bool loadFormFile(const QString &filename);
    bool saveToFile(const QString &filename);
    void reloadDataStream(const QByteArray &bytes);

private:
    DbfDatabaseHeader *m_dbfHeader;
    QStringList m_dbfFieldList;
    QHash<QString, DbfField *> m_dbfFields;
    QHash<QString, DbfRecord *> m_dbfRecords;

    QString m_indexField;

    int m_recordLen;

    static DbfDatabase *m_instance;

    static const QByteArray DbfHeaderEndFlag;
    static const QByteArray DbfDatabaseEndFlag;
    static const int DbfRecordStartIndex;
    static const int DbfDatabaseVersion;

};

#endif // DBFDATABASE_H
