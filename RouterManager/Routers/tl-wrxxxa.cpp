string HelloWord(string acc,string psw)
{
	int c=5,a=2,b=1;

	acc+="\r\n";
	
	for(a=1;a<3;a++)
	{
		b*=c;
		c++;
		acc+=" for "+b;
	}

	while(c>0)
	{
		c--;
		acc+=" while_"+c+" ";
	}

	do
	{
		acc += " do"+c%2+" ";
		c++;
	}while(c<6);
	
	if(c==6)
	{
		acc +=" if";
		return acc+"\n\n"+psw+"\n	2016-09-01";
	}
	else if(c==7)
	{
		return "else if";
	}
	else
	{
		return "else";
	}
}

