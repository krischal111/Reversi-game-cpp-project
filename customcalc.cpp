#ifndef CUSTOMCALC
#define CUSTOMCALC


#define bitsize(x) (sizeof(x)*8)
#define rotateleft(x,n) ((x<<n) | ((unsigned(x))>>(bitsize(x) - n)))
#define rotateright(x,n) ((x<<(bitsize(x)-n)) | ((unsigned(x))>>n))

#define mask1(r) (1<<r)
#define mask0(r) (~(mask1(r)))

#define mask_0_last(num) ((-1)<<num)
#define mask_1_last(num) (~(mask_0_last(num)))

#define mask_1_(r, num) (mask_1_last(num) << r+1-num)
#define mask_0_(r, num) (~mask_1_(r, num))


#define bit(x,r) ((x>>r)&1UL)
#define bits(x,r,num) (((unsigned)x>>(r-num+1)) & (mask_1_last(num)))

#define setbit(x,r,value)  (x = (x & mask0(r)) | (!!value << r))
#define setbits(x,r,num,value) (x = (x & mask_0_(r,num)) | ((value & mask_1_last(num)) << (r+1-num)))

template <class T>
T modulo(T a, T b)
{
    T c = a%b;
    return (c<0)?c+b:c;
}

template <class T>
T& set_bit(T& x, int r, char value)
{
    (x = (x & mask0(r)) | (!!x << r));
    return x;
}

#ifdef _GLIBCXX_IOSTREAM
#ifndef BITPRINTING
#define BITPRINTING
    template <class T>
    void bitprint(T x)
    {
        T y = 0;
        for(int i = 0; i<bitsize(x); i++)
        {
            y<<=1;
            y+=x&1;
            x>>=1;
        }
        for(int i = 0; i<bitsize(x); i++)
        {
            std::cout << (y & 1);
            y >>= 1;
        }
    }
#endif
#endif

#endif // customcalc.cpp included

#ifndef MAIN
#define MAIN 1

#include <cassert>
#include <iostream>
using namespace std;

#ifndef BITPRINTING
#define BITPRINTING
    template <class T>
    void bitprint(T x)
    {
        T y = 0;
        for(int i = 0; i<bitsize(x); i++)
        {
            y<<=1;
            y+=x&1;
            x>>=1;
        }
        for(int i = 0; i<bitsize(x); i++)
        {
            std::cout << (y & 1);
            y >>= 1;
        }
    }
#endif

int main()
{
    int x = 4;

    assert(bitsize(char) == 8);
    assert(rotateleft(1,3) == 8);
    assert( mask0(2) == -5);
    assert( mask1(2) == 4);
    assert(mask_0_last(2) == -4);
    assert(mask_1_last(2) ==  3);

    assert(mask_0_(2,2) == -7);
    assert(mask_1_(2,2) == 6);
    assert(bit(9,3) == 1);
    assert(bit(9,2) == 0);
    assert(bits(9, 3, 2) == 2);
    assert(bits(9, 2, 3) == 1);
    assert(setbit(x,1,1) == 6);
    assert(setbit(x,2,0) == 2);
    assert(setbits(x,3,3,5) == 10);
    assert(setbits(x,3,3,2) == 4);

    std::cout << "All test successful"<<endl;
}


#endif