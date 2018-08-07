#ifndef CRC32_H
#define CRC32_H


class Crc32
{
public:
    Crc32();
    void reset();
    void update(unsigned char b);
    void update(char *buffer,int len);
    unsigned int getresult();
private:
    unsigned int Crc=0;
    unsigned int Crc32Table[256]={};

};

#endif // CRC32_H
