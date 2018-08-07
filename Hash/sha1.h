#ifndef _SHA1_H_
#define _SHA1_H_

//#include "Hash.h"

typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef char TCHAR;
class SHA1 //: public Hash
{
public:
    SHA1();
    virtual ~SHA1();

    enum{
        SHA1_DATA_PUSH_SUCCEED			=	 0,
        SHA1_DATA_TOO_LONG				=	-1,
        SHA1_DATA_ALREADY_COMPUTED		=	-2,
        SHA1_FILE_OPEN_FAILED			=	-3,
    };

    int PushData(const void* data, uint32 sizeInBytes);

    // make sure the length of strMsgDigest at least 41 (40 + '\0')
    bool GetHexString(TCHAR* strMsgDigest);

    //////////////////////////////////////////////////////////////////////////

    void Reset();

    bool GetResult(uint32 msgDigest[5]);

    static void ResultToHex(uint32 msgDigest[5], TCHAR* strMsgDigest, bool bUpCase);
    static bool Get(const void* data, uint32 sizeInBytes, uint32 msgDigest[5]);

private:
    void ProcessDataBlock();
    void PadData();
    inline uint32 CircularLeftShift32(int bits, uint32 data);

    static uint32 K[4];

    uint32 H[5];
    uint8 m_dataBlock[64];
    int m_dataIndex;
    uint64 m_dataSizeInBits;
    bool m_computed;
    bool m_corrupted;
};


#endif // _SHA1_H_
