#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>  /* Inclusion de la libreria de acceso a funcionalidad para manipular objetos y conectarse a PostgreSQL */
#include <time.h>
#include <malloc.h>

#include "utils.h"
#include "../../config/config.h"

//#define MAXFECHA 17
 //Instancia que permite manipular conexion con el servidor
//----------------------------------------------------
char* rtrim(char* string, char junk)
{
    char* original = string + strlen(string);
    while(*--original == junk);
    *(original + 1) = '\0';
    return string;
}
//----------------------------------------------------
char** fStrSplit(char *str, const char *delimiters)
{
    char * token; 
    char **tokenArray;
    int count=0;
    token = (char *)strtok(str, delimiters); // Get the first token
    tokenArray = (char**)malloc(1 * sizeof(char*));
    tokenArray[0] = NULL;
    if (!token) {       
        return tokenArray;  
    } 
    while (token != NULL) { // While valid tokens are returned
        tokenArray[count] = (char*)strdup(token);
        //printf ("%s", tokenArray[count]);
        count++;
        tokenArray = (char **)realloc(tokenArray, sizeof(char *) * (count + 1));
        token = (char *)strtok(NULL, delimiters); // Get the next token
    }
    tokenArray[count] = NULL;  /* Terminate the array */
    return tokenArray;
}
//----------------------------------------------------
char* fStrJoin(char **str, const char *delimiters, int sz_opt)
{
    char *joinedStr;
    int i = 1, size=0;
    
    if(sz_opt!=0 ) 
    {
      size = sz_opt;
    }
    else
      size = strlen(str[0])+1;
    joinedStr = (char *)realloc(NULL, size);
    strcpy(joinedStr, str[0]);
    if (str[0] == NULL){
        return joinedStr;
    }
    while (i< size ){
        joinedStr = (char*)realloc(joinedStr, strlen(joinedStr) + strlen(str[i]) + strlen(delimiters) + 1);
        strcat(joinedStr, delimiters);
        strcat(joinedStr, str[i]);
        i++;
    }
    return joinedStr;
}
//----------------------------------------------------
PGconn * connectdb(char *servidor,char *port,char *base,char *usuario,char *password)
{
   PGconn *conn;
   /*Realizar la conexion al servidor*/
   conn = PQsetdbLogin(servidor, port, NULL,NULL, base,usuario,password);
   /*Consultar estado de la operacion de conexion*/

   if (PQstatus(conn) != CONNECTION_OK)
      fprintf(stderr, "La conexion a la Base de Datos ha fallado: %s", PQerrorMessage(conn) );               
   
   return conn;
 }
//----------------------------------------------------
void disconnectdb(PGconn * conn)
{   
   /*Cerrar conexion y liberar recurso de conexion con el servidor*/
   PQfinish(conn);
}
//----------------------------------------------------
char *getFecha()
{
     char *ffecha;
     time_t t = time(NULL);
     struct tm tm = *localtime(&t);
     ffecha = malloc(sizeof(char)*MAXFECHA);
     snprintf( ffecha, MAXFECHA, "%d-%02d-%02d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
     return ffecha;
}
//----------------------------------------------------
char *getFechaHora()
{
     char *ffecha;
     time_t t = time(NULL);
     struct tm tm = *localtime(&t);
     ffecha = malloc(sizeof(char)*MAXFECHA);
     snprintf( ffecha, MAXFECHA, "%d-%02d-%02d %02d:%02d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,tm.tm_hour, tm.tm_min);
     return ffecha;
}
//----------------------------------------------------
void inicializarArregloInt(int *arreglo, int cantidad, int valor){
    int i;
    for(i = 0; i< cantidad; i++){
        arreglo[i] = valor;
    }
}
//----------------------------------------------------

static char *itoa_simple_helper(char *dest, int i) {
  if (i <= -10) {
    dest = itoa_simple_helper(dest, i/10);
  }
  *dest++ = '0' - i%10;
  return dest;
}
//----------------------------------------------------

char *itoa_simple(char *dest, int i) {
  char *s = dest;
  if (i < 0) {
    *s++ = '-';
  } else {
    i = -i;
  }
  *itoa_simple_helper(s, i) = '\0';
  return dest;
}
//----------------------------------------------------

int startswith(const char *pre, const char *str) {
    size_t lenpre = strlen(pre), lenstr = strlen(str);
    return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

// ---------------------------------------------------------------
void prepararConsulta(char** consulta, char* valor, char* clave){
	*consulta = malloc (50*sizeof(char));
	strncpy(*consulta, valor, strlen(valor)+1);
	strcat(*consulta, " = ");
	strcat(*consulta, clave);
}

// --------------------------------------------------------------------
void prepararConsultaString(char** consulta, char* valor, char* clave){
	*consulta = malloc (50*sizeof(char));
	strncpy(*consulta, valor, strlen(valor)+1);
	strcat(*consulta, " = '");
	strcat(*consulta, clave);
	strcat(*consulta, "'");
}

// ---------------------------------------------------------------------
void separarArgumentos(int inicio, char* patronFin, char**argv , int max, char***argumentos, int *cantidad){
	int i;
	int aux_cantidad = 0;
	char **aux_argumentos;
	for(i = inicio; i <= max; i++){
		if(strcmp(argv[i], patronFin) == 0){
			break;
		}
		aux_cantidad++;
	} 
	if (aux_cantidad == 0){
		mostrarAyuda("-l");
		return;
	}
	else{
		aux_argumentos = (char**)malloc(aux_cantidad*sizeof(char**)-1);
		for(i = 0; i < aux_cantidad; i++ )
			aux_argumentos[i] = argv[inicio + i];
	}
	*argumentos = aux_argumentos;
	*cantidad = aux_cantidad;
}

// --------------------------------------------------------------------------
void separarArgumentosProfesional(char **argumentos, int inicio, int maximo, int **argumentos_profesional){
	int i,j ;
	int aux_argumentos[3];
	inicializarArregloInt(aux_argumentos, 3, 0);
	for(i = inicio, j = 0; i < maximo && j < 2; i++, j++){
		if( strcmp(argumentos[i], "-esp") == 0 && !aux_argumentos[0])
			aux_argumentos[0] = 1;
		else if(strcmp(argumentos[i], "-act") == 0 && !aux_argumentos[1] && !aux_argumentos[2] ){
			aux_argumentos[1] = 1;
		}	
		else if(strcmp(argumentos[i], "-inac") == 0 && !aux_argumentos[1] && !aux_argumentos[2]){	
			aux_argumentos[2] = 1;
		}
	}
	*argumentos_profesional = aux_argumentos;
}	

// ----------------------------------------------------------------------------
void separarArgumentosTurno(char **argumentos, int inicio, int maximo, char ***argumentos_profesional){
	char *aux_argumentos[2];
	aux_argumentos[0] = NULL;
	printf("Llega aca\n");

	if(inicio+1<maximo)
		if( strcmp(argumentos[inicio], "-pdni") == 0 || strcmp(argumentos[inicio], "-prof-matricula") == 0 || strcmp(argumentos[inicio], "-prof-id") == 0){
			aux_argumentos[0] = argumentos[inicio];
			aux_argumentos[1] = argumentos[inicio+1];
		}
	printf("pupu\n");
			
	*argumentos_profesional = aux_argumentos;	
}			

//--------------------------------------------------------------------
void mostrarAyuda(char* argumento){
	printf("MODO DE USO: [-h] tpfinalc <comando> [<argumentos>]\n");
} 
