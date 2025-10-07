#include <iostream>
#include <vector>

class bigint
{
private:
    unsigned* _digits;
    int _sign;
    size_t _size;

public:
    bigint(unsigned* arr, size_t sz);

    bigint(std::vector<unsigned> arr);

    ~bigint();

    void inline ChangeSign(int &n);

    int& GetSign();

    int GetAbs(int n);

    bool IsNegative(int n) const;

    void CreateWithOneNumber(unsigned num);

    void Print();

    bool operator==(const bigint &obj) const;

    bool operator!=(const bigint &obj) const;

    bool operator<(const bigint &obj) const;

    bool operator>(const bigint &obj) const;

    bool operator<=(const bigint &obj) const;

    bool operator>=(const bigint &obj) const;

    bigint operator+(const bigint &obj) const;
};

bigint::bigint(unsigned* arr, size_t size)
{
    if(size == 1)
    {
        CreateWithOneNumber(arr[0]);
        return;
    }

    while(arr[size - 1] == 0) // удаление ведущих нулей
    {
        size--;
    }

    if(arr[size - 1] > INT_MAX)
    {
        _size = size;
        _sign = 0;
    }
    else
    {
        _size = size - 1;
        _sign = arr[_size];
    }

    _digits = new unsigned [size];

    for(int i = 0; i < _size; i++)
    {
        _digits[i] = arr[i];
    }
}

bigint::bigint(std::vector<unsigned> arr)
{
    int size = arr.size();
    if(size == 1)
    {
        CreateWithOneNumber(arr[0]);
        return;
    }

    while(arr[size - 1] == 0) // удаление ведущих нулей
    {
        size--;
    }

    if(arr[size - 1] > INT_MAX)
    {
        _size = size;
        _sign = 0;
    }
    else
    {
        _size = size - 1;
        _sign = arr[_size];
    }

    _digits = new unsigned [size];

    for(int i = 0; i < _size; i++)
    {
        _digits[i] = arr[i];
    }
}

bigint::~bigint()
{
    delete [] _digits;
}

void inline bigint::ChangeSign(int& n) 
{
    n = n ^ (1 << 31);
}

int& bigint::GetSign()
{
    return _sign;
}

int bigint::GetAbs(int n)
{
    if(IsNegative(n)) 
    {
        ChangeSign(n);
    }

    return n;
}

bool bigint::IsNegative(int n) const
{
    return (n & (1 << 31)) != 0;
}

void bigint::CreateWithOneNumber(unsigned num)
{
    if(num <= INT_MAX)
    {
        _sign = num;
        _digits = nullptr;
        _size = 0;
    }
    else
    {
        _sign = 0;
        _digits = new unsigned[1];
        _digits[0] = num;
        _size = 1;
    }
}

void bigint::Print()
{
    std::cout << "Sign: ";
    if(IsNegative(_sign)) std::cout << '-';
    std::cout << GetAbs(_sign) << ' ';

    std::cout << "Digigts: ";
    for(int i = _size - 1; i >= 0; i--)
    {
        std::cout << _digits[i] << ' ';
    }
    std::cout << std::endl;
}

bool bigint::operator==(const bigint &obj) const
{
    if (_size != obj._size || _sign != obj._sign) return false;
    for (size_t i = 0; i < _size; i++) 
    {
        if (_digits[i] != obj._digits[i]) return false;
    }
    return true;
}

bool bigint::operator!=(const bigint &obj) const
{
    return !(*this == obj);
}

bool bigint::operator<(const bigint &obj) const
{
    int left_sign = this->_sign;
    int right_sign = obj._sign;

    if(IsNegative(this->_sign) && !IsNegative(obj._sign)) return true;

    else if(!IsNegative(this->_sign) && IsNegative(obj._sign)) return false;

    else if(IsNegative(this->_sign) && IsNegative(obj._sign))
    {
        left_sign ^= (1 << 31);
        right_sign ^= (1 << 31);

        if(left_sign > right_sign) return true;
        else return false;
    }
    else
    {
        if(_size < obj._size) return true;
        else if(_size > obj._size) false;

        for(int i = 0; i < _size; i++)
        {
            if(_digits[i] > obj._digits[i]) return false;
        }

        return true;
    }
}

bool bigint::operator>(const bigint &obj) const
{
    return !(*this < obj);
}

bool bigint::operator<=(const bigint &obj) const
{
    return *this < obj || *this == obj;
}

bool bigint::operator>=(const bigint &obj) const
{
    return *this > obj || *this == obj;
}

bigint bigint::operator+(const bigint &obj) const
{
    std::vector<unsigned> result;
    long long sum;
    int carry = 0;
    int sz = _size < obj._size ? _size : obj._size;
    int i = 0;

    for(; i < sz; i++)
    {
        sum = _digits[i] + obj._digits[i] + carry;
        carry = 0;
        
        if(sum >= UINT_MAX)
        {
            carry = 1;
            sum -= UINT_MAX;
        }

        result.push_back(sum);
    }

    if(sz == _size && _size != obj._size)
    {
        for(;i < obj._size; i++) result.push_back(obj._digits[i]);
        result.push_back(_sign);
    }
    else if(sz == obj._size && _size != obj._size)
    {
        for(;i < _size; i++) result.push_back(_digits[i]);
        result.push_back(obj._sign);
    }

    sum = _sign + obj._sign + carry;
    carry = 0;
    if(sum >= UINT_MAX)
    {
        carry = 1;
        sum -= UINT_MAX;
    }
    result.push_back(sum);
    if(carry) result.push_back(carry);

    return bigint(result);
}

int main()
{
    std::vector<unsigned> a{0, 1, 2, 3, 0};
    bigint obj1(a);
    obj1.Print();

    unsigned b[5] = {1, 2, 3, 4, 5};
    bigint obj2(b, (size_t)5);
    obj2.Print();

    obj1 < obj2 ? std::cout << std::endl << "True" : std::cout << std::endl << "False";
    std::cout << std::endl;

    bigint obj3 = obj1 + obj2;

    obj3.Print();

    return 0;
}