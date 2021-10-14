#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>
#include <stdio.h>
#include <cstring>
#include <ctime>
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using namespace std;
using namespace std::string_literals;

bool same(const bool a, const bool b) {
    if ((a && b) || ((!a) && (!b))) {
        return true;
    } else {
        return false;
    }
}

size_t convertCharToInt(const char c) {
    assert(c >= 48 && c <= 57);
    return static_cast<size_t>(c) - 48;
}

char convertIntToChar(size_t c) {
    return static_cast<char>(c + 48);
}

long long int convertStringToLong(const string &s) {
    long long int x = 0;
    for (char i : s) {
        x *= 10;
        x += convertCharToInt(i);
    }
    return x;
}

template<typename T>
string convertNumToString(T x) {
    // transfer int number to string
    if (x == 0) {
        return "0";
    }
    string s;
    x *= 10;
    while (x >= 10) {
        x /= 10;
        s.insert(0, string(1, static_cast<char>(x % 10 + 48)));
    }
    return s;
}

bool convertStringToBool(const string &bo) {
    if (bo == "0") {
        return false;
    } else {
        return true;
    }
}

string convertBoolToString(bool bo) {
    if (bo) {
        return "true";
    } else {
        return "false";
    }
}

size_t nonZero(const string &getStr) {
    for (size_t i = 0; i < getStr.length(); ++i) {
        if (getStr[i] != '0') {
            return i;
        }
    }
    //获取整数字符串中第一个非零字符的位置，用于简化字符串存储
    //比如传入 0000123 那么该函数将会返回4，后续函数可以利用该数字来处理简化
    return getStr.length();
}

string simplifyBigIntegerValue(string BigIntegerValue) {
    string v = std::move(BigIntegerValue);
    //用于简化BigInteger.add(BigInteger&,BigInteger&)的输出
    //比如将-00001,简化为-1
    //将00001简化为1
    if (v[0] == '-' || v[0] == '+') {
        size_t bn0 = nonZero(string(v.begin() + 1, v.end()));
        if (bn0 + 1 == v.length()) {
            return "0";
        }
        string str = string(v.begin() + int(bn0) + 1, v.end());
        return str.insert(0, string(1, v[0]));
    } else {
        size_t bn0 = nonZero(v);
        if (bn0 == v.length()) {
            return "0";
        }
        return string(v.begin() + bn0, v.end());
    }
}

class BigInteger {
public:
    size_t effectiveLength;
    bool over0;
    string value;
    string deploy;

    explicit BigInteger(long long int initNum) {
        if (initNum >= 0) {
            over0 = true;
        } else {
            initNum = -initNum;
            over0 = false;
        }
        this->value = convertNumToString(initNum);
        this->effectiveLength = this->value.length();
        this->deploy = this->getDeploy();
    }

    explicit BigInteger(const string &initWithString) {
        string initHelper = initWithString;
        size_t posFix = 0;
        if (initWithString == "-0") {
            this->value = "0";
            this->over0 = true;
            this->deploy = this->getDeploy();
            this->effectiveLength = 1;
            return;
        }
        if (initHelper[0] == '-') {
            //-12344
            //no -000989
            this->over0 = false;
            posFix = 1;
        } else if (initHelper[0] == '0') {
            //08990
            //0000798
            //0000
            size_t nonZeroBit;
            nonZeroBit = nonZero(string(initHelper));
            this->over0 = true;
            if (nonZeroBit == initHelper.length()) {
                this->value = "0";
            } else {
                this->value = string(initHelper.begin() + nonZeroBit, initHelper.end());
            }
            this->effectiveLength = this->value.length();
            this->deploy = this->getDeploy();
            return;

        } else if (initHelper[0] == '+') {
            //+9009
            //no +000789
            this->over0 = true;
            posFix = 1;
        } else {
            //989080
            this->over0 = true;
        }
        this->value = string(initHelper.begin() + posFix, initHelper.end());
        this->effectiveLength = this->value.length();
        this->deploy = this->getDeploy();
    }

    BigInteger(const string &initWithDeploy, bool simplifyDeploy) {
        vector<string> x = getOrigin(initWithDeploy);
        this->over0 = convertStringToBool(string(x[0]));
        this->value = string(x[1]);
        this->effectiveLength = this->value.length();
        if (simplifyDeploy) {
            if (this->value == "0") {
                this->over0 = true;
            }
            this->deploy = getDeploy();
        } else {
            this->deploy = string(initWithDeploy);
        }
    }

    BigInteger(const BigInteger &bi) {
        //拷贝构造函数，不应再函数前面添加explicit，即使它只有一个参数
        //似乎可以看作构造函数，即从BigInteger构造BigInteger
        this->over0 = bi.over0;
        this->value = bi.value;
        this->effectiveLength = bi.effectiveLength;
        this->deploy = bi.deploy;
    }

    ~BigInteger() = default;

    string getDeploy() const {
        if (this->over0) {
            return "0" + this->value;
        }
        size_t length = this->effectiveLength;
        string deployTemp;
        size_t p;
        size_t push = 0;
        size_t endBit = 10 - convertCharToInt(string(this->value)[length - 1]);
        if (endBit == 10) {
            deployTemp = '0';
            push = 1;
        } else {
            deployTemp = convertIntToChar(endBit);
        }
        for (int iter = 2; iter <= length; ++iter) {
            p = length - iter;
            endBit = 9 - convertCharToInt(string(this->value)[p]) + push;
            if (endBit == 10) {
                deployTemp = '0' + deployTemp;
                push = 1;
            } else {
                deployTemp = convertIntToChar(endBit) + deployTemp;
                push = 0;
            }

        }
        return '9' + deployTemp;
    }

    static string getDeploy(string valueTemp, bool over0Temp) {
        //计算“10补数”
        if (over0Temp) {
            return "0" + valueTemp;
        }
        size_t length = valueTemp.length();
        string deployTemp;
        size_t p;
        size_t push = 0;
        size_t endBit = 10 - convertCharToInt(valueTemp[length - 1]);
        if (endBit == 10) {
            deployTemp = '0';
            push = 1;
        } else {
            deployTemp = convertIntToChar(endBit);
        }
        for (int iter = 2; iter <= length; ++iter) {
            p = length - iter;
            endBit = 9 - convertCharToInt(valueTemp[p]) + push;
            if (endBit == 10) {
                if (iter == length) {
                    //                    deploy =
                }
                deployTemp.insert(0, "0");
                push = 1;
            } else {
                deployTemp.insert(0, string(1, convertIntToChar(endBit)));
                push = 0;
            }

        }
        deployTemp.insert(0, "9");
        return deployTemp;
    }

    static vector<string> getOrigin(string deployed) {
        //将计算结果转化为分开的包含符号位和数字位的组合
        vector<string> x(2);
        size_t getBack = 1;
        if (deployed[0] == '0') {
            x[0] = "1";
            const string t = string(deployed.begin() + 1, deployed.end());
            x[1] = simplifyBigIntegerValue(t);
        } else {
            assert(deployed[0] == '9');
            //            cout<<"&&&!!!"<<deployed<<" "<<deployed[0]<<endl;
            x[0] = "0";//false
            string originTemp = getDeploy(string(deployed.begin() + 1, deployed.end()), false);
            const string t = string(originTemp.begin() + 1, originTemp.end());
            x[1] = simplifyBigIntegerValue(t);
        }
        return x;
    }

    void toString() {
        //用于对齐
        if (this->over0) {
            printf("symbol:BigInteger\nover0: %s\nvalue:  %s \ndeploy: %s\neffectLength: %d\n",
                   convertBoolToString(this->over0).c_str(), this->getValue().c_str(), this->deploy.c_str(),
                   this->effectiveLength);
        } else {
            printf("symbol:BigInteger\nover0: %s\nvalue: %s \ndeploy: %s\neffectLength: %d\n",
                   convertBoolToString(this->over0).c_str(), this->getValue().c_str(), this->deploy.c_str(),
                   this->effectiveLength);
        }
    }

    void getDetails() {
        this->toString();
    }

    BigInteger add(const BigInteger &b2) const {
        const BigInteger b1(this->deploy, true);
        size_t l1 = b1.effectiveLength;
        size_t l2 = b2.effectiveLength;
        string effects1, effects2;
        effects1 = b1.deploy;
        effects2 = b2.deploy;
        effects1 = string(effects1.begin() + 1, effects1.end());
        effects2 = string(effects2.begin() + 1, effects2.end());
        if (l1 >= l2) {
            if (b2.over0) {
                effects2 = (string(b2.deploy))[0] + string(l1 - l2, '0') + effects2;
            } else {
                effects2 = (string(b2.deploy))[0] + string(l1 - l2, '9') + effects2;
            }
            effects1 = (string(b1.deploy))[0] + effects1;
        } else {
            if (b1.over0) {
                effects1 = (string(b1.deploy))[0] + string(l2 - l1, '0') + effects1;
            } else {
                effects1 = (string(b1.deploy))[0] + string(l2 - l1, '9') + effects1;
            }
            effects2 = (string(b2.deploy))[0] + effects2;
        }
        size_t push = 0;
        size_t temp;
        size_t n1;
        size_t n2;
        string deployed;
        size_t ll = effects1.length();
        assert(ll == effects1.length());
        for (size_t j = 0; j < ll; j++) {
            n1 = convertCharToInt(effects1[ll - j - 1]);
            n2 = convertCharToInt(effects2[ll - j - 1]);
            temp = n1 + n2 + push;
            push = 0;
            if (temp >= 10) {
                push = 1;
            }
            temp = temp % 10;
            deployed.insert(0, string(1, convertIntToChar(temp)));
        }
        if (b1.over0 && b2.over0 && deployed[0] == '1') {
            deployed.insert(0, "0");
        }
        vector<string> x = getOrigin(deployed);
        return BigInteger(simplifyBigIntegerValue(convertStringToBool(string(x[0])) ? x[1] : x[1].insert(0, "-")));
    }

    BigInteger add(const string &b2) const {
        BigInteger b1 = BigInteger(this->deploy);
        BigInteger bx = BigInteger(b2, true);
        return b1.add(bx);
    }

    BigInteger add(const long long int b2) const {
        BigInteger b1 = BigInteger(this->deploy);
        BigInteger bx = BigInteger(b2);
        return b1.add(bx);
    }

    BigInteger operator+() const {
        return *this;
    }

    BigInteger operator-() const {
        return BigInteger(this->over0 ? '-' + this->value : this->value);
    }

    BigInteger &operator=(const BigInteger &b2) = default;//simplify by clion
    BigInteger sub(const BigInteger &b2) const {
        return this->add(-b2);
    }

    BigInteger sub(const string &b2) const {
        return this->add(-BigInteger(b2));
    }

    BigInteger sub(long long int b2) const {
        return this->add(-BigInteger(b2));
    }

    BigInteger operator+(const BigInteger &b2) const {
        return this->add(b2);
    }

    BigInteger operator+(const string &b2) const {
        return this->add(BigInteger(b2));
    }

    BigInteger operator+(const long long int b2) const {
        return this->add(BigInteger(b2));
    }

    //pre
    BigInteger &operator++() {
        (*this) += 1;
        return *this;
    }

    //post
    BigInteger operator++(int dummy) {
        const BigInteger x = BigInteger(*this);
        (*this) += 1;
        return x;
    }

    //pre
    BigInteger &operator--() {
        (*this) -= 1;
        return *this;
    }

    //post
    BigInteger operator--(int dummy) {
        BigInteger x = BigInteger(*this);
        (*this) -= 1;
        return x;
    }

    BigInteger operator-(const BigInteger &b2) const {
        return this->sub(b2);
    }

    BigInteger operator-(const string &b2) const {
        return this->sub(b2);
    }

    BigInteger operator-(const long long int b2) const {
        return this->sub(b2);
    }

    char operator[](size_t index) const {
        assert(index + 1 <= this->value.length());
        return this->value[index];
    }

    BigInteger &operator+=(const BigInteger &b2) {
        *this = this->add(b2);
        return *this;
    }

    BigInteger &operator+=(const string &b2) {
        *this = this->add(b2);
        return *this;
    }

    BigInteger &operator+=(const long long int b2) {
        *this = this->add(b2);
        return *this;
    }

    BigInteger &operator-=(const BigInteger &b2) {
        *this = this->sub(b2);
        return *this;
    }

    BigInteger &operator-=(const string &b2) {
        *this = this->sub(b2);
        return *this;
    }

    BigInteger &operator-=(const long long int b2) {
        *this = this->sub(b2);
        return *this;
    }

    int compareTo(const BigInteger &b2) {
        //注意要比较的是有效值长度
        int y = 2;
        if (this->over0 && (!b2.over0)) {
            return 1;
        }
        if ((!this->over0) && b2.over0) {
            return -1;
        }
        if (this->value.length() > b2.value.length()) {
            y = 3;
        } else if (this->value.length() < b2.value.length()) {
            y = 1;
        } else {
            for (unsigned int c = 0; c < this->value.length(); c++) {
                if (this->value[c] > b2.value[c]) {
                    y = 3;
                    break;
                } else if (this->value[c] < b2.value[c]) {
                    y = 1;
                    break;
                } else {
                    continue;
                }
            }
        }
        if (y == 2) {
            return 0;
        }
        if (this->over0) {
            if (y == 3) {
                return 1;
            } else {
                return -1;
            }
        } else {
            if (y == 3) {
                return -1;
            } else {
                return 1;
            }
        }
    }

    int compareTo(const string &b2) {
        const BigInteger t(b2);
        return this->compareTo(t);
    }

    int compareTo(const long long int b2) {
        const BigInteger t(b2);
        return this->compareTo(t);
    }

    //-- ++ equals()
    bool equals(const BigInteger &b2) {
        if (this->compareTo(b2) == 0) {
            return true;
        } else {
            return false;
        }
    }

    bool equals(const string &b2) {
        if (this->compareTo(b2) == 0) {
            return true;
        } else {
            return false;
        }
    }

    bool equals(const long long int b2) {
        if (this->compareTo(b2) == 0) {
            return true;
        } else {
            return false;
        }
    }

    bool operator<(const BigInteger &b) {
        return this->compareTo(b) < 0;
    }

    bool operator>(const BigInteger &b) {
        return this->compareTo(b) > 0;
    }

    bool operator<=(const BigInteger &b) {
        return this->compareTo(b) <= 0;
    }

    bool operator>=(const BigInteger &b) {
        return this->compareTo(b) >= 0;
    }

    bool operator==(const BigInteger &b) {
        return this->equals(b);
    }

    bool operator!=(const BigInteger &b) {
        return !this->equals(b);
    }

    BigInteger &operator<<(const long long int x) {
        assert(x >= 0);
        BigInteger t(this->over0 ? this->value + string(x, '0') : '-' + this->value + string(x, '0'));
        this->value = t.value;
        this->deploy = t.deploy;
        this->effectiveLength = t.effectiveLength;
        return *this;
    }

    BigInteger operator<<(const string &y) const {
        //这个并非是为冗余重载设计，而是用于提供一个不改变原对象（返回新对象）的方法
        long long int x = convertStringToLong(y);
        assert(x >= 0);
        BigInteger t(this->over0 ? this->value + string(x, '0') : '-' + this->value + string(x, '0'));
        return t;
    }

    BigInteger &operator>>(const long long int x) {
        assert(x >= 0);
        if (x >= this->effectiveLength) {
            this->value = "0";
            this->deploy = "00";
            this->effectiveLength = 1;
            return *this;
        } else {
            BigInteger t(this->over0 ? string((this->value).begin(), (this->value).end() - x) : '-' +
                                                                                                string((this->value).begin(),
                                                                                                       (this->value).end() -
                                                                                                       x));
            this->value = t.value;
            this->deploy = t.deploy;
            this->effectiveLength = t.effectiveLength;
            return *this;
        }
    }

    BigInteger operator>>(const string &b) {
        //这个并非是为冗余重载设计，而是用于提供一个不改变原对象（返回新对象）的方法
        long long int x = convertStringToLong(b);
        assert(x >= 0);
        if (x >= this->effectiveLength) {
            return BigInteger(0);
        } else {
            BigInteger t(this->over0 ? string((this->value).begin(), (this->value).end() - x) : '-' +
                                                                                                string((this->value).begin(),
                                                                                                       (this->value).end() -
                                                                                                       x));
            return t;
        }
    }

    //    BigInteger operator<<(const BigInteger& x) {
    //        //自举左移重载，但实际用途不大
    //        assert(x.over0);
    //       较好的实现这个函数并不简单，况且考虑到处理器的性能与思维方式广阔度决定暂时不实现该方法
    //    }
    string getValue() const {
        return over0 ? this->value : '-' + this->value;
    }

    BigInteger singleMul(const unsigned int b) const {
        string c;
        string b1 = this->value;
        size_t le = b1.length();
        size_t push = 0;
        size_t multi;
        for (size_t h = 0; h < le; h++) {
            multi = convertCharToInt(b1[le - h - 1]) * b + push;
            c = convertIntToChar(multi % 10) + c;
            push = multi / 10 % 10;
        }
        c = convertIntToChar(push) + c;
        return BigInteger(c);
    }

    BigInteger mul(const BigInteger &b2) {
        BigInteger c(0);
        BigInteger c2(*this);
        const string b3 = b2.value;
        for (size_t h = 0; h < b2.value.length(); ++h) {
            c = c + (c2 << (convertNumToString(h))).singleMul(convertCharToInt(b3[b2.value.length() - h - 1]));
        }
        if ((this->over0 && b2.over0) || ((!this->over0) && (!b2.over0))) {
            c.over0 = true;
        } else {
            c.over0 = false;
        }
        c.deploy = c.getDeploy();
        return c;
    }

    BigInteger mul(const string &b2) {
        return this->mul(BigInteger(b2));
    }

    BigInteger mul(const long long int b2) {
        return this->mul(BigInteger(b2));
    }

    BigInteger operator*(const BigInteger &b2) {
        return this->mul(b2);
    }

    BigInteger operator*(const string &b2) {
        return this->mul(b2);
    }

    BigInteger operator*(const long long int b2) {
        return this->mul(b2);
    }

    BigInteger &operator*=(const BigInteger &b2) {
        BigInteger x = this->mul(b2);
        this->over0 = x.over0;
        this->value = x.value;
        this->deploy = x.deploy;
        this->effectiveLength = x.effectiveLength;
        return *this;
    }

    BigInteger &operator*=(const string &b2) {
        BigInteger x = this->mul(b2);
        this->over0 = x.over0;
        this->value = x.value;
        this->deploy = x.deploy;
        this->effectiveLength = x.effectiveLength;
        return *this;
    }

    BigInteger &operator*=(const long long int b2) {
        BigInteger x = this->mul(b2);
        this->over0 = x.over0;
        this->value = x.value;
        this->deploy = x.deploy;
        this->effectiveLength = x.effectiveLength;
        return *this;
    }

    BigInteger &toAbs() {
        this->over0 = true;
        this->deploy = this->getDeploy();
        return *this;
    }

    static BigInteger abs(const BigInteger &b) {
        BigInteger x(b.value);
        return x;
    }

    static size_t divHelp(BigInteger &b1, BigInteger &b2) {
        for (size_t k = 2; k <= 9; ++k) {
            if (b1 == (b2 * k)) {
                return k;
            } else if (b1 < (b2 * k)) {
                return --k;
            } else {

            }
        }
        return 9;
    }

    BigInteger div(BigInteger b2) {
        if (b2.value == "1") {
            return *this;
        }
        bool b2_over0 = b2.over0;
        b2.toAbs();
        if (BigInteger::abs(*this) < b2) {
            return BigInteger(0);
        }
        BigInteger dh = BigInteger::abs(*this);
        size_t l1 = this->value.length();
        //1000
        //div 35
        size_t p = 0;
        string get;
        for (; p < l1;) {
            if (BigInteger(string(dh.value.begin(), dh.value.begin() + 1 + p)) < b2) {
                p++;
            } else {
                break;
            }
        }
        auto *t = new BigInteger(string(dh.value.begin(), dh.value.begin() + 1 + p));
        size_t newp = p;
        size_t cal;
        while (true) {
            if (*t < b2) {
                get.append("0");
            } else {
                cal = divHelp(*t, b2);
                get.append(convertNumToString(cal));
            }
            dh -= ((b2 * cal) << (l1 - 1 - newp));
            if (get.length() == l1 - p) {
                break;
            }
            newp++;
            *t = ((*t - (b2 * cal)) << 1) + convertCharToInt((*this)[newp]);
            cal = 0;
        }
        assert(get.length() == l1 - p);//默认查错
        get = same(this->over0, b2_over0) ? get : '-' + get;
        return BigInteger(get);
    }

    BigInteger operator%(BigInteger b2) {
        //b2 > 0
        b2.toAbs();
        if (BigInteger::abs(*this) < b2) {
            return BigInteger(0);
        }
        BigInteger dh = BigInteger::abs(*this);
        size_t l1 = this->value.length();
        size_t l2 = b2.value.length();
        //1000
        //div 35
        size_t p = 0;
        size_t get = 0;
        for (; p < l1;) {
            if (BigInteger(string(dh.value.begin(), dh.value.begin() + 1 + p)) < b2) {
                p++;
            } else {
                break;
            }
        }
        BigInteger *t = new BigInteger(string(dh.value.begin(), dh.value.begin() + 1 + p));
        size_t newp = p;
        size_t cal;
        while (true) {
            if (*t < b2) {
            } else {
                cal = divHelp(*t, b2);
            }
            dh -= ((b2 * cal) << (l1 - newp - 1));
            get += 1;
            if (get == l1 - p) {
                break;
            }
            newp++;
            *t = ((*t - (b2 * cal)) << 1) + convertCharToInt((*this)[newp]);
            cal = 0;
        }
        assert(get == l1 - p);//默认查错
        return (*this).over0 ? dh : -dh;
    }

    BigInteger operator%(const string &b2) {
        return *this % BigInteger(b2);
    }

    BigInteger operator%(const long long int b2) {
        return *this % BigInteger(b2);
    }

    BigInteger &operator%=(const BigInteger &b2) {
        *this = (*this) % b2;
        return (*this);
    }

    BigInteger &operator%=(const string &b2) {
        *this = (*this) % b2;
        return (*this);
    }

    BigInteger &operator%=(const long long int b2) {
        *this = (*this) % b2;
        return (*this);
    }

    BigInteger div(const string &b2) {
        BigInteger b(b2);
        return this->div(b);
    }

    BigInteger div(const long long int b2) {
        BigInteger b(b2);
        return this->div(b);
    }

    BigInteger operator/(const BigInteger &b2) {
        return this->div(b2);
    }

    BigInteger operator/(const string &b2) {
        BigInteger b(b2);
        return this->div(b);
    }

    BigInteger operator/(const long long int b2) {
        BigInteger b(b2);
        return this->div(b);
    }

    BigInteger &operator/=(const BigInteger &b) {
        *this = this->div(b);
        return *this;
    }

    BigInteger &operator/=(const string &b) {
        *this = this->div(b);
        return *this;
    }

    BigInteger &operator/=(const long long int b) {
        *this = this->div(b);
        return *this;
    }

    BigInteger sqrt() {
        BigInteger res(1);
        bool t = true;
        while (t) {
            res = (res + (*this) / res) / 2;
            t = (res * res <= (*this) && (res + 1) * (res + 1) > (*this)) ||
                (res * res >= (*this) && (res - 1) * (res - 1) < (*this));
            t = !t;
        }
        return (res * res <= (*this) && (res + 1) * (res + 1) > (*this)) ? res : res - 1;
    }

    BigInteger pow(unsigned int x) {
        assert(x > 0);
        BigInteger tmp = BigInteger(*this);
        BigInteger res(1);
        for (int i = 1; i < x; i++) {
            res *= tmp;
        }
        return res;
    }

    static BigInteger pow(int x, int y) {
        BigInteger tmp(x);
        BigInteger res(1);
        for (int i = 1; i <= y; i++) {
            res *= tmp;
        }
        return res;
    }

    static BigInteger pow(const string &x, int y) {
        BigInteger tmp(x);
        BigInteger res(1);
        for (int i = 1; i <= y; i++) {
            res *= tmp;
        }
        return res;
    }

    static BigInteger factorial(int i) {
        if (i == 1 || i == 0) {
            return BigInteger(1);
        } else {
            BigInteger tmp(1);
            for (int j = 1; j < i; j++) {
                tmp *= j;
            }
            return tmp;
        }

    }


};

string getFibo(long long int p) {
    if (p == 0) {
        return "0";
    } else if (p == 1) {
        return "1";
    } else {
        BigInteger x(1);
        BigInteger y(0);
        BigInteger z(1);
        for (int h = 2; h <= p; h++) {
            x = y;
            y = z;
            z = x + y;
        }
        return z.value;
    }
}

int main() {
//    high_resolution_clock::time_point beginTime = high_resolution_clock::now();
//    for(int i = 0;i < 10;i++){
////        cout<<BigInteger::factorial(1000).value;
//        cout<<getFibo(10000)<<endl;
//    }
//    high_resolution_clock::time_point endTime = high_resolution_clock::now();
//    milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(endTime - beginTime);
//    cout << "\nRunning Time " << timeInterval.count()  << "ms" << endl;
    high_resolution_clock::time_point beginTime = high_resolution_clock::now();
    clock_t tick = clock();
//    cout<<BigInteger::pow(2,1000).value<<endl;
    int j = 688;
    for (int i = 1000000; i < 1010000; i++) {
        cout << (BigInteger(i) % j).value << endl;
    }
//    std::cout<<BigInteger::factorial(1000).value<<std::endl;
//    high_resolution_clock::time_point endTime = high_resolution_clock::now();
//    milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(endTime - beginTime);
//    std::cout<<timeInterval.count()<<std::endl;
    std::cout << clock() - tick << "ms" << std::endl;
    return 0;
}
