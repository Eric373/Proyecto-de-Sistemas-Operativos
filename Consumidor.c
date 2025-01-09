#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libConsulta.c" // Archivo de encabezado limpio para libConsulta

#define BROKER_ADDRESS "localhost"
#define BROKER_PORT 1883
#define TOPIC "escuela"

// Declaración de la función Insertar proporcionada por libConsulta
// Asegúrate de que esta declaración coincida con la implementación en libConsulta.so
int Insertar(int matricula, const char *nombre, const char *primer_a, const char *segundo_a,
             const char *materia, const char *calificacion, const char *carrera);

// Callback para manejar los mensajes recibidos
void mensaje_recibido(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    if (message->payloadlen) {
        printf("Mensaje recibido: %s\n", (char *)message->payload);

        // Procesar el mensaje
        char *payload = strdup((char *)message->payload);
        char *tokens[7];
        int i = 0;
        char *token = strtok(payload, "|");

        while (token && i < 7) {
            tokens[i++] = token;
            token = strtok(NULL, "|");
        }

        // Validar que se hayan recibido todos los campos necesarios
        if (i == 7) {
            int matricula = atoi(tokens[0]);
            const char *nombre = tokens[1];
            const char *primer_a = tokens[2];
            const char *segundo_a = tokens[3];
            const char *materia = tokens[4];
            const char *calificacion = tokens[5];
            const char *carrera = tokens[6];

            // Insertar en la base de datos
            if (Insertar(matricula, nombre, primer_a, segundo_a, materia, calificacion, carrera) == 0) {
                printf("Datos insertados correctamente en la base de datos.\n");
            } else {
                fprintf(stderr, "Error al insertar los datos en la base de datos.\n");
            }
        } else {
            fprintf(stderr, "Error al procesar el mensaje: formato inválido.\n");
        }

        free(payload);
    } else {
        fprintf(stderr, "Mensaje recibido sin contenido.\n");
    }
}

int main() {
    struct mosquitto *mosq;

    // Inicializar la librería Mosquitto
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        fprintf(stderr, "Error al inicializar Mosquitto.\n");
        return EXIT_FAILURE;
    }

    // Establecer el callback para recibir mensajes
    mosquitto_message_callback_set(mosq, mensaje_recibido);

    // Conectar al broker
    if (mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectar al broker Mosquitto.\n");
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return EXIT_FAILURE;
    }

    // Suscribirse al topic
    if (mosquitto_subscribe(mosq, NULL, TOPIC, 0) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al suscribirse al topic.\n");
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return EXIT_FAILURE;
    }

    printf("Esperando mensajes en el topic '%s'...\n", TOPIC);

    // Bucle principal para escuchar mensajes
    mosquitto_loop_forever(mosq, -1, 1);

    // Limpiar y desconectar
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return EXIT_SUCCESS;
}

