import time
time_start = time.time()  # 记录开始时间


j=1
while(j<=100):

    i=3
    sum=0
    while(i<=3000):
    
        isprime=1
        b=2
        while(b*b<=i):
        
            if(i%b==0):
            
                isprime=0
            
            b=b+1
        
        if(isprime==1):
        
            sum=sum+i
        
        i=i+1
    
    j=j+1

print("done")


# function()   执行的程序
time_end = time.time()  # 记录结束时间
time_sum = time_end - time_start  # 计算的时间差为程序的执行时间，单位为秒/s
print(str(time_sum) + "s")