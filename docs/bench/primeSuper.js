startTime = new Date();
	
	  // 在这里编写你的业务逻辑

	  var j=1;
	  while(j<=100)
	  {
		  var i=3;
		  var sum=0;
		  while(i<=10000)
		  {
			  var isprime=1;
			  var b=2;
			  while(b*b<=i)
			  {
				  if(i%b==0)
				  {
					  isprime=0;
				  };
				  b=b+1;
			  };
			  if(isprime==1)
			  {
				  sum=sum+1;
			  };
			  i=i+1;
		  };
		  j=j+1;
	  };
	  console.log(sum);
	  console.log("done");

	  // 记录结束时间
endTime = new Date();
	
	  // 计算执行时间（以毫秒为单位）
executionTime = (endTime - startTime)/1000;
	  
	  // 输出执行时间
console.log(`代码执行时间：${executionTime} 秒`);
