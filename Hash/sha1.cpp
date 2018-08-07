#include <stdio.h>
#include <stdlib.h>
#include "sha1.h"

#pragma warning (disable: 4996)		// fopen sprintf

uint32 SHA1::K[4] = {
    0x5A827999,
    0x6ED9EBA1,
    0x8F1BBCDC,
    0xCA62C1D6
};

SHA1::SHA1()
{
    Reset();
}

SHA1::~SHA1()
{

}

void SHA1::Reset()
{
    H[0] = 0x67452301;
    H[1] = 0xEFCDAB89;
    H[2] = 0x98BADCFE;
    H[3] = 0x10325476;
    H[4] = 0xC3D2E1F0;

    m_dataIndex = 0;
    m_dataSizeInBits = 0;
    m_computed = false;
    m_corrupted = false;
}

int SHA1::PushData( const void* data, uint32 sizeInBytes )
{
    if (m_computed)
    {
        return SHA1_DATA_ALREADY_COMPUTED;
    }

    if (uint64(-1) - m_dataSizeInBits < (uint64)sizeInBytes)
    {
        m_corrupted = true;
        return SHA1_DATA_TOO_LONG;
    }

    uint8* dataByte = (uint8*)data;

    while ( sizeInBytes-- )
    {
        m_dataBlock[m_dataIndex++] = *dataByte++;
        m_dataSizeInBits += 8;
        if (m_dataIndex == 64)
        {
            ProcessDataBlock();
        }
    }

    return SHA1_DATA_PUSH_SUCCEED;
}

uint32 SHA1::CircularLeftShift32(int bits, uint32 data)
{
    return (data << bits) | (data >> (32 - bits));
}

void SHA1::ProcessDataBlock()
{
    uint32 W[80];

    int t;
    uint32 A, B, C, D, E, temp;

    for (t = 0; t < 16; t++)
    {
        W[t] = (m_dataBlock[t * 4] << 24) | (m_dataBlock[t * 4 + 1] << 16)
          | (m_dataBlock[t * 4 + 2] << 8) | (m_dataBlock[t * 4 + 3]);
    }

    for ( ; t < 80; t++)
    {
        W[t] = CircularLeftShift32(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
    }

    A = H[0];
    B = H[1];
    C = H[2];
    D = H[3];
    E = H[4];

    for (t = 0; t < 20; t++)
    {
        temp = CircularLeftShift32(5, A) + ((B & C)|((~B) & D)) + E + W[t] + K[0];
        E = D;
        D = C;
        C = CircularLeftShift32(30, B);
        B = A;
        A = temp;
    }

    for (; t < 40; t++)
    {
        temp = CircularLeftShift32(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
        E = D;
        D = C;
        C = CircularLeftShift32(30, B);
        B = A;
        A = temp;
    }

    for (; t < 60; t++)
    {
        temp = CircularLeftShift32(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        E = D;
        D = C;
        C = CircularLeftShift32(30, B);
        B = A;
        A = temp;
    }

    for (; t < 80; t++)
    {
        temp = CircularLeftShift32(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
        E = D;
        D = C;
        C = CircularLeftShift32(30, B);
        B = A;
        A = temp;
    }

    H[0] += A;
    H[1] += B;
    H[2] += C;
    H[3] += D;
    H[4] += E;

    m_dataIndex = 0;
}

void SHA1::PadData()
{
    m_dataBlock[m_dataIndex++] = 0x80;
    if (m_dataIndex > 56)	// not enough space for the length data;
    {
        while(m_dataIndex < 64)
        {
            m_dataBlock[m_dataIndex++] = 0;
        }

        ProcessDataBlock();
    }

    while(m_dataIndex < 56)
    {
        m_dataBlock[m_dataIndex++] = 0;
    }

    for (int i = 56; m_dataIndex < 64; i -= 8)
    {
        m_dataBlock[m_dataIndex++] = (m_dataSizeInBits >> i) & 0xFF;
    }

    ProcessDataBlock();
}

bool SHA1::GetResult(uint32 msgDigest[5])
{
    if (m_corrupted)
    {
        return false;
    }

    if (!m_computed)
    {
        PadData();
        m_computed = true;
    }

    for (int i = 0; i < 5; i++)
    {
        msgDigest[i] = H[i];
    }

    return true;
}

bool SHA1::Get( const void* data, uint32 sizeInBytes, uint32 msgDigest[5] )
{
    SHA1 s;
    s.PushData(data, sizeInBytes);
    return s.GetResult(msgDigest);
}

void SHA1::ResultToHex( uint32 msgDigest[5], TCHAR* strMsgDigest, bool bUpCase )
{
    for (int i = 0; i < 5; i++, strMsgDigest += 8)
    {
        sprintf(strMsgDigest, bUpCase ? ("%08X") : ("%08x"), msgDigest[i]);
    }

    *strMsgDigest = '\0';
}

bool SHA1::GetHexString( TCHAR* strMsgDigest)
{
    uint32 msg[5];

    if (!GetResult(msg))
    {
        *strMsgDigest = '\0';
        return false;
    }
    else
    {
        ResultToHex(msg, strMsgDigest, true);
        return true;
    }
}

#pragma warning (default: 4996)		// fopen sprintf
