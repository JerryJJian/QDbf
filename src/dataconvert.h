#ifndef DATACONVERT_H
#define DATACONVERT_H

#include <QByteArray>

class DataConvert
{
public:

    static int bytesToInt(QByteArray bytes);
    static QByteArray intToByte(int number);
};

#endif // DATACONVERT_H
