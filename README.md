let's start
===
备注:  
本项目作为共享项目，但本身已经停止更新与优化。  
请转向: https://github.com/Basicconstruction/BigIntegerUseVector  

以下是简要从`https://github.com/Basicconstruction/BigIntegerUseVector`
拷贝而来，作为简要介绍，有些操作已经不太一样，有些操作的结果可能会显得诡异。
这或许是因为add等函数设置为了返回BigInteger&.

声明一个对象并初始化
--
```cpp
构造函数声明
BigInteger();
explicit BigInteger(long long int);
explicit BigInteger(string);


BigInteger x;          x = 0;
BigInteger x(10)       x = 10;
BigInteger* p = new BigInteger(12);     p-> BigInteger(12);
前两种方法都会对对象进行初始化，默认初始化是 0

在初始化之后我们也可以使用
BigInteger d;     d = 0;
d = 1234;         d = 1234;

额外:
当数字足够大，比如大于 2 ^ 63-1 = 9223372036854775807
你需要使用字符串进行初始化，因为long long int 已经不足以囊括他。
使用字符串初始化地方式和数字一样，也可以通过字符串赋值。
``` 
基本操作
--
```cpp
使用BigInteger时，就像和使用 int 一样，支持+-*/%
示例:
BigInteger x(100);
x = x + 1;
x = x - 1;
x = x * 100;
x = x / 100;
x += 1;
x -= 1;
x *= 100;
x /= 100;
x = x % 3;
x %= 2;
x++;
x--;
甚至还可以直接使用原始方法(重载调用地方法)
+  ->   add
-  ->   sub
*  ->   mul
/  ->   div
%  ->   mod
示例:
    x = x.add(1);
    ....
对于这些操作，你也可以作用于 字符串，而不仅仅是 可以被推导到 long long int的值。
例如:
BigInteger x("100");
x += "1234";
x *= "134";
.....
```


Performance Testing
==
test platform
--
CLion 2021.2  
Build \#CL-212.4746.93, built on July 27, 2021  
Runtime version: 11.0.11+9-b1504.13 amd64  
VM: OpenJDK 64-Bit Server VM by JetBrains s.r.o.  
Windows 10 10.0  
GC: G1 Young Generation, G1 Old Generation  
Memory: 2000M  
Cores: 16  
Registry: run.processes.with.pty=TRUE  
Non-Bundled Plugins: com.kite.intellij (1.9.4)  
gun mingw  
Amd Ryzen 7 4000  
microsoft window 11  

本项目的测试速度标识为 **string**,标识为vector的项目在文档开始的链接里面 
--

|aspect|string|python|vector|java|julia|
|:---: | :---:|:---:|:---:|:---:|:---:|
|factorial(10) | | | | | |
| 100 | 67ms|0|3ms|6ms|0.024215 s|
|1000|17930ms|0|955ms|21ms|0.070049 s|
|10000|>>10s|0.34375s|22996ms|516ms|0.546226 s|
|100000| |50.875s| |29551.0ms|  30.027473 s |
|pow(2 ** x)(10)| | | | | |
|100|12ms|0|5ms|0|0.023329|
|1000|730ms|0|21ms|0|0.026742|
|10000|8234ms / one|0|1337ms|16ms|0.110875|
|100000| |0.125s|11181ms|148ms|3.518582 s|
|1000000| |12.5625s|null|1504.0ms|155.85642 s(get one and mul 10)|
|fibo(10)| | | | | |
|100|10ms| |10ms|2ms/0ms|0.058215 s/0.023118 s|
|1000|277ms| |7ms|11ms/3ms|0.061844 s/ 0.026205 s|
|10000|26977ms| |300ms|62ms/41ms| 0.130s/0.084748|
|100000|null|null|null|1531.0ms|null/2.615493 s|  

***在计算Fibonacci序列时，对java和julia分别采用了两种算法来进行计算，大致可以说明赋值要快于从字典中查找值。***
```python
#计算pow(2,x)  python
import time
start = time.process_time()
for i in range(10):
    print(2**10000)
end = time.process_time()
print(end-start)
```

```python
#就算factorial  python
import time
start = time.process_time()
for i in range(10):
    res = 1
    for j in range(1,100000):
        res *= j
        print(res)
end = time.process_time()
print(end-start)
```

```julia
#julia
function factorial(i)
    res = BigInt(1)
    for j in collect(1:i)
        res *= j
    end
    return res
end
```
```julia
#julia
function pow2(i,n=2)
    res = BigInt(1)
    for j in collect(1:i)
        res *= n
    end
    return res
end
```
```julia
#julia
known = Dict(0=>BigInt(0), 1=>BigInt(1))
function fibonacci(n)
    if n ∈ keys(known)
        return known[n]
    end
    res = fibonacci(n-1) + fibonacci(n-2)
    known[n] = res
    res
end
#fun2
function fibonacci2(n)
    x = BigInt(1)
    y = BigInt(0)
    z = BigInt(1)
    for k in collect(2:n)
        x = y
        y = z
        z = x + y
    end
    z
end
```
```java
//java
import javax.xml.crypto.Data;
import java.math.BigInteger;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

public class BoostUp {
    public static BigInteger myfibo(HashMap<Integer,BigInteger> hm,int x){
        if(hm.containsKey(x)){
            return hm.get(x);
        }
        BigInteger res =  myfibo(hm,x-2).add(myfibo(hm,x-1));
        hm.put(x,res);
        return res;
    }
    public static BigInteger myFibo2(int p){
        BigInteger x = new BigInteger("1");
        BigInteger y = new BigInteger("0");
        BigInteger z = new BigInteger("1");
        for (int h = 2; h <= p; h++) {
            x = y;
            y = z;
            z = x.add(y);
        }
        return z;
    }
//    public static BigInteger help(int i)
    public static void myPow(int x){
        BigInteger v = new BigInteger("2");
        System.out.println(v.pow(x));
    }
    public static void myFactorial(int x){
        BigInteger v = new BigInteger("1");
        for(int i = 1;i<=x;i++){
            v = v.multiply(BigInteger.valueOf(i));
        }
        System.out.println(v);
    }
    public static void main(String[] args){
        double predata = new Date().getTime();
        for(int i = 0;i <10;i++){
//            myPow(1000);
            HashMap<Integer,BigInteger> map = new HashMap<>();
            map.put(0,new BigInteger("0"));
            map.put(1,new BigInteger("1"));
//            myFactorial(100000);
//            System.out.println(myfibo(map,100000));
            System.out.println(myFibo2(100000));
        }
        System.out.println((new Date().getTime()-predata)+"ms");
    }
}

```