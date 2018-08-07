#include "crc32.h"

Crc32::Crc32()
{
    reset();
}

void Crc32::reset()
{
    int i=0,j=0;
    for (i = 0; i < 256; i++)
    {
        Crc = i;
        for (j = 0; j < 8; j++)
        {
            if (Crc & 1)
                Crc = (Crc >> 1) ^ 0xEDB88320;
            else
                Crc >>= 1;
        }
        Crc32Table[i] = Crc;
    }
    Crc=0xffffffff;
}

void Crc32::update(unsigned char b)
{
    Crc = (Crc >> 8)^ Crc32Table[(Crc & 0xFF) ^ b];
}

void Crc32::update(char *buffer, int len)
{
    for(int i=0;i<len;i++)
    {
        update(buffer[i]);
    }
}

unsigned int Crc32::getresult()
{
    Crc ^= 0xFFFFFFFF;
    return Crc;
}
