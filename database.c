#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <general.h>

int agregarUsuario(){
	
	int rc;
	int auxID;
	sqlite3 *db;
	sqlite3_stmt *result;
	char usrNme[20];
	char crrElec[150];
	char sede[20];
	char *errMsg = 0;
	char *query = NULL;
	char str[128],c=' ';
	int i=0;
	//char hash[256];
	
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
	
	asprintf(&query, "SELECT userID FROM usuarios WHERE userName = '%s';", &usrNme);
	rc = sqlite3_prepare(db, query, -1, &result, NULL);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(3);
	}
	while ( sqlite3_step(result) == SQLITE_ROW) {
		auxID = sqlite3_column_int(result, 0);
	}
	//printf("\n %i", auxID);
	getchar();
	
	//hashPassword(&str);
	
	asprintf(&query, "INSERT INTO passwordUser (userID, passwordHash) VALUES ('%i', '%s');", auxID, &str);
	rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
	if (errMsg != NULL) {
		printf("Error in sqlite3_exec: %s\n", errMsg);
		sqlite3_free(errMsg);
		return (2);
	}
	
	asprintf(&query, "INSERT INTO access (sam, type) VALUES ('%s', 'user');", &usrNme);
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

int modificarUsuario(){
	
	char usrNme [20];
	char usrNmeMod [20];
	char crrElec[150];
	char sede[20];
	int rc;
	sqlite3 *db;
	char *errMsg = 0;
	char *query = NULL;
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	printf("Ingrese nombre de usuario a modificar: ");
	fgets(usrNme, 20, stdin);
	printf("Ingrese nombre de usuario deseado: ");
	fgets(usrNmeMod, 20, stdin);
	printf("Ingrese correo electronico del usuario: ");
	fgets(crrElec, 150, stdin);
	printf("Ingrese sede del usuario: ");
	fgets(sede, 20, stdin);
	
	asprintf(&query, "UPDATE usuarios SET userName = '%s', email = '%s', location = '%s' WHERE userName = '%s';", &usrNmeMod, &crrElec, &sede, &usrNme);
	rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Error al crear el primer registro: %s.\n", errMsg);
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return(2);
	}
	free(query);
	
	sqlite3_close(db);
	return (0);
}

int eliminarUsuario(){
	
	char usrNme[20];
	int rc;
	sqlite3 *db;
	char *errMsg = 0;
	char *query = NULL;
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	printf("Ingrese nombre de usuario para eliminar: ");
	fgets(usrNme, 20, stdin);
	asprintf(&query, "DELETE FROM usuarios WHERE username = '%s';", &usrNme);
	rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
	if (errMsg != NULL) {
		printf("Error in sqlite3_exec: %s\n", errMsg);
		sqlite3_free(errMsg);
		return(2);
	}
	getchar();
	free(query);
	
	sqlite3_close(db);
	return(0);
}

int listarUsuarios(){
	
	int rc;
	sqlite3 *db;
	sqlite3_stmt *result;
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
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
	
	sqlite3_close(db);
	return(0);
}

int inscribirAlu(){
	
	int rc;
	sqlite3 *db;
	char *errMsg = 0;
	char *query = NULL;
	sqlite3_stmt *result;
	char aluNme[1024];
	char aluDni[20];
	char auxOpcionLocal[50];
	char auxObservaciones[1024];
	int auxIntDni, auxIntMetodo, auxLegajo, fecha;
	int auxID;
	char str[128],c=' ';
	int i=0;
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
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
	printf("%i \n", auxIntMetodo);
	getchar();
	if(auxIntMetodo == 1){
		asprintf(&query, "INSERT INTO pagos (nroCuenta, planCuotas, nroCuota, totalAdeudado, fecha) VALUES ('%i', '6', '1', '25000', '%i');", auxIntDni, fecha);
		rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
		if (errMsg != NULL) {
			printf("Error in sqlite3_exec: %s\n", errMsg);
			sqlite3_free(errMsg);
		}
		free(query);
	}else if(auxIntMetodo == 2){
		asprintf(&query, "INSERT INTO pagos (nroCuenta, planCuotas, nroCuota, totalAdeudado, fecha) VALUES ('%i', '12', '1', '25000', '%i');", auxIntDni, fecha);
		rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
		if (errMsg != NULL) {
			printf("Error in sqlite3_exec: %s\n", errMsg);
			sqlite3_free(errMsg);
		}
		free(query);
	}else {
		asprintf(&query, "INSERT INTO pagos (nroCuenta, planCuotas, nroCuota, totalAdeudado, fecha) VALUES ('%i', '1', '1', '25000', '%i');", auxIntDni, fecha);
		rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
		if (errMsg != NULL) {
			printf("Error in sqlite3_exec: %s\n", errMsg);
			sqlite3_free(errMsg);
		}
		free(query);
	}
	
	printf("\n Ingrese clave de Alumno: \n");
	while (i<=sizeof(str)){
		str[i]=getch();
		c=str[i];
		if(c==13) break;
		else printf("*");
		i++;
	}
	str[i]='\0';
	i=0;
	
	asprintf(&query, "SELECT aluID FROM alumnos WHERE aluNme = '%s';", &aluNme);
	rc = sqlite3_prepare(db, query, -1, &result, NULL);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(3);
	}
	while ( sqlite3_step(result) == SQLITE_ROW) {
		auxID = sqlite3_column_int(result, 0);
	}
	//printf("\n %i", auxID);
	
	asprintf(&query, "INSERT INTO passwordAlu (aluID, passwordHash) VALUES ('%i', '%s');", auxID, &str);
	rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
	if (errMsg != NULL) {
		printf("Error in sqlite3_exec: %s\n", errMsg);
		sqlite3_free(errMsg);
		return (2);
	}
	
	asprintf(&query, "INSERT INTO access (sam, type) VALUES ('%s', 'alu');", &aluNme);
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

int ingresarPago(){
	
	int rc;
	sqlite3 *db;
	char *errMsg = 0;
	char *query = NULL;
	sqlite3_stmt *result;
	
	char auxPago[0];
	int auxIntLoco, auxPlanCuotas, auxTotalAdeudado, auxNroCuota, auxCobro, fecha, auxIntPago;
	int auxNroCuenta = 0;
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	printf("1 -- Legajo de Alumno \n2 -- Dni del Alumno \n");
	fgets(auxPago, 15, stdin);
	auxIntPago = atoi(auxPago);
	if(auxIntPago == 1){
		printf("Ingrese Legajo de usuario: \n");
		fgets(auxPago, 1024, stdin);
		auxIntLoco = atoi(auxPago);
		printf("%i \n", auxIntLoco);
		fecha = getDate();
		asprintf(&query, "SELECT nroCuenta FROM alumnos WHERE legajo = '%i';", auxIntLoco);
		rc = sqlite3_prepare(db, query, -1, &result, NULL);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return(3);
		}
		while ( sqlite3_step(result) == SQLITE_ROW) {
			auxNroCuenta = sqlite3_column_int(result,0);
		}
		// ----------------------------------------------------------------------
		printf("%i \n", auxNroCuenta);
		asprintf(&query, "SELECT * FROM pagos WHERE nroCuenta = %i;", auxNroCuenta);
		rc = sqlite3_prepare(db, query, -1, &result, NULL);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return(3);
		}
		while ( sqlite3_step(result) == SQLITE_ROW) {
			auxPlanCuotas = sqlite3_column_int(result, 1),
			auxNroCuota = sqlite3_column_int(result, 2),
			auxTotalAdeudado = sqlite3_column_int(result, 3);
		}
		auxNroCuota = auxNroCuota+1;
		auxCobro = division(25000, auxPlanCuotas);
		auxTotalAdeudado = auxTotalAdeudado - auxCobro;
		printf("%i \n", auxNroCuenta);
		printf("Cobro a imputar: %i pesos \n", auxCobro);
		cierreMovs(auxCobro);
		getchar();
		// ----------------------------------------------------------------------
		asprintf(&query, "UPDATE pagos SET nroCuota = '%i', totalAdeudado = '%i', fecha = '%i' WHERE nroCuenta = '%i';", auxNroCuota, auxTotalAdeudado, fecha, auxNroCuenta);
		rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Error al crear el primer registro: %s.\n", errMsg);
			sqlite3_free(errMsg);
			sqlite3_close(db);
			return(2);
		}
		free(query);
	}else{
		printf("Ingrese Dni de usuario: \n");
		fgets(auxPago, 15, stdin);
		auxIntLoco = atoi(auxPago);
		fecha = getDate();
		asprintf(&query, "SELECT * FROM pagos WHERE nroCuenta = '%i';", auxIntLoco);
		rc = sqlite3_prepare(db, query, -1, &result, NULL);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return(3);
		}
		while ( sqlite3_step(result) == SQLITE_ROW) {
			auxPlanCuotas = sqlite3_column_int(result, 1),
			auxNroCuota = sqlite3_column_int(result, 2),
			auxTotalAdeudado = sqlite3_column_int(result, 3);
		}
		auxNroCuota = auxNroCuota + 1;
		auxCobro = division(25000, auxPlanCuotas);
		auxTotalAdeudado = auxTotalAdeudado - auxCobro;
		printf("Cobro a imputar: %i pesos \n", auxCobro);
		cierreMovs(auxCobro);
		getchar();
		// ----------------------------------------------------------------------
		asprintf(&query, "UPDATE pagos SET nroCuota = '%i', totalAdeudado = '%i', fecha = '%i' WHERE nroCuenta = '%i';", auxNroCuota, auxTotalAdeudado, fecha, auxIntLoco);
		rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Error al crear el primer registro: %s.\n", errMsg);
			sqlite3_free(errMsg);
			sqlite3_close(db);
			return(2);
		}
		free(query);
	}
	
	sqlite3_close(db);
	return(0);
}

int cierreCaja(){
	
	int rc;
	sqlite3 *db;
	char *errMsg = 0;
	char *query = NULL;
	sqlite3_stmt *result;
	
	char auxCierre[15];
	int auxCrr, auxFfecha, auxTickets, auxTotalActual;
	FILE *fichero;
	char nombre[15] = "CierreZ.txt";
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	printf("1 -- Realizar cierre X\n 2 -- Realizar cierre Z\n ");
	fgets(auxCierre, 15, stdin);
	auxCrr = atoi(auxCierre);
	if(auxCrr == 1){
		asprintf(&query, "SELECT * FROM cierreDia;");
		rc = sqlite3_prepare(db, query, -1, &result, NULL);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return(3);
		}
		while ( sqlite3_step(result) == SQLITE_ROW) {
			auxTickets = sqlite3_column_int(result, 0),
			auxTotalActual = sqlite3_column_int(result, 7),
			auxFfecha = sqlite3_column_int(result, 9);
		}
		system("cls"); 
		printf("\t\tCIERRE X:\n"); 
		printf("-----------------------------\n\n"); 
		printf("Dia de la fecha : '%i'\n", auxFfecha); 
		printf("Cantidad de Tickets : '%i'\n", auxTickets); 
		printf("Total cobrado hasta el momento : '%i'\n", auxTotalActual);
		printf("-----------------------------\n\n");
		getchar();
		free(query);
		//----------------------------------------------------------------------------------
	}else{
		asprintf(&query, "SELECT * FROM cierreDia;");
		rc = sqlite3_prepare(db, query, -1, &result, NULL);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return(3);
		}
		while ( sqlite3_step(result) == SQLITE_ROW) {
			auxTickets = sqlite3_column_int(result, 0),
			auxTotalActual = sqlite3_column_int(result, 7),
			auxFfecha = sqlite3_column_int(result, 9);
		}
		
		asprintf(&query, "UPDATE cierreHistorico SET cierreID = '%i', totalEfectivo = 0, totalTarjeta = 0, faltante = 0, totalFinal = '%i';", auxFfecha, auxTotalActual);
		rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Error al crear el primer registro: %s.\n", errMsg);
			sqlite3_free(errMsg);
			sqlite3_close(db);
			return(2);
		}
		
		asprintf(&query, "DELETE FROM cierreDia WHERE fecha = '%i';", auxFfecha);
		rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
		if (errMsg != NULL) {
			printf("Error in sqlite3_exec: %s\n", errMsg);
			sqlite3_free(errMsg);
		}
		
		fichero = fopen( nombre, "w" );
		printf( "Fichero: %s -> ", nombre );
		if( fichero ){
			printf( "creado (ABIERTO)\n" );
		}else{
			printf( "Error (NO ABIERTO)\n" );
			return 1;
		}
		//----------------------------------------------------------------------------------
				
		fprintf( fichero, "\t\tCIERRE Z:\n"); 
		fprintf( fichero, "-----------------------------\n\n"); 
		fprintf( fichero, "Dia de la fecha : '%i'\n", auxFfecha); 
		fprintf( fichero, "Cantidad de Tickets : '%i'\n", auxTickets); 
		fprintf( fichero, "Total cobrado hasta el momento : '%i'\n", auxTotalActual);
		fprintf( fichero, "-----------------------------\n\n");
		
		//stream = fopen("PRN", "w");
		//fprintf(stream, "\t\tCIERRE Z:\n-----------------------------\n\n");
		//----------------------------------------------------------------------------------
		if( !fclose(fichero) )
			printf( "Fichero cerrado\n" );
		else{
			printf( "Error: fichero NO CERRADO\n" );
			return 1;
		}
		free(query);
	}
	//----------------------------------------------------------------------------------
	sqlite3_close(db);
	return(0);
}

int listarAlu(){
	
	int rc;
	sqlite3 *db;
	char *errMsg = 0;
	char *query = NULL;
	sqlite3_stmt *result;
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	// Consulta a realizar sobre la tabla.
	rc = sqlite3_prepare(db, "SELECT * FROM alumnos;", -1, &result, NULL);
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
	
	sqlite3_close(db);
	return(0);
}

int bajaAlu(){
	
	int rc;
	sqlite3 *db;
	char *errMsg = 0;
	char *query = NULL;
	sqlite3_stmt *result;
	
	char aluDni[20];
	int auxDni;
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	
	printf("Ingrese DNI de alumno para dar de baja: ");
	fgets(aluDni, 20, stdin);
	auxDni = atoi(aluDni);
	asprintf(&query, "UPDATE alumnos SET estadoAlu = 'INACTIVO' WHERE aluDni = '%i';", auxDni);
	rc=sqlite3_exec(db, query, NULL, NULL, &errMsg);
	if (errMsg != NULL) {
		printf("Error in sqlite3_exec: %s\n", errMsg);
		sqlite3_free(errMsg);
	}
	getchar();
	free(query);
	
	sqlite3_close(db);
	return(0);
}




void cierreMovs(int cobro){
	
	int tickets, totalActual, ffecha, rb;
	char *aux = NULL;
	sqlite3_stmt *resultado;
	sqlite3 *db;
	char *errMsg = 0;
	
	rb = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rb != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	//----------------------------------------------------------------------------------
	asprintf(&aux, "SELECT * FROM cierreDia;");
	rb = sqlite3_prepare(db, aux, -1, &resultado, NULL);
	if (rb != SQLITE_OK) {
		fprintf(stderr, "Error en la consulta: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(3);
	}
	while ( sqlite3_step(resultado) == SQLITE_ROW) {
		tickets = sqlite3_column_int(resultado, 0),
		totalActual = sqlite3_column_int(resultado, 7),
		ffecha = sqlite3_column_int(resultado, 9);
	}
	tickets = tickets + 1;
	totalActual = totalActual + cobro;
	//----------------------------------------------------------------------------------
	asprintf(&aux, "UPDATE cierreDia SET tickets = '%i', totalActual = '%i' WHERE fecha = '%i';", tickets, totalActual, ffecha);
	rb = sqlite3_exec(db, aux, NULL, NULL, &errMsg);
	if (rb != SQLITE_OK) {
		fprintf(stderr, "Error al crear el primer registro: %s.\n", errMsg);
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return(2);
	}
	free(aux);
	sqlite3_close(db);
}