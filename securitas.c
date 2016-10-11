#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

int login() 
{
	char str[20],c=' ';
	char name[20];
	int i=0;
	printf("\n Ingrese nombre de usuario: \n");
	fgets(name, 20, stdin);
	printf("\n Ingrese clave de usuario: \n");
	while (i<=sizeof(str)){
		str[i]=getch();
		c=str[i];
		if(c==13) break;
		else printf("*");
		i++;
	}
	str[i]='\0';
	i=0;
	printf("\n");
	//printf("\n Your password is : %s",str);
	
	return 0;
}