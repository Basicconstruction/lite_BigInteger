Performance Testing
CLion 2021.2
Build #CL-212.4746.93, built on July 27, 2021
Runtime version: 11.0.11+9-b1504.13 amd64
VM: OpenJDK 64-Bit Server VM by JetBrains s.r.o.
Windows 10 10.0
GC: G1 Young Generation, G1 Old Generation
Memory: 2000M
Cores: 16
Registry: run.processes.with.pty=TRUE
Non-Bundled Plugins: com.kite.intellij (1.9.4)

gun 
Amd Ryzen 7 4000
microsoft window 11

aspect  lite-BigInteger/release          python              java             julia
factorial(10)
100      67ms                            0
1000     17930ms                         0
10000    >>10s                           0.34375
100000
pow(2 ** x)(10)
100     12ms                             0
1000    730ms                            0
10000   8234ms / one                     0
1000000                                12.5625s
fibo(10)
100     10ms
1000    277ms
10000   26977ms






python{
    import time
    start = time.process_time()
    for i in range(10):
        print(2**10000)
    end = time.process_time()
    print(end-start)
}
python{
    import time
    start = time.process_time()
    for i in range(10):
        res = 1
        for j in range(1,100000):
            res *= j
        print(res)
    end = time.process_time()
    print(end-start)
}
