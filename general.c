#include <stdio.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <database.h>

int getDate(){
	int ff;
	time_t now;
	struct tm ts;
	char buf[80];
	// Get current time
	time(&now);
	// Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
	ts = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%d%m", &ts);
	ff = atoi(buf);
	return (ff);
}

int division(int a,int b) { //DEFINICION DE FUNCION
	double resu;
	if(b != 0) {
		resu = (double)a/b;
	}
	else {
		printf("La division entre cero no esta permitida. ");
		return -1;
	}
		//regresa la division de los dos numeros introducidos
	int resu2 = (int) resu;
	return resu2;
}

