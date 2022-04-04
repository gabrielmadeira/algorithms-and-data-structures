#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>

using namespace std;

const int max_digits = 10000;

struct bignum {
        bool neg;
        char d[max_digits];
        int size;

        bignum() : neg(false), size(0){ d[0] = 0; }

        bignum(const bignum& x, bool sig) : neg(sig), size(x.size)
        {
                copy(x.d, x.d+x.size, d);
        }
        
        bignum(long long x) : size(0)
        {
                neg = x<0;
                if (neg) x = -x;
                while (x) {
                        d[size++] = x % 10;
                        x /= 10;
                }
        }
        
        bignum(const string& s) : neg(false), size(0)
        {
                const char *p = s.c_str();
                if (*p == '-') {
                        neg = true;
                        ++p;
                }
                while (*p)
                        d[size++] = *p++ - '0';
                reverse(d, d+size);
                clean();
        }
        
        bignum& clean()
        {
                int i;
                for (i=size; i>0; --i)
                        if (d[i-1])
                                break;
                resize(i);
                if (!size)
                        neg = false; // zero n„o È negativo
                return *this;
        }
        
        bool operator!() const
        {       
                return !size;         
        }
        operator bool() const 
        {
                return size;
        }

        void resize(int n)
        {
                if (n>size)
                        for (int i=size; i<n; ++i)
                                d[i] = 0;
                size = n;
        }

        char operator[](int i) const
        {
                return d[i];
        }

        char& operator[](int i)
        {
                return d[i];
        }
        
        bignum operator-() const
        {
                return bignum(*this, !neg).clean();
        }
        
        bignum& operator<<=(int i)
        {
                copy_backward(d, d+size, d+size+1);
                d[0] = 0;
                size++;
                return *this;
        }
};

ostream& operator<<(ostream& o, const bignum& x)
{
        if (!x)
                return o << 0;
        if (x.neg)
                o << '-';
        copy(reverse_iterator<const char*>(x.d + x.size), 
             reverse_iterator<const char*>(x.d), ostream_iterator<int>(o));
        return o;
}

bignum& add(bignum& x, const bignum& y)
{
        int tam = max(x.size, y.size);
        x.resize(tam);

        int carry = 0;
        for (int i=0; i<x.size; ++i) {
                int s = x[i] + (i<y.size ? y[i] : 0) + carry;
                x[i] = s % 10;
                carry = s > 9;
        }
        if (carry)
                x[x.size++] = 1;
        return x;
}


bool operator<(const bignum& x, const bignum& y)
{
	   if (x.neg != y.neg)
                return x.neg;
                
        if (x.size != y.size) // magnitudes diferentes        
                return x.neg == (x.size > y.size);

        for (int i=x.size-1; i>=0; --i)
                if (x[i] != y[i])
                        return x.neg != ( x[i]<y[i] );
        return false;
}

bool operator==(const bignum& x, const bignum& y)
{
        return x.neg==y.neg && 
                x.size == y.size &&
                equal(x.d, x.d+x.size, y.d);
}


// necess·rio sÛ pra valladolid
bool operator!=(const bignum& x, const bignum& y) { return !(x == y); }
bool operator>(const bignum& x, const bignum& y) { return y < x; }
bool operator<=(const bignum& x, const bignum& y) { return !(y < x); }
bool operator>=(const bignum& x, const bignum& y) { return !(x < y); }

/*
  #include <utility>
  using namespace std::rel_ops;
*/


// x -= y, assume que tem sinais iguais
bignum& sub(bignum& x, const bignum& y)
{
        bignum z;
        bool xmaior = x>y;	

        if (xmaior)
                z = y;
        else {
                z = x;
                x = y;
        }

        int borrow = 0;
        for (int i=0; i<x.size; ++i) {
                int s = x[i] - borrow - (i<z.size ? z[i] : 0);
                x[i] = (s + 20) % 10;
                borrow = s < 0;
        }
        //if (!xmaior)
            //    x.neg = !x.neg;

        return x;
}


bignum&
operator+=(bignum& x, const bignum& y)
{
        if (!y) return x;       // y È zero
        if (!x) return x = y;
        
      //  if (x.neg != y.neg)     // sinais diferentes
               // return sub(x, y).clean();

        return add(x, y).clean();
}



bignum&
operator-=(bignum& x, const bignum& y)
{
        if (!y) return x;
        if (!x) return x = -y;
        
        //if (x.neg != y.neg)     // sinais diferentes
              //  return add(x, y).clean();
        
        return sub(x, y).clean();
}







bignum& operator*=(bignum& x, const bignum& y)
{
        bignum r;
        for (int i=0; i<y.size; ++i) {
                for (int j=0; j<y[i]; ++j)
                        add(r, x);
                x <<= 1;
        }

        //r.neg = x.neg != y.neg;
        x = r;
        return x.clean();
}




bignum& operator/=(bignum& x, const bignum& y)
{
        bignum temp;
        temp.resize(x.size);

        //temp.neg = x.neg != y.neg;

        bignum row;
        bignum ty = y;
       // ty.neg = false;

        for (int i=x.size-1; i>=0; --i) {
                row <<= 1;
                row[0] = x[i];
                row.clean();
                while (ty <= row) {
                        ++temp[i];
                        sub(row, ty).clean();
                }
        }
        return (x = temp).clean();
}


bignum operator+(const bignum& a, const bignum& b)
{
        bignum c = a;
        return c += b;
}

bignum operator-(const bignum& a, const bignum& b)
{
        bignum c = a;
        return c -= b;
}

bignum operator*(const bignum& a, const bignum& b)
{
        bignum c = a;
        return c *= b;
}

bignum operator/(const bignum& a, const bignum& b)
{
        bignum c = a;
        return c /= b;
}

