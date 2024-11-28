/*  Grupo 1 2024  */

void servidor();

int crearSocket();

struct sockaddr_in6 crearDireccionServidor(int puerto); 

struct sockaddr_in6 crearDireccionCliente();

void configurarMulticast(int socketServidor);

void enlazarSocket(int socketServidor, struct sockaddr_in6 direccionServidor);

void obtenerMensajeConsola(char *mensaje);

int enviarMensajeSubred(int socketServidor, const char *mensaje, struct sockaddr_in6 direccionCliente);

void imprimirMensajeExito();

void imprimirMensajeError(const char *mensaje);
