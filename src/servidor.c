#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h>
#include "servidor.h"

void servidor(){
    int socketServidor = crearSocket();
    int puerto = 1022;
    struct sockaddr_in6 direccionServidor = crearDireccionServidor(puerto);
    struct sockaddr_in6 direccionCliente = crearDireccionCliente();
    configurarMulticast(socketServidor);
    enlazarSocket(socketServidor, direccionServidor);

    while(1) {
        char mensaje[200];
        obtenerMensajeConsola(mensaje);
        int enviado = enviarMensajeSubred(socketServidor, mensaje, direccionCliente);
        if (enviado) {
            imprimirMensajeExito();
        } else {
            imprimirMensajeError("Error al enviar el mensaje");
        }
    }
}

int crearSocket(){
    int socketServidor = socket(AF_INET6, SOCK_DGRAM, 0);
    if (socketServidor < 0) {
        perror("Error al crear el socket");
        exit(1);
    }
    return socketServidor;
}

struct sockaddr_in6 crearDireccionServidor(int puerto){
    struct sockaddr_in6 direccionServidor;
    direccionServidor.sin6_family = AF_INET6;
    inet_pton(AF_INET6, "ff12::1", &direccionServidor.sin6_addr);
    direccionServidor.sin6_port = htons(puerto);
    direccionServidor.sin6_scope_id = 0;
    return direccionServidor;
}

struct sockaddr_in6 crearDireccionCliente(){
    struct sockaddr_in6 direccionCliente;
    direccionCliente.sin6_family = AF_INET6;
    inet_pton(AF_INET6, "ff12::/16", &direccionCliente.sin6_addr);
    direccionCliente.sin6_port = htons(1022);
    direccionCliente.sin6_scope_id = 0;
    return direccionCliente;
}

void configurarMulticast(int socketServidor){
    int multicastPermission = 1;
    if (setsockopt(socketServidor, IPPROTO_IPV6, IPV6_MULTICAST_IF, (void *)&multicastPermission, sizeof(multicastPermission)) < 0) {
        perror("Error al setear el socket para multicast");
        exit(1);
    }
}

void enlazarSocket(int socketServidor, struct sockaddr_in6 direccionServidor){
    if (bind(socketServidor, (struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) < 0) {
        perror("Error al enlazar el socket");
        exit(1);
    }
}

void obtenerMensajeConsola(char *mensaje){
    printf("Ingrese el mensaje que desea enviar\n");
    fgets(mensaje, 200, stdin);
    size_t len = strlen(mensaje);
    if (len > 0 && mensaje[len - 1] == '\n') {
        mensaje[len - 1] = '\0'; // Eliminamos el salto de línea
        len = strlen(mensaje);
    } else {
        // Limpiamos el buffer de entrada en caso de que la entrada sea demasiado larga
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
}

int enviarMensajeSubred(int socketServidor, const char *mensaje, struct sockaddr_in6 direccionCliente){
    int enviado = sendto(socketServidor, mensaje, strlen(mensaje), 0, (struct sockaddr *)&direccionCliente, sizeof(direccionCliente));
    if (enviado < 0) {
        perror("Error al enviar el mensaje");
        return 0;
    }
    return 1;
}

void imprimirMensajeExito(){
    printf("Mensaje enviado con éxito\n");
}

void imprimirMensajeError(const char *mensaje){
    perror(mensaje);
}
