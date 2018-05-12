#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h> 

#ifndef __DEFOPTS__
#define __DEFOPTS__
typedef enum { false=0,true=1} bool;

#define MAX 80
#define MAXFECHA 17
#define MAX1 30
#define MAX2 80
#define MAX3 90
#define MAX4 2
#define MAX5 150
#define MAX30 30
#define MAX50 50
#define MAX10 10
#define MAX12 12
#define MAXOBS 200
#define CANT_TABLES 7
#define MAX_SQL 800
#define MAX_WHERE_SQL 600

#define SERVER "localhost"
#define PORT "5432"
#define DB "negocio"
#define USER "postgres"
#define PASS "postgres"
 
#endif
