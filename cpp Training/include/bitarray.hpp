/*  ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~
* Version       1.0.0                                           *
~ Date          05/01/2022                                      ~
~ Description   BitArray API                                    ~
* Group         R&D 110                                         *
~ Company       ILRD Ramat Gan                                  ~
* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#ifndef __ILRD_RD110_BITARRAY_HPP__
#define __ILRD_RD110_BITARRAY_HPP__

#include <cstddef>      // size_t
#include <stdexcept>    // std::out_of_range
#include <cstring> //memset

static const size_t WORD_BITS = 64;
static const size_t CHAR_BIT = 8;

namespace ilrd
{

    static size_t MyPower(size_t num, int power);

    template <size_t N>
    class BitArray
    {
    private:
        class BitArrayProxy;

    public:
        explicit BitArray() throw();

        // Indexing
        BitArrayProxy operator[](size_t index_) throw(std::out_of_range);
        bool operator[](size_t index_) const throw(std::out_of_range);

        // Bitwise Operations
        BitArray &operator|=(const BitArray &other_) throw()
        {
            int blocks = sizeof(m_bitarray) / sizeof(size_t);

            for (int i = 0; i < blocks; ++i)
            {
                m_bitarray[i] |= other_.m_bitarray[i];
            }

            return *this;
        }
        BitArray &operator&=(const BitArray &other_) throw()
        {

            int blocks = sizeof(m_bitarray) / sizeof(size_t);

            for (int i = 0; i < blocks; ++i)
            {
                m_bitarray[i] &= other_.m_bitarray[i];
            }

            return *this;
        }

        BitArray &operator^=(const BitArray &other_) throw()
        {

            int blocks = sizeof(m_bitarray) / sizeof(size_t);

            for (int i = 0; i < blocks; ++i)
            {
                m_bitarray[i] ^= other_.m_bitarray[i];
            }

            return *this;
        }

        // Comparison
        bool operator==(const BitArray &other_) const throw()
        {
            size_t size = sizeof(m_bitarray);

            return !std::memcmp(m_bitarray, other_.m_bitarray, size);
        }

        bool operator!=(const BitArray &other_) const throw()
        {
            return !(*this == other_);
        }

        // BitArray Functionality
        void Set(size_t index_, bool val_) throw(std::out_of_range) // SetBit
        {
            ValidateIndex(index_);
            BitArrayProxy(*this, index_) = val_;
            // bit = val_;
        }

        void Set(bool val_) throw() // SetAll
        {
            int mask = (true == val_) ? ~0 : 0;
            std::memset(m_bitarray, mask, sizeof(m_bitarray));

            size_t lastIndex = m_lastBit / WORD_BITS;
            size_t lastVal = (true == val_) ? MyPower(2, (m_lastBit % WORD_BITS)+1) - 1 : 0;
            m_bitarray[lastIndex] = lastVal;
    }

    bool Get(size_t index_) const throw(std::out_of_range)// GetBit
    {
        ValidateIndex(index_);
        BitArrayProxy bit(const_cast<BitArray&>(*this), index_);
        return bit;
    }

    void Flip(size_t index_) throw(std::out_of_range) // FlipBit
    {
        ValidateIndex(index_);

        bool curr = (*this)[index_];
            (*this)[index_] = curr ^ 1;

    }
    void Flip() throw() // FlipAll
    {
        for (size_t i = 0; i <= m_lastBit; ++i)
        {
            bool curr = (*this)[i];
            (*this)[i] = curr ^ 1;
        }
    }

    size_t CountSetBits() const throw() // with LUT
    {
        size_t count = 0;
        for (size_t i = 0; i <= m_lastBit; ++i)
        {
            count += (((*this)[i]) == true);
        }

        return count;
    }

    // Misc
    std::string ToString() const throw()
    {
        std::string result = "";
    
        for (size_t i = 0; i < m_lastBit + 1; i++)
        {
            result += Get(i) + '0';
            if (i % CHAR_BIT == CHAR_BIT - 1)
            {
                result += ' ';
            }
        }
    
        return result;

    }

private:
    size_t m_bitarray[(((long)N -1) / WORD_BITS) + 1];
    size_t m_lastBit;
    void ValidateIndex(size_t index_) const throw(std::out_of_range);

    class BitArrayProxy
    {
    public:
        explicit BitArrayProxy(BitArray& array_, size_t index_) throw();

        BitArrayProxy &operator=(const BitArrayProxy& other_) throw();
        BitArrayProxy &operator=(bool _val) throw();
        operator bool() const throw();

    private:
        BitArray& m_array;
        size_t m_index;
        int GetBit()const;
        void SetBit(size_t bit);
    };
};



template <size_t N>
BitArray<N>::BitArray() throw():m_lastBit(N - 1)
{
    std::memset(m_bitarray, 0, sizeof(m_bitarray));
}

template <size_t N>
void BitArray<N>::ValidateIndex(size_t index_) const throw(std::out_of_range)
{
    if (index_ > m_lastBit)
    {
        throw std::out_of_range("out of range");
    }

}

template <size_t N>
typename BitArray<N>::BitArrayProxy BitArray<N>::operator[](size_t index_) throw(std::out_of_range)
{
    // if (index_ > m_lastBit)
    // {
    //     throw std::out_of_range;
    // }

    ValidateIndex(index_);

    return BitArrayProxy(*this, index_);
}

template <size_t N>
int BitArray<N>::BitArrayProxy::GetBit() const
{
    size_t internalArrayIndex = m_index / WORD_BITS;
    size_t chunk = m_array.m_bitarray[internalArrayIndex];
    size_t bitOffset = m_index % WORD_BITS;

    return ((chunk >> bitOffset) & (size_t)1);
}

template <size_t N>
void BitArray<N>::BitArrayProxy::SetBit(size_t bit)
{
    size_t internalArrayIndex = m_index / WORD_BITS;
    size_t bitOffset = m_index % WORD_BITS;
    bit = !!bit;
    m_array.m_bitarray[internalArrayIndex] &= ~(static_cast<size_t>(1) << bitOffset);
    m_array.m_bitarray[internalArrayIndex] |= (bit << bitOffset);
}

template <size_t N>
bool BitArray<N>::operator[](size_t index_) const throw(std::out_of_range)
{
    ValidateIndex(index_);

    BitArray<N> bit_arr_copy = *this;

    return BitArrayProxy(bit_arr_copy, index_);
}

template <std::size_t N>
BitArray<N>::BitArrayProxy::BitArrayProxy(BitArray<N>& array_, size_t index_) throw()
:m_array(array_), m_index(index_)
{

}

template <size_t N>
typename BitArray<N>::BitArrayProxy& BitArray<N>::BitArrayProxy::operator=(const BitArrayProxy& other_) throw()
{
    bool bit = other_;
    *this = bit;

    return *this;
}

template <size_t N>
typename BitArray<N>::BitArrayProxy& BitArray<N>::BitArrayProxy::operator=(bool bit) throw()
{

    SetBit(static_cast<size_t>(bit));

    return *this;
}

template <size_t N>
BitArray<N>::BitArrayProxy::operator bool() const throw()
{
    return 1 == GetBit();
}


static size_t MyPower(size_t num, int power)
{
    size_t result = 1;
    while (power > 0)
    {
        result *= num;
        --power;
    }

    return result;
}

} // namespace ilrd

#endif // __ILRD_RD110_BITARRAY_HPP__
