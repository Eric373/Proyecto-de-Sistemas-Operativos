#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

#ifndef LIBCONSULTA_H
#define LIBCONSULTA_H

int Insertar (int matricula, const char *nombre, const char *primer_a, const char *segundo_a,
const char *materia, const char *calificacion, const char *carrera){

#endif

MYSQL *conn;
char query [512];

//Conexion a la base de datos 
conn = mysql_init (NULL);
if (!conn){
	fprintf (stderr, "Error al iniciar la base de datos\n");
	exit (EXIT_FAILURE);
}

if (!mysql_real_connect (conn, "localhost", "Eric", "21112003", "Escuela", 0, NULL, 0)){
	fprintf (stderr, "Error al conectar la base de datos: %s \n",mysql_error (conn));
	mysql_close (conn);
	exit (EXIT_FAILURE);
}

//Consulta 
snprintf(query, sizeof (query),
	"INSERT INTO alumnos (matricula, nombre, Primer_A, Segundo_A, materia, calificacion, carrera)"
	"VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s')",
	matricula, nombre, primer_a, segundo_a, materia, calificacion, carrera);


//ejecucion consulta
if (mysql_query(conn, query)){
	fprintf (stderr, "Error al insertar en la base de datos: %s\n", mysql_error (conn));
}else {
	printf ("Datos insertados correctamente \n");
}

mysql_close (conn);

}
