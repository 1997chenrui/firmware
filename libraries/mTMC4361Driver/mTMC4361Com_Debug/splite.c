#include <string.h>

int Synthesis(char d,char n)
{
    int result=1;
    for(char i=0; i<n;i++)
	{
		result=result*10;
	}
    return d*result;
}

int split(char *a,char s,char n)
{
	int result=0;
	char i=0;
	if(a[s] == '-') i=s+1;else i=s;
	for(;i<s+n;i++)
	{
		result+=Synthesis(a[i]&0x0f,s+n-i-1);
	}
	if(a[s] == '-') return -result;else return result;		
}

char StrToInt(char *str,int *result)
{
	char n=0, i=0,s=0;
	char length=strlen(str);

	if(str[length-1] != '/')  return 0;
	
    while(length)
	{	
        s=i;
        while(1)
		{					
			if(str[i] != '/')
			{
				i+=1;
                n+=1;
			}
            else
			{
				length-=n;
				length-=1;
                break;
			}
		}
        *result=split(str,s,n);   
        result+=1;				
        i+=1;
        n=0;
	}
	return 1;
}
