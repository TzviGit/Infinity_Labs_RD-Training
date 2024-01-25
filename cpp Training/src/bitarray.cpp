

#include <iostream> /* cout */

#include "bitarray.hpp"

#define _RED_    "\033[1;31m"
#define _GREEN_  "\033[1;32m"
#define _YELLOW_ "\033[1;33m"
#define _PURPLE_ "\033[1;35m"
#define _CYAN_   "\033[1;36m"
#define _WHITE_  "\033[0m"


using namespace ilrd;


static void TestBitArray();

static void TestBitArrayAssignment();
static void TestBitArrayAccessors();
static void TestBitArrayComparison();
static void TestBitArrayBitwise();
static void TestBitArrayFlip();
static void TestBitArrayCount();
static void TestBitArrayToString();

template <std::size_t N>
static void SetOddBits(BitArray<N>& bitarr);
template <std::size_t N>
static void SetEvenBits(BitArray<N>& bitarr);

static void PrintResults(bool result, bool expect, const char *title);
static void PrintResults(size_t result, size_t expect, const char *title);
template <std::size_t N>
static void PrintResults(const BitArray<N>& bitarr);


void Foo(const BitArray<3>& b)
{ if (b[1]) { std::cout << "Foo\n"; } }

void Bar(BitArray<3> b)
{ if (b[1]) { std::cout << "Bar\n"; } }

static void Practice1()
{
    BitArray<3> b;
	
	Foo(b);
	Bar(b);
}



int main()
{
	TestBitArray();
	
	std::cout << std::endl;
	return 0;
}


static void TestBitArray()
{
//    Practice1();
    
    TestBitArrayAssignment();
    TestBitArrayAccessors();
    TestBitArrayComparison();
    TestBitArrayBitwise();
    TestBitArrayFlip();
    TestBitArrayCount();
    TestBitArrayToString();
}



static void TestBitArrayAssignment()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - BitArray Assignment" << _WHITE_ << std::endl;
    
    BitArray<25> bitarr71_odd;
    BitArray<64> bitarr64_odd;
    
    PrintResults(bitarr71_odd[0], false, "init BitArray<25>[1]");
    PrintResults(bitarr64_odd[1], false, "init BitArray<64>[2]");
    
    bitarr71_odd[0] = true;
    PrintResults(bitarr71_odd[0], true, "bool assignment [0]");
    
    bitarr64_odd[1] = bitarr71_odd[0];
    PrintResults(bitarr64_odd[1], true, "bitArray assignment");
    
    (bitarr71_odd[0]) ? PrintResults(true, true, "if()") :
        PrintResults(false, true, "if()");
    (bitarr71_odd[1]) ? PrintResults(false, true, "if()") :
        PrintResults(true, true, "if()");
    
    bitarr64_odd[1] = bitarr64_odd[1];
    PrintResults(bitarr64_odd[1], true, "assignment to self");
    
    bitarr64_odd[63] = true;
    PrintResults(bitarr64_odd[63], true, "bool assignment [63]");
    
    for (size_t i = 0; i < 64; i++)
    {
        bitarr64_odd[i] = true;
    }
    
    bool val = true;
    for (size_t i = 0; i < 64; i++)
    {
        val &= bitarr64_odd[i];
    }
    
    PrintResults(val, true, "loop assignment");
}


static void TestBitArrayAccessors()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - BitArray Accessors" << _WHITE_ << std::endl;
    
    BitArray<25> bitarr71_odd;
    BitArray<71> bitarr64_odd;
    
    bitarr71_odd.Set(0, true);
    PrintResults(bitarr71_odd[0], true, "Set(0, true)");
    PrintResults(bitarr71_odd.Get(0), true, "Get(0)");
    PrintResults(bitarr71_odd.Get(1), false, "Get(1)");
    
    bitarr64_odd.Set(70, bitarr71_odd.Get(0));
    PrintResults(bitarr64_odd.Get(70), true, "A.Set(B.Get())");
    
    bitarr64_odd.Set(true);
    bool val = true;
    for (size_t i = 0; i < 71; i++)
    {
        val &= bitarr64_odd.Get(i);
    }
    
    PrintResults(val, true, "Set all <71>");
}


static void TestBitArrayComparison()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - BitArray Comparison" << _WHITE_ << std::endl;
    
    BitArray<64> bitarr64_odd;
    BitArray<64> bitarr64_even;
    
    SetOddBits(bitarr64_odd);
    SetEvenBits(bitarr64_even);
    
    PrintResults(bitarr64_odd == bitarr64_even, false, "b1 == b2 <64>");
    PrintResults(bitarr64_odd != bitarr64_even, true, "b1 != b2 <64>");
    PrintResults(bitarr64_odd == bitarr64_odd, true, "b1 == b1 <64>");
    PrintResults(bitarr64_odd != bitarr64_odd, false, "b1 != b1 <64>");
    
    BitArray<71> bitarr71_odd;
    BitArray<71> bitarr71_even;
    
    SetOddBits(bitarr71_odd);
    SetOddBits(bitarr71_even);
    
    PrintResults(bitarr71_odd == bitarr71_even, true, "b1 == b2 <71>");
    PrintResults(bitarr71_odd != bitarr71_even, false, "b1 != b2 <71>");
}


static void TestBitArrayBitwise()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - BitArray Bitwise" << _WHITE_ << std::endl;
    
    const BitArray<64> bitarr64_zero;
    BitArray<64> bitarr64_odd, bitarr64_even, bitarr64_all;
    BitArray<64> bitarr64;
    
    SetOddBits(bitarr64_odd);   // 0101
    SetEvenBits(bitarr64_even); // 1010
    bitarr64_all.Set(true);     // 1111
    
    bitarr64 &= bitarr64_odd;   // 0000
    PrintResults(bitarr64 == bitarr64_zero, true, "b1 &= b2 <64>");
    bitarr64 |= bitarr64_odd;   // 0101
    PrintResults(bitarr64 == bitarr64_odd, true, "b1 |= b2 <64>");
    bitarr64 ^= bitarr64_odd;   // 0000
    PrintResults(bitarr64 == bitarr64_zero, true, "b1 ^= b2 <64>");
    bitarr64 ^= bitarr64_odd;   // 0101
    PrintResults(bitarr64 == bitarr64_odd, true, "b1 ^= b2 <64>");
    bitarr64 |= bitarr64_even;  // 1111
    PrintResults(bitarr64 == bitarr64_all, true, "b1 |= b2 <64>");
    
    bitarr64 &= bitarr64;       // 1111
    PrintResults(bitarr64 == bitarr64_all, true, "b1 &= b1 <64>");
    bitarr64 ^= bitarr64;       // 0000
    PrintResults(bitarr64 == bitarr64_zero, true, "b1 ^= b1 <64>");
    bitarr64 |= bitarr64;       // 0000
    PrintResults(bitarr64 == bitarr64_zero, true, "b1 |= b1 <64>");
    
    const BitArray<71> bitarr71_zero;
    BitArray<71> bitarr71_odd, bitarr71_even, bitarr71_all;
    BitArray<71> bitarr71;
    
    SetOddBits(bitarr71_odd);   // 0101
    SetEvenBits(bitarr71_even); // 1010
    bitarr71_all.Set(true);     // 1111
    
    bitarr71 &= bitarr71_odd;   // 0000
    PrintResults(bitarr71 == bitarr71_zero, true, "b1 &= b2 <71>");
    bitarr71 |= bitarr71_odd;   // 0101
    PrintResults(bitarr71 == bitarr71_odd, true, "b1 |= b2 <71>");
    bitarr71 ^= bitarr71_odd;   // 0000
    PrintResults(bitarr71 == bitarr71_zero, true, "b1 ^= b2 <71>");
    bitarr71 |= bitarr71_odd;   // 0101
    PrintResults(bitarr71 == bitarr71_odd, true, "b1 |= b2 <71>");
    bitarr71 ^= bitarr71_even;  // 1111
    PrintResults(bitarr71 == bitarr71_all, true, "b1 ^= b2 <71>");
}


static void TestBitArrayFlip()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - BitArray Flip" << _WHITE_ << std::endl;
    
    const BitArray<64> bitarr64_zero;
    BitArray<64> bitarr64_odd, bitarr64_even, bitarr64_all;
    BitArray<64> bitarr64;
    
    SetOddBits(bitarr64_odd);   // 0101
    SetEvenBits(bitarr64_even); // 1010
    bitarr64_all.Set(true);     // 1111
    bitarr64.Set(true);         // 1111
    
    bitarr64.Flip();            // 0000
    PrintResults(bitarr64 == bitarr64_zero, true, "Flip(1111) <64>");
    bitarr64.Flip();            // 1111
    PrintResults(bitarr64 == bitarr64_all, true, "Flip(0000) <64>");
    bitarr64_odd.Flip();        // 1010
    PrintResults(bitarr64_odd == bitarr64_even, true, "Flip(0101) <64>");
    
    bitarr64.Flip(0);           // 1110
    PrintResults(bitarr64.Get(0), false, "Flip[0] (0) <64>");
    bitarr64.Flip(1);           // 1100
    PrintResults(bitarr64.Get(1), false, "Flip[1] (0) <64>");
    bitarr64.Flip(0);           // 1101
    PrintResults(bitarr64.Get(0), true, "Flip[0] (1) <64>");
    bitarr64.Flip(1);           // 1111
    PrintResults(bitarr64.Get(1), true, "Flip[1] (1) <64>");
    
    const BitArray<71> bitarr71_zero;
    BitArray<71> bitarr71_odd, bitarr71_even, bitarr71_all;
    BitArray<71> bitarr71;
    
    SetOddBits(bitarr71_odd);   // 0101
    SetEvenBits(bitarr71_even); // 1010
    bitarr71_all.Set(true);     // 1111
    
    bitarr71.Flip();            // 1111
    PrintResults(bitarr71 == bitarr71_all, true, "Flip(0000) <71>");
    bitarr71.Flip();            // 0000
    PrintResults(bitarr71 == bitarr71_zero, true, "Flip(1111) <71>");
    bitarr71_odd.Flip();        // 1010
    PrintResults(bitarr71_odd == bitarr71_even, true, "Flip(0101) <71>");
    bitarr71.Flip(70);          // 0111
    PrintResults(bitarr71.Get(70), true, "Flip[70] (0) <71>");
    bitarr71.Flip(70);          // 1111
    PrintResults(bitarr71.Get(70), false, "Flip[70] (1) <71>");
}


static void TestBitArrayCount()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - BitArray Count" << _WHITE_ << std::endl;
    
    const BitArray<25> bitarr25_zero;
    BitArray<25> bitarr25_odd, bitarr25_even, bitarr25_all;
    
    SetOddBits(bitarr25_odd);   // 0101
    SetEvenBits(bitarr25_even); // 1010
    bitarr25_all.Set(true);     // 1111
    
    PrintResults(bitarr25_zero.CountSetBits(), 0, "Count(0000) <25>");
    PrintResults(bitarr25_odd.CountSetBits(), 12, "Count(0101) <25>");
    PrintResults(bitarr25_even.CountSetBits(), 13, "Count(1010) <25>");
    PrintResults(bitarr25_all.CountSetBits(), 25, "Count(1111) <25>");
    
    const BitArray<64> bitarr64_zero;
    BitArray<64> bitarr64_odd, bitarr64_even, bitarr64_all;
    
    SetOddBits(bitarr64_odd);   // 0101
    SetEvenBits(bitarr64_even); // 1010
    bitarr64_all.Set(true);     // 1111
    
    PrintResults(bitarr64_zero.CountSetBits(), 0, "Count(0000) <64>");
    PrintResults(bitarr64_odd.CountSetBits(), 32, "Count(0101) <64>");
    PrintResults(bitarr64_even.CountSetBits(), 32, "Count(1010) <64>");
    PrintResults(bitarr64_all.CountSetBits(), 64, "Count(1111) <64>");
    
    const BitArray<71> bitarr71_zero;
    BitArray<71> bitarr71_odd, bitarr71_even, bitarr71_all;
    
    SetOddBits(bitarr71_odd);   // 0101
    SetEvenBits(bitarr71_even); // 1010
    bitarr71_all.Set(true);     // 1111
    
    PrintResults(bitarr71_zero.CountSetBits(), 0, "Count(0000) <71>");
    PrintResults(bitarr71_odd.CountSetBits(), 35, "Count(0101) <71>");
    PrintResults(bitarr71_even.CountSetBits(), 36, "Count(1010) <71>");
    PrintResults(bitarr71_all.CountSetBits(), 71, "Count(1111) <71>");
}


static void TestBitArrayToString()
{
    std::cout << std::endl << _PURPLE_ <<
        "Test - BitArray ToString" << _WHITE_ << std::endl;
    
    const BitArray<25> bitarr25_zero;
    BitArray<25> bitarr25_odd, bitarr25_even, bitarr25_all;
    
    SetOddBits(bitarr25_odd);   // 0101
    SetEvenBits(bitarr25_even); // 1010
    bitarr25_all.Set(true);     // 1111
    
    PrintResults(bitarr25_zero);
    PrintResults(bitarr25_odd);
    PrintResults(bitarr25_even);
    PrintResults(bitarr25_all);
    
    const BitArray<64> bitarr64_zero;
    BitArray<64> bitarr64_odd, bitarr64_even, bitarr64_all;
    
    SetOddBits(bitarr64_odd);   // 0101
    SetEvenBits(bitarr64_even); // 1010
    bitarr64_all.Set(true);     // 1111
    
    PrintResults(bitarr64_zero);
    PrintResults(bitarr64_odd);
    PrintResults(bitarr64_even);
    PrintResults(bitarr64_all);
    
    const BitArray<71> bitarr71_zero;
    BitArray<71> bitarr71_odd, bitarr71_even, bitarr71_all;
    
    SetOddBits(bitarr71_odd);   // 0101
    SetEvenBits(bitarr71_even); // 1010
    bitarr71_all.Set(true);     // 1111
    
    PrintResults(bitarr71_zero);
    PrintResults(bitarr71_odd);
    PrintResults(bitarr71_even);
    PrintResults(bitarr71_all);
}





// UTILS

template <std::size_t N>
static void SetOddBits(BitArray<N>& bitarr)
{
    for (size_t i = 0; i < N; i++)
    {
        (i % 2 == 0) ? bitarr.Set(i, 0) : bitarr.Set(i, 1);
    }
}

template <std::size_t N>
static void SetEvenBits(BitArray<N>& bitarr)
{
    for (size_t i = 0; i < N; i++)
    {
        (i % 2 != 1) ? bitarr.Set(i, 1) : bitarr.Set(i, 0);
    }
}


static void PrintResults(bool result, bool expect, const char *title)
{
    std::cout << ((result == expect) ? _GREEN_ : _RED_);
	std::cout << title << ": " << result << _WHITE_ << std::endl;
}


static void PrintResults(size_t result, size_t expect, const char *title)
{
    std::cout << ((result == expect) ? _GREEN_ : _RED_);
	std::cout << title << ": " << result << _WHITE_ << std::endl;
}


template <std::size_t N>
static void PrintResults(const BitArray<N>& bitarr)
{
	std::cout << _CYAN_ << N << ": " << _YELLOW_ <<
	    bitarr.ToString() << _WHITE_ << std::endl;
}




