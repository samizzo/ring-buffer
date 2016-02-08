#include "RingBuffer.h"
#include <memory.h>
#include <stdlib.h>
#include <assert.h>

RingBuffer::RingBuffer(int sizeBytes)
{
    assert(sizeBytes > 0);
    m_sizeBytes = sizeBytes;
    m_buffer = (char*)malloc(sizeBytes);
    assert(m_buffer);
    m_writeBytesAvailable = sizeBytes;
    m_readBytesAvailable = 0;
    m_writeIndex = 0;
    m_readIndex = 0;
}

RingBuffer::~RingBuffer()
{
    free(m_buffer);
}

int RingBuffer::Write(const void* source, int length)
{
    assert(m_buffer);
    assert(source);
    assert(length >= 0);
    assert(m_writeBytesAvailable >= 0);

    // If there is no space or nothing to write then don't do anything.
    if (IsFull() || length == 0)
        return 0;

    // Clamp the length to the number of bytes available for writing.
    if (length > m_writeBytesAvailable)
        length = m_writeBytesAvailable;

    int remainingWriteBytes = m_sizeBytes - m_writeIndex;
    if (length > remainingWriteBytes)
    {
        // If the number of bytes to write is bigger than the remaining bytes
        // in the buffer, we have to wrap around and write into two regions.
        memcpy(m_buffer + m_writeIndex, source, remainingWriteBytes);
        memcpy(m_buffer, (char*)source + remainingWriteBytes, length - remainingWriteBytes);
    }
    else
    {
        // No wrapping, only one region to write to, which starts from the write index.
        memcpy(m_buffer + m_writeIndex, source, length);
    }

    // Increment the write index and wrap around at the end.
    m_writeIndex = (m_writeIndex + length) % m_sizeBytes;

    // Update the read and write sizes.
    m_writeBytesAvailable -= length;
    m_readBytesAvailable += length;

    return length;
}

int RingBuffer::Read(void* dest, int length)
{
    assert(m_buffer);
    assert(dest);
    assert(length >= 0);
    assert(m_readBytesAvailable >= 0);

    // If there is no data in the buffer or nothing to read then don't do anything.
    if (IsEmpty() || length == 0)
        return 0;

    // Clamp the length to the maximum number of bytes available for reading.
    if (length > m_readBytesAvailable)
        length = m_readBytesAvailable;

    int remainingReadBytes = m_sizeBytes - m_readIndex;
    if (length > remainingReadBytes)
    {
        // If the number of bytes to read is bigger than the remaining bytes
        // in the buffer, we have to wrap around and read from two regions.
        memcpy(dest, m_buffer + m_readIndex, remainingReadBytes);
        memcpy((char*)dest + remainingReadBytes, m_buffer, length - remainingReadBytes);
    }
    else
    {
        // No wrapping, only one region to read from, which starts from the read index.
        memcpy(dest, m_buffer + m_readIndex, length);
    }

    // Increment the read index and wrap around at the end.
    m_readIndex = (m_readIndex + length) % m_sizeBytes;

    // Update the read and write sizes.
    m_writeBytesAvailable += length;
    m_readBytesAvailable -= length;

    return length;
}
