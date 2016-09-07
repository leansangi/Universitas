#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>

int Menu();

int main (int argc, char **argv) {
	char *query = NULL;
	char usrNme[20];
	char crrElec[150];
	char sede[20];
	char resp[50];
	char fullStmt[1024];
	int response;
	char aluNme[1024];
	char aluDni[20];
	int auxIntPago;
	char auxPago[15];
	char auxOpcionLocal[50];
	char auxObservaciones[1024];
	int auxIntDni;
	int auxIntMetodo;
	int legajo;
	sqlite3 *db;          // Definimos un puntero a la base de datos
	char *errMsg = 0;     // Variable para el mensaje de error
	int rc;               // Variable para el retorno de la sentencia
	sqlite3_stmt *stmt;
	sqlite3_stmt *result; // Puntero a la respuesta de la consulta
	int auxLegajo;
	char auxChDni[1024];
	int auxPlanCuotas;
	int auxTotalAdeudado;
	int auxNroCuota;
	int auxCobro;
	int fecha;
	int auxNroCuenta = 0;
	int auxIntPago2;
	
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
			getchar();
			free(query);
		break;
		case '4':
			// Consulta a realizar sobre la tabla.
			rc = sqlite3_prepare(db, "SELECT * FROM usuarios;", -1, &result, NULL);
			// Compruebo que no hay error
			if (rc != SQLITE_OK) {
				fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(3);
			}
			// Bucle de presentación en pantalla del resultado de la consulta
			while ( sqlite3_step(result) == SQLITE_ROW) {
				fprintf(stderr, "Los usuarios actuales son:  ============= ID = %i \n UserName = %s \n Privilegios = %i \n Correo = %s \n Sede = %s.\n"
																					, sqlite3_column_int(result, 0)
																					, sqlite3_column_text(result, 1)
																					, sqlite3_column_int(result, 2)
																		, sqlite3_column_text(result, 3), sqlite3_column_text(result, 4));
				getchar();
			}
		break;
		case '5':
			printf("Ingrese nombre del nuevo alumno: \n");
			fgets(aluNme, 1024, stdin);
			printf("Ingrese numero de documento: \n");
			fgets(aluDni, 20, stdin);
			printf("Ingrese plan de pago deseado: \n 1 -- 6 cuotas sin interes \n 2 -- 12 cuotas con interes del 10 porciento \n 3 -- 1 pago con 35 porciento de descuento \n");
			fgets(auxOpcionLocal, 50, stdin);
			printf("Ingrese, de ser necesario, una observacion del usuario: \n");
			fgets(auxObservaciones, 1024, stdin);
			auxIntDni = atoi(aluDni);
			auxIntMetodo = atoi(auxOpcionLocal);
			
			//Buscamos el valor actual del contador de legajos en la tabla de Datos Auxiliares
			rc = sqlite3_prepare(db, "SELECT legajoActual FROM auxiliaryData;", -1, &result, NULL);
			if (rc != SQLITE_OK) {
				fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(3);
			}
			while ( sqlite3_step(result) == SQLITE_ROW) {
				auxLegajo = sqlite3_column_int(result,0);
			}
			++auxLegajo; //Aumentamos el valor del campo auxiliar a impactar en el alumno
			
			fecha = getDate();
			//Cargamos Alumno nuevo
			asprintf(&query, "INSERT INTO alumnos(aluNme, aluDni, metodoPago, costoTotal, nroCuenta, legajo, observaciones, fechaIngreso, estadoAlu) VALUES ('%s', '%i', '%i', '%i', '%i', '%i', '%s', '%i', 'ACTIVO');", &aluNme
															, auxIntDni, auxIntMetodo, 25000, auxIntDni, auxLegajo, &auxObservaciones, fecha);
			rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
			if (errMsg != NULL) {
				printf("Error in sqlite3_exec: %s\n", errMsg);
				sqlite3_free(errMsg);
			}
			free(query);
			
			//Actualizamos valor del campo axuiliar del numero de legajo correspondiente a cada alumno
			asprintf(&query, "UPDATE auxiliaryData SET legajoActual = %i;", auxLegajo);
			rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
			if (errMsg != NULL) {
				printf("Error in sqlite3_exec: %s\n", errMsg);
				sqlite3_free(errMsg);
			}
			getchar();
			free(query);
			
			switch(auxIntMetodo){
				case '1':
					asprintf(&query, "INSERT INTO pagos (nroCuenta, planCuotas, nroCuota, totalAdeudado, fecha) VALUES ('%i', '6', '1', '25000', '%i');", auxIntDni, fecha);
					rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
					if (errMsg != NULL) {
						printf("Error in sqlite3_exec: %s\n", errMsg);
						sqlite3_free(errMsg);
					}
					free(query);
				break;
				case '2':
				asprintf(&query, "INSERT INTO pagos (nroCuenta, planCuotas, nroCuota, totalAdeudado) VALUES ('%i', '12', '1', '25000', '%i');", auxIntDni, fecha);
					rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
					if (errMsg != NULL) {
						printf("Error in sqlite3_exec: %s\n", errMsg);
						sqlite3_free(errMsg);
					}
					free(query);
				break;
				case '3':
				asprintf(&query, "INSERT INTO pagos (nroCuenta, planCuotas, nroCuota, totalAdeudado) VALUES ('%i', '1', '1', '25000', '%i');", auxIntDni, fecha);
					rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
					if (errMsg != NULL) {
						printf("Error in sqlite3_exec: %s\n", errMsg);
						sqlite3_free(errMsg);
					}
					free(query);
				break;
			}
			
		break;
		case '6':
			auxPago[0] = '\0';
			auxIntPago2 = 0;
			auxPlanCuotas = 0;
			auxTotalAdeudado = 0;
			auxNroCuota = 0;
			auxCobro = 0;
			fecha = 0;
			printf("1 -- Legajo de Alumno \n2 -- Dni del Alumno \n");
			fgets(auxPago, 15, stdin);
			auxIntPago = atoi(auxPago);
			if(auxIntPago == 1){
				printf("Ingrese Legajo de usuario: \n");
				fgets(auxPago, 1024, stdin);
				auxIntPago2 = atoi(auxPago);
				fecha = getDate();
				asprintf(&query, "SELECT nroCuenta FROM alumnos WHERE legajo = '%i';", auxIntPago2);
				rc = sqlite3_prepare(db, query, -1, &result, NULL);
				if (rc != SQLITE_OK) {
					fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
					sqlite3_close(db);
					return(3);
				}
				while ( sqlite3_step(result) == SQLITE_ROW) {
					auxNroCuenta = sqlite3_column_int(result,0);
				}
				free(query);
				// ----------------------------------------------------------------------
				asprintf(&query, "SELECT * FROM pagos WHERE nroCuenta = '%i';", auxNroCuenta);
				rc = sqlite3_prepare(db, query, -1, &result, NULL);
				if (rc != SQLITE_OK) {
					fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
					sqlite3_close(db);
					return(3);
				}
				while ( sqlite3_step(result) == SQLITE_ROW) {
					auxPlanCuotas = sqlite3_column_int(result, 1);
					auxTotalAdeudado = sqlite3_column_int(result, 2);
					auxNroCuota = sqlite3_column_int(result, 3);
				}
				auxCobro = division(25000, auxPlanCuotas);
				auxTotalAdeudado = auxTotalAdeudado - auxCobro;
				printf("Cobro a imputar: '%i' pesos \n", auxCobro);
				// ----------------------------------------------------------------------
				asprintf(&query, "UPDATE pagos SET totalAdeudado = '%i', fecha = '%i' WHERE nroCuenta = '%i';", auxTotalAdeudado, fecha, auxNroCuenta);
				rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
				if (rc != SQLITE_OK) {
					fprintf(stderr, "Error al crear el primer registro: %s.\n", errMsg);
					sqlite3_free(errMsg);
					sqlite3_close(db);
					return(2);
				}
			}else{
				
				printf("Ingrese Dni de usuario: \n");
				fgets(auxPago, 15, stdin);
				auxIntPago2 = atoi(auxPago);
				fecha = getDate();
				asprintf(&query, "SELECT * FROM pagos WHERE nroCuenta = '%i';", auxIntPago2);
				rc = sqlite3_prepare(db, query, -1, &result, NULL);
				if (rc != SQLITE_OK) {
					fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
					sqlite3_close(db);
					return(3);
				}
				while ( sqlite3_step(result) == SQLITE_ROW) {
					auxPlanCuotas = sqlite3_column_int(result, 1);
					auxTotalAdeudado = sqlite3_column_int(result, 2);
					auxNroCuota = sqlite3_column_int(result, 3);
				}
				auxCobro = 25000 / auxPlanCuotas;
				auxTotalAdeudado = auxTotalAdeudado - auxCobro;
				printf("Cobro a imputar: '%i' pesos \n", auxCobro);
				// ----------------------------------------------------------------------
				asprintf(&query, "UPDATE pagos SET totalAdeudado = '%i', fecha = '%i' WHERE nroCuenta = '%i';", auxTotalAdeudado, fecha, auxNroCuenta);
				rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
				if (rc != SQLITE_OK) {
					fprintf(stderr, "Error al crear el primer registro: %s.\n", errMsg);
					sqlite3_free(errMsg);
					sqlite3_close(db);
					return(2);
				}
				
			}
		break;
		case '8':
			// Consulta a realizar sobre la tabla.
			rc = sqlite3_prepare(db, "SELECT * FROM alumnos", -1, &result, NULL);
			// Compruebo que no hay error
			if (rc != SQLITE_OK) {
				fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(3);
			}
			// Bucle de presentación en pantalla del resultado de la consulta
			while ( sqlite3_step(result) == SQLITE_ROW) {
				fprintf(stderr, "Los usuarios actuales son:  ============= ID = %i \n Nombre Alumno = %s \n Dni = %i \n Metodo de Pago = %i \n Costo total del anio = %i.\n Numero de Cuenta = %i \n Legajo = %i \n Observaciones = %s \n Fecha de Ingreso = %i \n Estado = %s \n"
																					, sqlite3_column_int(result, 0)
																					, sqlite3_column_text(result, 1)
																					, sqlite3_column_int(result, 2)
																		, sqlite3_column_int(result, 3), sqlite3_column_int(result, 4), sqlite3_column_int(result, 5)
																		, sqlite3_column_int(result, 6), sqlite3_column_text(result, 7), sqlite3_column_int(result, 8)
																					, sqlite3_column_text(result, 9));
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
		printf("5 -- Inscripcion nuevo Alumno\n");
		printf("6 -- Ingresar Pago\n"); 
		printf("7 -- Cierre de caja\n"); 
		printf("8 -- Listar Alumnos Actuales\n");
		printf("9 -- Dar de baja Alumno\n");
		printf("0 -- Salir\n"); 
		fgets(resp, 15, stdin); 
	}while(resp[0] < '0' && resp[0] > '9'|| resp[0]=="f"); 
		return resp[0];while(resp!=0);
}

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

int division(int a,int b) { /*DEFINICION DE FUNCION*/
	double resu;
	if(b != 0) {
		resu = (double)a/b;
	}
	else {
		printf("La division entre cero no esta permitida. ");
		return -1;
	}
		/*regresa la division de los dos numeros introducidos*/
	int resu2 = (int) resu;
	return resu2;
}