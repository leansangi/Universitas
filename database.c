#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int agregarUsuario(){
	
	int rc;
	sqlite3 *db;
	char usrNme[20];
	char crrElec[150];
	char sede[20];
	char *errMsg = 0;
	char *query = NULL;
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	printf("Ingrese nombre de usuario para el acceso: \n");
	fgets(usrNme, 20, stdin);
	printf("Ingrese correo electronico del usuario: \n");
	fgets(crrElec, 150, stdin);
	printf("Ingrese sede del usuario: \n");
	fgets(sede, 20, stdin);
	
	asprintf(&query, "INSERT INTO usuarios (userName, email, location) VALUES ('%s', '%s', '%s');", &usrNme, &crrElec, &sede);
	rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
	if (errMsg != NULL) {
		printf("Error in sqlite3_exec: %s\n", errMsg);
		sqlite3_free(errMsg);
		return (2);
	}
	free(query);
	
	sqlite3_close(db);
	return (0);
}