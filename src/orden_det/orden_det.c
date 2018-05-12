#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "orden_det.h"
/// referenciar a la estructua de la cual se va a crear instancia
#include "../producto/producto.h"
#include "../orden/orden.h"

//columnas de la tabla: orden_det
static const char *campos = "orden_id,detalle_id,producto_id,cantidad";
static const char *tabla = "detalle_ordenes";
static const char *col_orden_det[]={"orden_id","detalle_id","producto_id","cantidad"};

static const char *strSQL_KEY = "orden_id = %d and detalle_id = %d";
static const char *strSQL_INSERT = "insert into detalle_ordenes(orden_id,detalle_id,producto_id,cantidad)values(%s);";
static const char *strSQL_INSERT_Param = "%d,%d,%d,%d";
static const char *strSQL_UPDATE = "update detalle_ordenes set %s where %s;";
static const char *strSQL_UPDATE_Param ="cantidad=%d";

//----------------------------------------------------
//Obtener la cadena SQL para con uso de la clave
static char *getKey(int orden_id,int detalle_id,char *where)
{
   snprintf( where, MAX_SQL, strSQL_KEY ,orden_id,detalle_id);
   return  where;
}
//----------------------------------------------------
static obj_orden_det *this(void *self)
{
   return (obj_orden_det*)self;
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
void fillOrdenDetFromDB(void **rw , int rowi,PGresult *res)
{
    t_orden_det *dsorden_det;
    dsorden_det = &(( (t_orden_det *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD
	dsorden_det->orden_id = atoi(PQgetvalue(res,rowi,0));
    dsorden_det->detalle_id = atoi(PQgetvalue(res,rowi,1));
    dsorden_det->producto_id = atoi(PQgetvalue(res,rowi,2));
    dsorden_det->cantidad = atoi(PQgetvalue(res,rowi,3));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_orden_det
void fillObjOrdenDet(obj_orden_det *p,  t_orden_det rwitm)
{  
      p->orden_id = rwitm.orden_id;
      p->detalle_id = rwitm.detalle_id;
      p->producto_id = rwitm.producto_id;
      p->cantidad = rwitm.cantidad;
	  p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_orden_det
void fillRowsOrdenDet(void **list, int size,void *data)
{
     int i;
     obj_orden_det *d;
     // pedir memoria para el listado en memoria de detalle_ordenes obj_orden_det
    *list = (obj_orden_det **)malloc(sizeof(obj_orden_det*)* size);
    for(i=0;i<size;++i)
    {
      d = orden_det_new();
      fillObjOrdenDet(d,((t_orden_det *)data)[i]);
      ((obj_orden_det **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
int exec_get_orden_det(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_orden_det),fillOrdenDetFromDB);
}
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset
void fill_dataset_orden_det(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(sizeof(t_orden_det)* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     {
      ((t_orden_det *)ds->rows)[i].orden_id  = ((t_orden_det *)data)[i].orden_id;
      ((t_orden_det *)ds->rows)[i].detalle_id = ((t_orden_det *)data)[i].detalle_id;
	  ((t_orden_det *)ds->rows)[i].producto_id = ((t_orden_det *)data)[i].producto_id;
      ((t_orden_det *)ds->rows)[i].cantidad = ((t_orden_det *)data)[i].cantidad;
     }
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
int findAll_orden_detImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, campos, tabla, sizeof(t_orden_det), ((data_set_orden_det*)((obj_orden_det*)self)->ds)->rows,fillOrdenDetFromDB,fillRowsOrdenDet);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_orden_det *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para orden_det
int find_orden_detImpl(void *self, int orden_id,int detalle_id)
{
   int size=0; void *data;  
   obj_orden_det *obj = this(self);
   char where[MAX_WHERE_SQL],sql[MAX_SQL];
   strcpy(where,getKey(orden_id,detalle_id,where));
   strcpy(sql ,getFindSQL(campos, tabla, where));
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((data_set_orden_det*)obj->ds)->rows;
   size = exec_get_orden_det(sql,&data);
   fill_dataset_orden_det(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjOrdenDet(obj,((t_orden_det *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
int saveObj_orden_detImpl(void *self)
{
   PGconn *conn;
   PGresult *res; 
   int code=0;
   char values[MAX_WHERE_SQL];  
   char where[MAX_WHERE_SQL];
   char sql[MAX_SQL];
   obj_orden_det *obj = this(self);
    //Conexion con la base de datos 
   if ((conn = connectdb(SERVER,PORT,DB,USER,PASS))== NULL) {
      exit (-1); 
   }
   //Si esta marcado como nuevo objeto, es decir se creo instancia y no fue obtenido de la BD,
   if(obj->getIsNewObj(obj))
   {// insert    
    sprintf(values, strSQL_INSERT_Param, obj->orden_id,obj->detalle_id,obj->producto_id, obj->cantidad);
    snprintf( sql, MAX_SQL, strSQL_INSERT, values);
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);
	obj->isNewObj=false;// marcar que esta guardado en la BD.
  }
  else
  {// update
      sprintf(values, strSQL_UPDATE_Param, obj->cantidad);
      strcpy(where,getKey(obj->orden_id, obj->detalle_id,where));
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
void showObj_orden_detImpl(void *self)
{
     obj_orden_det *p=this(self);
   
   printf("orden_id: %d - detalle_id: %d - producto_id:%d  cantidad: %d\n",p->orden_id,p->detalle_id,p->producto_id,p->cantidad);
}
//----------------------------------------------------
//implementacion de getters
int getOrdenId_Impl_OrdenDel(void *self)
{ 
  obj_orden_det *obj = this(self);
  return obj->orden_id;
}
//----------------------------------------------------
int getDetalleId_Impl(void *self)
{
	obj_orden_det *obj = this(self);
	return obj->detalle_id;
}
//----------------------------------------------------
int getProductoId_Impl_OrdenDel(void *self)
{
	obj_orden_det *obj = this(self);
	return obj->producto_id;	
}
//----------------------------------------------------
int getCantidad_Impl(void *self)
{
	obj_orden_det *obj = this(self);
	return obj->cantidad;	
}
//----------------------------------------------------
//implementacion setters
void setDetalleId_Impl(void *self,int detalle_id)
{
	obj_orden_det *obj = this(self);	
	obj->detalle_id = detalle_id;
}
//----------------------------------------------------
void setProductoId_Impl(void *self,int producto_id)
{ 
	obj_orden_det *obj = this(self);
	obj->producto_id = producto_id;
}
//----------------------------------------------------
void setCantidad_Impl(void *self,int cantidad)
{ 
	obj_orden_det *obj = this(self);
	obj->cantidad = cantidad;	
}
//----------------------------------------------------
void *init_orden_det(void *self, data_set *ds)
{
  obj_orden_det *obj;
  obj = (obj_orden_det *)self; 
  obj->ds = ds;
  obj->isNewObj = true;//marcar como objeto nuevo, si se instancia
  // Inicializar handlers de getters y setters
  /// getters
  obj->getOrdenId  = getOrdenId_Impl_OrdenDel;
  obj->getDetalleId = getDetalleId_Impl;
  obj->getProductoId = getProductoId_Impl_OrdenDel;
  obj->getCantidad  = getCantidad_Impl;
  /// setters
  obj->setDetalleId = setDetalleId_Impl;
  obj->setProductoId = setProductoId_Impl;
  obj->setCantidad  =  setCantidad_Impl;
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_orden_detImpl;
  obj->findAll =   findAll_orden_detImpl;
  obj->saveObj =   saveObj_orden_detImpl; 
  obj->showObj =   showObj_orden_detImpl;  
  //relaciones
  
  return obj;
}
//----------------------------------------------------
obj_orden_det *orden_det_new()
{
  return (obj_orden_det *)init_obj(sizeof(obj_orden_det),sizeof(t_orden_det), col_orden_det, CNT_COL_ORDDET, init_orden_det);
}
