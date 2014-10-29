#include   <stdio.h>
#include   <stdlib.h>
#include   <string.h>

/*
Encrypt(char*   cSrc,char*   cDest)   //字符串加密

{

 char   c;

 int   i,h,l,j=0;

 for   (i=0;i<(int)strlen(cSrc);i++)

 {

   c=cSrc[i];

   h=(c>>4)&0xf;

   l=c&0xf;

   cDest[j]=h+'x';

   cDest[j+1]=l+'z';

   j+=2;

  }

 cDest[j]='\0';

 return   0;

}
*/

int Decrypt(char*   cSrc,char*   cDest)   //字符串解密
{
 	int   i,h,l,m,n,j=0;
 	for(i=0;i<(int)strlen(cSrc);i=i+2)
 	{
   		h=(cSrc[i]-'x');
  		l=(cSrc[i+1]-'z');
   		m=(h<<4);
   		n=(l&0xf);
   		cDest[j]=m   +   n;
   		j++;
  	}
 	cDest[j]='\0';
 	return   0;	
}
/*
main(int   argc,   char   *argv[])

{

 char   dest1[128]="\0";

 char   dest2[128]="\0";

 char   src[128]="\0";

 printf("请输入源字符串：");

 scanf("%s",src);

 Encrypt(src,dest1);

 printf("加密后的字符串为：%s\n",dest1);

 Decrypt(dest1,dest2);

 printf("还原后的字符串为：%s\n",dest2);

 printf("长度为：%d\n",strlen(dest2));

 return   0;

}
*/
