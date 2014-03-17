#include "dataconvert.h"


int DataConvert::bytesToInt(QByteArray bytes)
{
    int integer = bytes[0] & 0x000000FF;

    if (bytes.size() >= 2)
        integer |= ((bytes[1] << 8) & 0x0000FF00);

    if (bytes.size() >= 3)
        integer |= ((bytes[2] << 16) & 0x00FF0000);

    if (bytes.size() >= 3)
        integer |= ((bytes[3] << 24) & 0xFF000000);

    return integer;
}

QByteArray DataConvert::intToByte(int number)
{
    QByteArray bytes;

    bytes.resize(4);
    bytes[0] = (uchar)  (0x000000ff & number);
    bytes[1] = (uchar) ((0x0000ff00 & number) >> 8);
    bytes[2] = (uchar) ((0x00ff0000 & number) >> 16);
    bytes[3] = (uchar) ((0xff000000 & number) >> 24);

    return bytes;
}
