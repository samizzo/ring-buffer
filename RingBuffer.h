#pragma once

class RingBuffer
{
    public:
        RingBuffer(int sizeBytes);
        ~RingBuffer();

        // Copies 'length' bytes from 'source' to the ring buffer.
        // Returns the number of bytes actually written, which may be less than 'length'
        // if there was insufficient space to write into the ring buffer.
        int Write(const void* source, int length);

        // Reads 'length' bytes from the ring buffer into 'dest'.
        // Returns the number of bytes actually read, which may be less than 'length'
        // if there was insufficient data in the ring buffer.
        int Read(void* dest, int length);

        // Returns true if the ring buffer has no space available for writing.
        bool IsFull() const;

        // Returns true if the ring buffer has no data in it.
        bool IsEmpty() const;

        // Returns the total number of bytes available for writing.
        int GetWriteBytesAvailable() const;

    private:
        int     m_sizeBytes;            // Size in bytes of the buffer.
        int     m_writeBytesAvailable;  // Number of bytes available for writing to in the ring buffer.
        int     m_readBytesAvailable;   // Number of bytes available for reading from in the ring buffer.
        int     m_writeIndex;           // Byte offset of the current write location.
        int     m_readIndex;            // Byte offset of the current read location.
        char*   m_buffer;
};

inline bool RingBuffer::IsFull() const
{
    return m_writeBytesAvailable == 0;
}

inline bool RingBuffer::IsEmpty() const
{
    return m_readBytesAvailable == 0;
}

inline int RingBuffer::GetWriteBytesAvailable() const
{
    return m_writeBytesAvailable;
}
