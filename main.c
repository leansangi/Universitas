#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

int Menu();

int main (int argc, char **argv) {
	char *query = NULL;
	char usrNme[20];
	char crrElec[150];
	char sede[20];
	char resp[50];
	char fullStmt[1024];
	int response;
	sqlite3 *db;          // Definimos un puntero a la base de datos
	char *errMsg = 0;     // Variable para el mensaje de error
	int rc;               // Variable para el retorno de la sentencia
	sqlite3_stmt *stmt;
	sqlite3_stmt *result; // Puntero a la respuesta de la consulta
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	do
	{
		response = Menu();
	
		switch(response){
		case '1':
			printf("Ingrese nombre de usuario para el acceso: ");
			fgets(usrNme, 20, stdin);
			printf("Ingrese correo electronico del usuario: ");
			fgets(crrElec, 150, stdin);
			printf("Ingrese sede del usuario: ");
			fgets(sede, 20, stdin);
		
			strcpy(fullStmt, "INSERT INTO usuarios (userName, email, location) VALUES ('");
			strncat(fullStmt, usrNme, 1024);
			strncat(fullStmt, "', '", 1024);
			strncat(fullStmt, crrElec, 1024);
			strncat(fullStmt, "', '", 1024);
			strncat(fullStmt, sede, 1024);
			strncat(fullStmt, "');", 1024);
			printf("\n");
			printf(fullStmt);
			rc = sqlite3_exec(db, fullStmt, NULL, NULL, &errMsg);
			if (rc != SQLITE_OK) {
				fprintf(stderr, "Error al crear el primer registro: %s.\n", errMsg);
				sqlite3_free(errMsg);
				sqlite3_close(db);
				return(2);
			}
			fullStmt[0] = '\0';
		break;
		case '2':
			printf("Ingrese nombre de usuario a modificar: ");
			fgets(usrNme, 20, stdin);
			printf("Ingrese nombre de usuario deseado: ");
			fgets(usrNme, 20, stdin);
			printf("Ingrese correo electronico del usuario: ");
			fgets(crrElec, 150, stdin);
			printf("Ingrese sede del usuario: ");
			fgets(sede, 20, stdin);
			strcpy(fullStmt, "UPDATE usuarios SET userName = '");
			strncat(fullStmt, usrNme, 1024);
			strncat(fullStmt, "', email = '", 1024);
			strncat(fullStmt, crrElec, 1024);
			strncat(fullStmt, "', location = '", 1024);
			strncat(fullStmt, sede, 1024);
			strncat(fullStmt, "' WHERE userName = '", 1024);
			strncat(fullStmt, usrNme, 1024);
			strncat(fullStmt, "';", 1024);
			printf(fullStmt);
			getchar();
			rc = sqlite3_exec(db, fullStmt, NULL, NULL, &errMsg);
			if (rc != SQLITE_OK) {
				fprintf(stderr, "Error al crear el primer registro: %s.\n", errMsg);
				sqlite3_free(errMsg);
				sqlite3_close(db);
				return(2);
			}
			fullStmt[0] = '\0';
		break;
		case '3':
			printf("Ingrese nombre de usuario para eliminar: ");
			fgets(usrNme, 20, stdin);
			asprintf(&query, "DELETE FROM usuarios WHERE username = '%s';", &usrNme);
			rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
			if (errMsg != NULL) {
				printf("Error in sqlite3_exec: %s\n", errMsg);
				sqlite3_free(errMsg);
			}
			free(query);
			//strcpy(fullStmt, "DELETE FROM usuarios WHERE userName = '");
			//strncat(fullStmt, usrNme, 1024);
			//strncat(fullStmt, "';", 1024);
			//printf(fullStmt);
			//rc = sqlite3_exec(db, fullStmt, NULL, NULL, &errMsg);
			//if (rc != SQLITE_OK) {
				//fprintf(stderr, "Error al crear el primer registro: %s.\n", errMsg);
				//sqlite3_free(errMsg);
				//sqlite3_close(db);
				//return(2);
			//}
			fullStmt[0] = '\0';
		break;
		case '4':
			// Consulta a realizar sobre la tabla.
			// En este caso quiero los campos idEmpresa y Nombre de la tabla Empresa
			rc = sqlite3_prepare(db, "SELECT * FROM usuarios", -1, &result, NULL);
			// Compruebo que no hay error
			if (rc != SQLITE_OK) {
				fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(3);
			}
			// Bucle de presentación en pantalla del resultado de la consulta
			while ( sqlite3_step(result) == SQLITE_ROW) {
				fprintf(stderr, "Los usuarios actuales son:  ID = %i - UserName = %s - Privilegios = %i - Correo = %s - Sede = %s.\n", sqlite3_column_int(result, 0)
																					, sqlite3_column_text(result, 1)
																					, sqlite3_column_int(result, 2)
																		, sqlite3_column_text(result, 3), sqlite3_column_text(result, 4));
				getchar();
			}
		break;
		}
	}while(response != '0');
	
	// Cierro la conexión
	sqlite3_close(db);
	return(0);
}

int Menu() 
{
	char resp[15];
	do
	{ 
		system("cls"); 
		printf("\t\tMENU PRINCIPAL\n"); 
		printf("-----------------------------\n\n"); 
		printf("1 -- Agregar Usuario\n"); 
		printf("2 -- Modificar Usuario\n"); 
		printf("3 -- Eliminar Usuario\n"); 
		printf("4 -- Listar Usuarios Actuales\n"); 
		printf("0- Salir\n"); 
		fgets(resp, 15, stdin); 
	}while(resp[0] < '0' && resp[0] > '9'|| resp[0]=="f"); 
		return resp[0];while(resp!=0);
}

