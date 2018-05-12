#include "../../config/config.h"
#include "../utils/utils.h"
#include "orm.h"
#include <string.h>
#include <malloc.h>
/*
funcion generica que recibe como argumento cadena sql puntero a array dinamico en memoria para alojar segun la funcion de
llenado pasada por parametro (puntero a funcion)
*/
int exec_get_fromDB(char *sql,void **rw, size_t t,void(*fcFill)(void **,int ri,PGresult *r)) {
    PGconn *conn;
    //Conexion con la base de datos 
	if ((conn = connectdb(SERVER,PORT,DB,USER,PASS)) == NULL){
        exit(-1);
    }
    int tuplas=0,i=0;
    PGresult *res; //Instancia para contener los datos de ejecucion de consulta
    res = PQexec(conn, sql);

    if (res != NULL && PGRES_TUPLES_OK == PQresultStatus(res)) {
        tuplas = PQntuples(res);
        if(tuplas!=0) {   
            
            *rw = malloc(t * tuplas);
            //printf("(exec_get_fromDB)  %d: t * tuplas",t * tuplas);
            for (i = 0; i<tuplas; i++) {
            fcFill(rw,i,res);
            }

        }
        PQclear(res);
    }
    disconnectdb(conn);
    return tuplas;
}
//----------------------------------------------------
/* interface generica para poder ejecuar fidall obj */
int findAllImpl(void *self, void **list, char *criteria, const char * campos, const char * tabla, size_t t, void *data,void(*fcFill)(void **,int ri, PGresult *r),void(*fillRows)(void **list,int size,void *data)) {
  
  int size=0;
  char sql[MAX_SQL], select_str[MAX_SQL], str_where[MAX_WHERE_SQL];
  memset(str_where, 0, MAX_WHERE_SQL);
  if(criteria!=NULL) {   
     //str_where = (char*)malloc(sizeof(char)*MAX_WHERE_SQL);
     //str_where =
      strcat(str_where,"where ");
     //str_where = 
     strcat( str_where , criteria );   
   }

   //select_str = (char*)malloc(sizeof(char)*MAX_SQL);
   *select_str = '\0';
   strcat(select_str, "select ");
   strcat(select_str, campos);
   strcat(select_str, " from ");
   strcat(select_str, tabla);
   strcat(select_str, " %s; ");

   //sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( sql, MAX_SQL, select_str,(str_where==NULL?"":str_where ));       
  //printf("%s\n",sql);
  // cargar el puntero data con datos desde la base de datos.
  size = exec_get_fromDB(sql,&data,t,fcFill);
  if(size>0)
    fillRows(list,size, data);
  return size;
}
//----------------------------------------------------
char *getFindSQL(const char * campos, const char * tabla, char *where)
{
  char *sql, *select_str, *str_where=NULL;
   // verificar si requiere configurar la condicion del where de la sentencia SQL
     str_where = (char*)malloc(sizeof(char)*MAX_WHERE_SQL);
     memset(str_where, 0, MAX_WHERE_SQL);                 
     str_where = strcat(str_where,"where ");
     str_where = strcat( str_where , where );
   
   select_str = (char*)malloc(sizeof(char)*MAX_SQL);
   *select_str = '\0';
   strcat(select_str, " select ");
   strcat(select_str, campos);
   strcat(select_str, " from ");
   strcat(select_str, tabla);
   strcat(select_str, " %s; ");
   
   sql = (char*)malloc(sizeof(char)*MAX_SQL);
   snprintf( sql, MAX_SQL, select_str,(str_where==NULL?"":str_where ));      
   return sql;
}
//----------------------------------------------------
void *init_obj(size_t t_obj,size_t t_ds ,const char *col[], int cnt_cols, void*(*init)(void *o,data_set *ds))
{    
     data_set *ds;
     
     size_t tam_col = 0;
     int cont = 0;
     //deducir tamaño del arreglo de columnas
	 while (*col) {
        tam_col += sizeof(*col);
        col++;
        cont++;
     }
     col = col - cont;
	 // pedir memoria para el objeto gral
     void *obj = malloc(t_obj);/// obj mem
	 ds = malloc(t_ds); /// obj->ds mem
     ds->columns = (char **)malloc(tam_col); ///obj->columns mem
     ds->cntRows=0;
     ds->cntCol = cnt_cols;
     memcpy(ds->columns,col, tam_col);
	 ((t_object*)obj)->destroyInternal=NULL;
	 return init(obj,ds);
}
//----------------------------------------------------
void destroyObj(void *objFree)
{
	t_object *obj = (t_object *)objFree;
	if(obj->destroyInternal!=NULL)
		obj->destroyInternal(obj);
	free((obj->ds)->columns);
	free(obj->ds);
	free(obj);
}
//----------------------------------------------------
void destroyObjList(void **list,int size)
{
	int i=0;
	for(i=0;i< size;++i)
	{
		destroyObj(list[i]);		
	}
	free(list);
}
//----------------------------------------------------
void listObj(void *objList, char *criteria, int freeObj, void(*lstFunc)(void *o))
{
	t_object *obj = (t_object *)objList;
	int i=0,size=0;
	void *list;
	t_object *rw;
    size = obj->findAll(obj,&list,NULL);
	for(i=0;i<size;++i)
	{
		rw = ((t_object **)list)[i];
		if(lstFunc!=NULL)
			lstFunc(rw);
		else
  		    rw->showObj(rw);
	}
	if(freeObj)
	{
		destroyObjList(list,size);
		destroyObj(objList);
	}
}
