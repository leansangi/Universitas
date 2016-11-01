#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>
#include <securitas.h>
#include <conio.h>
#include <database.h>
#include <general.h>

int Menu();

int main (int argc, char **argv) {
	
	char *inicio = NULL;
	sqlite3 *db;          // Definimos un puntero a la base de datos
	char *errMsg = 0;     // Variable para el mensaje de error
	int rc;               // Variable para el retorno de la sentencia
	int fecha;
	int privs;
	
	rc = sqlite3_open("universitas.db", &db);
	// Abro la conexión con la base de datos
	if (rc != SQLITE_OK) {
		fprintf(stderr, "No se puede acceder a la base de datos: %s.\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	fecha = getDate();
	asprintf(&inicio, "INSERT INTO cierreDia(tickets, ticketEfectivo, ticketTarjeta, totalEfectivo, totalTarjeta, movimientosCaja, valorMovCaja, totalActual, faltante, fecha) VALUES (0, 0, 0, 0, 0, 0, 0, 0, 0, '%i');", fecha);
	rc=sqlite3_exec(db, inicio, NULL, NULL, &errMsg);
	if (errMsg != NULL) {
		printf("Error in sqlite3_exec: %s\n", errMsg);
		sqlite3_free(errMsg);
	}
	
	free(inicio);
	
	system("cls");
	//if(login() == 1){
	//	system("pause");
	//}
	
	privs = login();
	printf("%i\n", privs);
	getchar();
	if (privs == 5){
		menuAlu();
	}else if(privs == 6){
		menuUser();
	}else {
		printf("Broken! \n");
	}
	
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

int aluMenu() 
{
	char resp[15];
	do
	{ 
		system("cls"); 
		printf("\t\tMENU PRINCIPAL\n"); 
		printf("-----------------------------\n\n"); 
		printf("1 -- Ver Materias\n"); 
		printf("2 -- Ver Inscripciones\n"); 
		printf("3 -- Inscribirse en Materia\n"); 
		printf("0 -- Salir\n"); 
		fgets(resp, 15, stdin); 
	}while(resp[0] < '0' && resp[0] > '9'|| resp[0]=="f"); 
		return resp[0];while(resp!=0);
}

int menuUser(){
	int response;
	do{
		response = Menu();
		switch(response){
		case '1':
			agregarUsuario();
		break;
		case '2':
			modificarUsuario();
		break;
		case '3':
			eliminarUsuario();
		break;
		case '4':
			listarUsuarios();
		break;
		case '5':
			inscribirAlu();
		break;
		case '6':
			ingresarPago();
		break;
		case '7':
			cierreCaja();
		break;
		case '8':
			listarAlu();
		break;
		case '9':
			bajaAlu();
		break;
		}
	}while(response != '0');
}

int menuAlu(){
	int response;
	do{
		response = aluMenu();
		switch(response){
		case '1':
			//agregarUsuario();
		break;
		case '2':
			//modificarUsuario();
		break;
		case '3':
			//eliminarUsuario();
		break;
		}
	}while(response != '0');
}