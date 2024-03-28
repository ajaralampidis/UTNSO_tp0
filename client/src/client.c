#include "client.h"
#include <string.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	if (config == NULL) {
    	// ¡No se pudo crear el config!
    	// Terminemos el programa
		return 1;
	}
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");
	// Loggeamos el valor de config
	log_info(logger, "%s", valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);
	log_info(logger, "Post leer consola");

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	log_info(logger, "ip: %s | puerto: %s ", ip, puerto);
	conexion = crear_conexion(ip, puerto);
	log_info(logger, "Post crear conexión");


	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	log_info(logger, "Post mensaje enviado");


	// Armamos y enviamos el paquete
	paquete(conexion);
	log_info(logger, "Post paquete enviado");


	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente


}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("./tp0.log", "prueba", true, LOG_LEVEL_INFO);
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
		// char *current_dir = getcwd(NULL, 0);
   		// printf("El directorio actual es %s\n", current_dir);
		// free(current_dir);
		nuevo_config = config_create("cliente.config");


	return nuevo_config;
}

void leer_consola(t_log* logger)
{

	// La primera te la dejo de yapa

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	char* leido;

	while (1) {
        leido = readline(">");

        if (!strncmp(leido, "", 1)) {
            break;
        }
        printf("%s\n", leido);
        free(leido);
    }

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;
	paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while (1) {
        leido = readline("> ");
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
        free(leido);

        if (!strncmp(leido, "", 1)) {
            break;
        }

    }
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	free(leido);

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  	log_destroy(logger);
		liberar_conexion(conexion);
}
