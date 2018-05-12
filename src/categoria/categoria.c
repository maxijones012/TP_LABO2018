#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "categoria.h"

//columnas de la tabla: categoria
static const char *campos = "categoria_id,nombre_categoria";
static const char *tabla = "categorias";
static const char *col_categoria[]={"categoria_id","nombre_categoria"};

static const char *strSQL_SELECT_MAX_ID = "select max(categoria_id) from categorias;";
static const char *strSQL_KEY = "categoria_id = %d";
static const char *strSQL_INSERT = "insert into categorias(categoria_id,nombre_categoria)values(%s);";
static const char *strSQL_INSERT_Param = "%d,'%s'";
static const char *strSQL_UPDATE = "update categorias set %s where %s;";
static const char *strSQL_UPDATE_Param ="nombre_categoria = '%s'";

//----------------------------------------------------
//Obtener la cadena SQL para con uso de la clave
static char *getKey(int k,char *where)
{
   snprintf( where, MAX_SQL, strSQL_KEY ,k);
   return  where;
}
//----------------------------------------------------
static obj_categoria *this(void *self)
{
   return (obj_categoria*)self;
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
void fillCategoriaFromDB(void **rw , int rowi,PGresult *res)
{
    t_categoria *dscategoria;
    dscategoria = &(( (t_categoria *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD
	dscategoria->categoria_id = atoi(PQgetvalue(res,rowi,0));
    strcpy( dscategoria->nombre_categoria ,rtrim(PQgetvalue(res,rowi,1),' '));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_categoria
void fillObjCategoria(obj_categoria *p,  t_categoria rwitm)
{  
      p->categoria_id = rwitm.categoria_id;
	  strcpy( p->nombre_categoria,rwitm.nombre_categoria);
	  p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_categoria
void fillRowsCategoria(void **list, int size,void *data)
{
     int i;
     obj_categoria *d;
     // pedir memoria para el listado en memoria de categorias obj_categoria
    *list = (obj_categoria **)malloc(sizeof(obj_categoria*)* size);
    for(i=0;i<size;++i)
    {
      d = categoria_new();
      fillObjCategoria(d,((t_categoria *)data)[i]);
      ((obj_categoria **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
int exec_get_categoria(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_categoria),fillCategoriaFromDB);
}
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset
void fill_dataset_categoria(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(sizeof(t_categoria)* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     {
      ((t_categoria *)ds->rows)[i].categoria_id =((t_categoria *)data)[i].categoria_id;
      strcpy( ((t_categoria *)ds->rows)[i].nombre_categoria,((t_categoria *)data)[i].nombre_categoria);
     }
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
int findAll_categoriaImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, campos, tabla, sizeof(t_categoria), ((data_set_categoria*)((obj_categoria*)self)->ds)->rows,fillCategoriaFromDB,fillRowsCategoria);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_categoria *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para categoria
int find_categoriaImpl(void *self, int k)
{
   int size=0; void *data;  
   obj_categoria *obj = this(self);
   char where[MAX_WHERE_SQL],sql[MAX_SQL];
   strcpy(where,getKey(k,where));
   strcpy(sql ,getFindSQL(campos, tabla, where));
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((data_set_categoria*)obj->ds)->rows;
   size = exec_get_categoria(sql,&data);
   fill_dataset_categoria(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjCategoria(obj,((t_categoria *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
int saveObj_categoriaImpl(void *self)
{
   PGconn *conn;
   PGresult *res; 
   int code=0;
   char values[MAX_WHERE_SQL];  
   char where[MAX_WHERE_SQL];
   char sql[MAX_SQL];
   int newIdCategoria;
   obj_categoria *obj = this(self);
    //Conexion con la base de datos 
   if ((conn = connectdb(SERVER,PORT,DB,USER,PASS))== NULL) {
      exit (-1); 
   }
   //Si esta marcado como nuevo objeto, es decir se creo instancia y no fue obtenido de la BD,
   if(obj->getIsNewObj(obj))
   {// insert
		res = PQexec(conn, strSQL_SELECT_MAX_ID);
		code = PQresultStatus(res);
		newIdCategoria = atoi(PQgetvalue(res,0,0));	
		PQclear(res);	
		sprintf(values, strSQL_INSERT_Param, ++newIdCategoria, obj->nombre_categoria);
		snprintf( sql, MAX_SQL, strSQL_INSERT, values);
		obj->categoria_id = newIdCategoria;
		res = PQexec(conn, sql);
		code = PQresultStatus(res);
		PQclear(res);
		obj->isNewObj=false;// marcar que esta guardado en la BD.
  }
  else
  {// update
      sprintf(values, strSQL_UPDATE_Param, obj->nombre_categoria);
      strcpy(where,getKey(obj->categoria_id,where));
	  snprintf( sql, MAX_SQL, strSQL_UPDATE ,values,where);      
      res = PQexec(conn, sql) ;
      code = PQresultStatus(res);
      PQclear(res);
  }
  if ( code != PGRES_COMMAND_OK)  {
        disconnectdb(conn);     
       return false;
  }
    else
    {
       disconnectdb(conn);
       return true;    
    }
}
//----------------------------------------------------
void showObj_categoriaImpl(void *self)
{
     obj_categoria *self_o=this(self);
     obj_categoria *sup;     
     printf("categoria_id: %d  categoria:%s \n",self_o->categoria_id,self_o->nombre_categoria);
}
//----------------------------------------------------
//implementacion de getters
int getCategoriaId_Impl(void *self)
{ 
  obj_categoria *obj = this(self);
  return obj->categoria_id;
}
//----------------------------------------------------
char *getNombreCategoria_Impl(void *self)
{
	obj_categoria *obj = this(self);
	return obj->nombre_categoria;	
}
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
void setNombreCategoria_Impl(void *self,char *nombre_categoria)
{ 
	obj_categoria *obj = this(self);
	strcpy(obj->nombre_categoria,nombre_categoria);
}
//----------------------------------------------------
void *init_categoria(void *self, data_set *ds)
{
  obj_categoria *obj;
  obj = (obj_categoria *)self; 
  obj->ds = ds;
  obj->isNewObj = true;//marcar como objeto nuevo, si se instancia
  // Inicializar handlers de getters y setters
  /// getters
  obj->getCategoriaId  	  = getCategoriaId_Impl;
  obj->getNombreCategoria = getNombreCategoria_Impl;  
  /// setters  
  obj->setNombreCategoria = setNombreCategoria_Impl;  
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_categoriaImpl;
  obj->findAll =   findAll_categoriaImpl;
  obj->saveObj =   saveObj_categoriaImpl; 
  obj->showObj =   showObj_categoriaImpl;      
  return obj;
}
//----------------------------------------------------
obj_categoria *categoria_new()
{
  return (obj_categoria *)init_obj(sizeof(obj_categoria),sizeof(t_categoria), col_categoria, CNT_COL_CAT, init_categoria);
}
//----------------------------------------------------
