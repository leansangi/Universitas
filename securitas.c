#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

int login() 
{
	char str[128],c=' ';
	char text[128];
	char name[20];
	int i=0,auxID,rc;
	sqlite3 *db;
	sqlite3_stmt *result;
	char *errMsg = 0;
	char *query = NULL;
	int h = 0;
	//int bytes;
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	system("cls");
	
	while(h != 1){
		
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
		
		asprintf(&query, "SELECT userID FROM usuarios WHERE userName = '%s';", &name);
		rc = sqlite3_prepare(db, query, -1, &result, NULL);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return(3);
		}
		while ( sqlite3_step(result) == SQLITE_ROW) {
			auxID = sqlite3_column_int(result, 0);
		}
		
		
		asprintf(&query, "SELECT * FROM passwordUser WHERE userID = '%i';", auxID);
		rc = sqlite3_prepare(db, query, -1, &result, NULL);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return(3);
		}
		while ( sqlite3_step(result) == SQLITE_ROW) {
			//fprintf(stderr, "\n clave de base: %s \n", sqlite3_column_text(result, 1)); LINEA de DEBUG
			strncpy(text, (const char*)sqlite3_column_text(result, 1), 128);
		}
		
		if(strcmp(str, text) == 0){
			h = 1;
			system("cls");
			printf("\n -----Bienvenido----- \n");
			getchar();
			system("cls");
			sqlite3_finalize(result);
			sqlite3_close(db);
		}else{
			printf("\n Clave Erronea. \n");
			getchar();
			system("cls");
		}
		
	}
	
	getPrivileges(&name);
	
	return 0;
}

int getPrivileges(char *user){
	
	sqlite3 *db;
	sqlite3_stmt *result;
	char *errMsg = 0;
	char *query = NULL;
	int rc;
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	
	
}