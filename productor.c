#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BROKER_ADDRESS "localhost"
#define BROKER_PORT 1883
#define TOPIC "escuela"

//Construccion de mensaje
void Mensaje (char *mensaje, int matricula, const char *nombre, const char *primer_a,
		const char *segundo_a, const char *materia, const char *calificacion, const char *carrera){
	snprintf (mensaje, 256, "%d|%s|%s|%s|%s|%s|%s", matricula, nombre, primer_a, segundo_a, materia, calificacion, carrera);

}

int main (){
struct mosquitto *mosq;
char mensaje [256];

//Variables 
int matricula;
char nombre [50], primer_a [50], segundo_a [50], materia [60], calificacion [50], carrera [50];

//biblioteca mosquito
mosquitto_lib_init ();

mosq = mosquitto_new (NULL, true, NULL);
if (!mosq){
	fprintf (stderr, "Error al inicializar MOSQUITTO \n");
	return EXIT_FAILURE;
}

//Conexion al broker mosquito
if (mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60)!= MOSQ_ERR_SUCCESS){
	fprintf (stderr, "Error al conectar con el BROKER MOSQUITTO \n");
	return EXIT_FAILURE;
}

printf ("Coectado al broker mosquito en %s: %d\n",BROKER_ADDRESS, BROKER_PORT);

while (1){
	//Solicitud de datos
	printf ("\n Ingrese los siguientes datos \n");
	printf ("Matricula: ");
	scanf ("%d", &matricula);

	printf ("Nombre:");
	scanf ("%s", nombre);

	printf ("Primer Apellido: ");
	scanf ("%s", primer_a);

	printf ("Segundo Apellido: ");
	scanf ("%s", segundo_a);

	printf ("Materia: ");
	getchar();  // Para limpiar el salto de línea de scanf previo
        fgets(materia, sizeof(materia), stdin);
        materia[strcspn(materia, "\n")] = '\0';

	printf ("Calificacion: ");
	scanf ("%s", calificacion);

	printf ("Carrera: ");
	scanf ("%s", carrera);

//generar el mensaje
	Mensaje(mensaje, matricula, nombre, primer_a, segundo_a, materia, calificacion, carrera);

//publicar el mensaje	
if (mosquitto_publish (mosq, NULL, TOPIC, strlen (mensaje), mensaje, 0, false)!=MOSQ_ERR_SUCCESS){
	fprintf (stderr, "Error al publicar el mensaje \n");
}else {
	printf ("mensaje publicado en el topic '%s' : %s\n",TOPIC,mensaje);
}
break;
}

mosquitto_disconnect (mosq);
mosquitto_destroy (mosq);
mosquitto_lib_cleanup();

return EXIT_SUCCESS;
}
