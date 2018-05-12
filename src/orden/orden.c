#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "orden.h"
/// referenciar a la estructua de la cual se va a crear instancia
#include "../empleado/empleado.h"
#include "../cliente/cliente.h"

//columnas de la tabla: orden
static const char *campos = "orden_id,empleado_id,cliente_id,fecha_orden,descuento";
static const char *tabla = "ordenes";
static const char *col_orden[]={"orden_id","empleado_id","cliente_id","fecha_orden","descuento"};

static const char *strSQL_SELECT_MAX_ID = "select max(orden_id) from ordenes;";
static const char *strSQL_KEY = "orden_id = %d";
static const char *strSQL_INSERT = "insert into ordenes(orden_id,empleado_id,cliente_id,fecha_orden,descuento)values(%s);";
static const char *strSQL_INSERT_Param = "%d,%d,%d,'%s',%d";
static const char *strSQL_UPDATE = "update ordenes set %s where %s;";
static const char *strSQL_UPDATE_Param ="fecha_orden ='%s', descuento=%d";
///----- variables de instancias a otras estructuras... 
static obj_empleado *empLocal=NULL;
static obj_cliente  *cliLocal=NULL;

//----------------------------------------------------
//Obtener la cadena SQL para con uso de la clave
static char *getKey(int id_orden,char *where)
{
   snprintf( where, MAX_SQL, strSQL_KEY ,id_orden);
   return  where;
}
//----------------------------------------------------
static obj_orden *this(void *self)
{
   return (obj_orden*)self;
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
void fillOrdenFromDB(void **rw , int rowi,PGresult *res)
{
    t_orden *dsorden;
    dsorden = &(( (t_orden *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD
	dsorden->orden_id = atoi(PQgetvalue(res,rowi,0));
    dsorden->empleado_id = atoi(PQgetvalue(res,rowi,1));
    dsorden->cliente_id = atoi(PQgetvalue(res,rowi,2));
    strcpy( dsorden->fecha_orden,rtrim(PQgetvalue(res,rowi,3),' '));
    dsorden->descuento = atoi(PQgetvalue(res,rowi,4));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_orden
void fillObjOrden(obj_orden *p,  t_orden rwitm)
{  
      p->orden_id = rwitm.orden_id;
      p->empleado_id = rwitm.empleado_id;
      p->cliente_id = rwitm.cliente_id;
	  strcpy( p->fecha_orden,rwitm.fecha_orden);	  
      p->descuento = rwitm.descuento;
	  p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_orden
void fillRowsOrden(void **list, int size,void *data)
{
     int i;
     obj_orden *d;
     // pedir memoria para el listado en memoria de ordenes obj_orden
    *list = (obj_orden **)malloc(sizeof(obj_orden*)* size);
    for(i=0;i<size;++i)
    {
      d = orden_new();
      fillObjOrden(d,((t_orden *)data)[i]);
      ((obj_orden **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
int exec_get_orden(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_orden),fillOrdenFromDB);
}
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset
void fill_dataset_orden(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(sizeof(t_orden)* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     {
      ((t_orden *)ds->rows)[i].orden_id  = ((t_orden *)data)[i].orden_id;
      ((t_orden *)ds->rows)[i].empleado_id = ((t_orden *)data)[i].empleado_id;
	  ((t_orden *)ds->rows)[i].cliente_id = ((t_orden *)data)[i].cliente_id;
	  strcpy( ((t_orden *)ds->rows)[i].fecha_orden,((t_orden *)data)[i].fecha_orden);
      ((t_orden *)ds->rows)[i].descuento = ((t_orden *)data)[i].descuento;
     }
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
int findAll_ordenImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, campos, tabla, sizeof(t_orden), ((data_set_orden*)((obj_orden*)self)->ds)->rows,fillOrdenFromDB,fillRowsOrden);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_orden *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para orden
int find_ordenImpl(void *self, int id_orden)
{
   int size=0; void *data;  
   obj_orden *obj = this(self);
   char where[MAX_WHERE_SQL],sql[MAX_SQL];
   strcpy(where,getKey(id_orden,where));
   strcpy(sql ,getFindSQL(campos, tabla, where));
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((data_set_orden*)obj->ds)->rows;
   size = exec_get_orden(sql,&data);
   fill_dataset_orden(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjOrden(obj,((t_orden *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
int saveObj_ordenImpl(void *self)
{
   PGconn *conn;
   PGresult *res; 
   int code=0;
   char values[MAX_WHERE_SQL];  
   char where[MAX_WHERE_SQL];
   char sql[MAX_SQL];
   obj_orden *obj = this(self);
   int newIdOrden;
    //Conexion con la base de datos 
   if ((conn = connectdb(SERVER,PORT,DB,USER,PASS))== NULL) {
      exit (-1); 
   }
   //Si esta marcado como nuevo objeto, es decir se creo instancia y no fue obtenido de la BD,
   if(obj->getIsNewObj(obj))
   {// insert   
	res = PQexec(conn, strSQL_SELECT_MAX_ID);
	code = PQresultStatus(res);
	newIdOrden = atoi(PQgetvalue(res,0,0));	
	PQclear(res);
	obj->orden_id = ++newIdOrden;
    sprintf(values, strSQL_INSERT_Param, obj->orden_id,obj->empleado_id,obj->cliente_id,obj->fecha_orden, obj->descuento);
    snprintf( sql, MAX_SQL, strSQL_INSERT, values);
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);
	obj->isNewObj=false;// marcar que esta guardado en la BD.
  }
  else
  {// update
      sprintf(values, strSQL_UPDATE_Param, obj->fecha_orden, obj->descuento);
      strcpy(where,getKey(obj->orden_id, where));
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
void showObj_ordenImpl(void *self)
{
     obj_orden *p=this(self);
    
   printf("orden_id: %d - empleado_id: %d - cliente_id:%d  fecha orden: %s descuento: %d\n",p->orden_id,p->empleado_id,p->cliente_id,p->fecha_orden,p->descuento);
}
//----------------------------------------------------
//implementacion de getters
int getOrdenId_Impl(void *self)
{ 
  obj_orden *obj = this(self);
  return obj->orden_id;
}
//----------------------------------------------------
int getEmpleadoId_Impl_Prod(void *self)
{
	obj_orden *obj = this(self);
	return obj->empleado_id;
}
//----------------------------------------------------
int getClienteId_Impl_Prod(void *self)
{
	obj_orden *obj = this(self);
	return obj->cliente_id;	
}
//----------------------------------------------------
char *getFechaOrden_Impl(void *self)
{
	obj_orden *obj = this(self);
	return obj->fecha_orden;	
}
//----------------------------------------------------
int getDescuento_Impl(void *self)
{
	obj_orden *obj = this(self);
	return obj->descuento;	
}
//----------------------------------------------------
//implementacion setters
void setEmpleadoId_Impl(void *self,int empleado_id)
{
	obj_orden *obj = this(self);	
	obj->empleado_id = empleado_id;
}
//----------------------------------------------------
void setClienteId_Impl(void *self,int cliente_id)
{ 
	obj_orden *obj = this(self);
	obj->cliente_id = cliente_id;
}
//----------------------------------------------------
void setFechaOrden_Impl(void *self,char *fecha_orden)
{ 
	obj_orden *obj = this(self);
	strcpy(obj->fecha_orden,fecha_orden);	
}
//----------------------------------------------------
void setDescuento_Impl(void *self,int descuento)
{ 
	obj_orden *obj = this(self);
	obj->descuento = descuento;
}
//----------------------------------------------------
void *getEmpleadoObj_Impl(void *self)
{
    if(empLocal==NULL)
    {
	  empLocal  = empleado_new();
	  obj_orden *obj = this(self);
	  empLocal->findbykey(empLocal,obj->getEmpleadoId(obj));
    }
	return empLocal;
}
//----------------------------------------------------
static void destroyInternal_Impl(void *self)
{//funcion para liberar las referencias a objetos internos
	obj_orden *obj = this(self);
	if(empLocal!=NULL)
      free(empLocal);
}
//----------------------------------------------------
void *init_orden(void *self, data_set *ds)
{
  obj_orden *obj;
  obj = (obj_orden *)self; 
  obj->ds = ds;
  obj->isNewObj = true;//marcar como objeto nuevo, si se instancia
  // Inicializar handlers de getters y setters
  /// getters
  obj->getOrdenId  = getOrdenId_Impl;
  obj->getEmpleadoId = getEmpleadoId_Impl_Prod;
  obj->getClienteId = getClienteId_Impl_Prod;
  obj->getFechaOrden = getFechaOrden_Impl;
  obj->getDescuento  = getDescuento_Impl;
  /// setters
  obj->setEmpleadoId = setEmpleadoId_Impl;
  obj->setClienteId = setClienteId_Impl;
  obj->setFechaOrden = setFechaOrden_Impl;
  obj->setDescuento  =  setDescuento_Impl;
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_ordenImpl;
  obj->findAll =   findAll_ordenImpl;
  obj->saveObj =   saveObj_ordenImpl; 
  obj->showObj =   showObj_ordenImpl;
  obj->getEmpleadoObj = getEmpleadoObj_Impl;
  obj->destroyInternal = destroyInternal_Impl;
  //relaciones
  
  return obj;
}
//----------------------------------------------------
obj_orden *orden_new()
{
  return (obj_orden *)init_obj(sizeof(obj_orden),sizeof(t_orden), col_orden, CNT_COL_ORD, init_orden);
}
