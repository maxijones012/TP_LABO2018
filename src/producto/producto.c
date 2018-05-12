#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "producto.h"
/// referenciar a la estructua de la cual se va a crear instancia
#include "../proveedor/proveedor.h"
#include "../categoria/categoria.h"

//columnas de la tabla: producto
static const char *campos = "producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia";
static const char *tabla = "productos";
static const char *col_producto[]={"producto_id","proveedor_id","categoria_id","descripcion","precio_unit","existencia"};

static const char *strSQL_SELECT_MAX_ID = "select max(producto_id) from productos;";
static const char *strSQL_KEY = "producto_id = %d";
static const char *strSQL_INSERT = "insert into productos(proveedor_id,descripcion,categoria_id,precio_unit,existencia)values(%s);";
static const char *strSQL_INSERT_Param = "%d,%d,%d,'%s',%.2f,%d";
static const char *strSQL_UPDATE = "update productos set %s where %s;";
static const char *strSQL_UPDATE_Param ="descripcion ='%s', precio_unit=%.2f, existencia=%d";
///----- variables de instancias a otras estructuras... 
static obj_proveedor *provLocal=NULL;
//----------------------------------------------------
//Obtener la cadena SQL para con uso de la clave
static char *getKey(int id_producto,char *where)
{
   snprintf( where, MAX_SQL, strSQL_KEY ,id_producto);
   return  where;
}
//----------------------------------------------------
static obj_producto *this(void *self)
{
   return (obj_producto*)self;
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
void fillProductoFromDB(void **rw , int rowi,PGresult *res)
{
    t_producto *dsproducto;
    dsproducto = &(( (t_producto *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD
	dsproducto->producto_id = atoi(PQgetvalue(res,rowi,0));
    dsproducto->proveedor_id = atoi(PQgetvalue(res,rowi,1));
    dsproducto->categoria_id = atoi(PQgetvalue(res,rowi,2));
    strcpy( dsproducto->descripcion,rtrim(PQgetvalue(res,rowi,3),' '));
    dsproducto->precio_unit = atof(PQgetvalue(res,rowi,4));
    dsproducto->existencia = atoi(PQgetvalue(res,rowi,5));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_producto
void fillObjProducto(obj_producto *p,  t_producto rwitm)
{  
      p->producto_id = rwitm.producto_id;
      p->proveedor_id = rwitm.proveedor_id;
      p->categoria_id = rwitm.categoria_id;
	  strcpy( p->descripcion,rwitm.descripcion);	  
      p->precio_unit = rwitm.precio_unit;
      p->existencia = rwitm.existencia;
	  p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_producto
void fillRowsProducto(void **list, int size,void *data)
{
     int i;
     obj_producto *d;
     // pedir memoria para el listado en memoria de productos obj_producto
    *list = (obj_producto **)malloc(sizeof(obj_producto*)* size);
    for(i=0;i<size;++i)
    {
      d = producto_new();
      fillObjProducto(d,((t_producto *)data)[i]);
      ((obj_producto **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
int exec_get_producto(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_producto),fillProductoFromDB);
}
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset
void fill_dataset_producto(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(sizeof(t_producto)* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     {
      ((t_producto *)ds->rows)[i].producto_id  = ((t_producto *)data)[i].producto_id;
      ((t_producto *)ds->rows)[i].proveedor_id = ((t_producto *)data)[i].proveedor_id;
	  ((t_producto *)ds->rows)[i].categoria_id = ((t_producto *)data)[i].categoria_id;
	  strcpy( ((t_producto *)ds->rows)[i].descripcion,((t_producto *)data)[i].descripcion);
      ((t_producto *)ds->rows)[i].precio_unit = ((t_producto *)data)[i].precio_unit;
      ((t_producto *)ds->rows)[i].existencia = ((t_producto *)data)[i].existencia;
     }
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
int findAll_productoImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, campos, tabla, sizeof(t_producto), ((data_set_producto*)((obj_producto*)self)->ds)->rows,fillProductoFromDB,fillRowsProducto);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_producto *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para producto
int find_productoImpl(void *self, int id_producto)
{
   int size=0; void *data;  
   obj_producto *obj = this(self);
   char where[MAX_WHERE_SQL],sql[MAX_SQL];
   strcpy(where,getKey(id_producto,where));
   strcpy(sql ,getFindSQL(campos, tabla, where));
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((data_set_producto*)obj->ds)->rows;
   size = exec_get_producto(sql,&data);
   fill_dataset_producto(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjProducto(obj,((t_producto *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
int saveObj_productoImpl(void *self)
{
   PGconn *conn;
   PGresult *res; 
   int code=0;
   char values[MAX_WHERE_SQL];  
   char where[MAX_WHERE_SQL];
   char sql[MAX_SQL];
   obj_producto *obj = this(self);
    //Conexion con la base de datos 
   if ((conn = connectdb(SERVER,PORT,DB,USER,PASS))== NULL) {
      exit (-1); 
   }
   //Si esta marcado como nuevo objeto, es decir se creo instancia y no fue obtenido de la BD,
   if(obj->getIsNewObj(obj))
   {// insert    
    sprintf(values, strSQL_INSERT_Param, obj->proveedor_id,obj->categoria_id,obj->descripcion, obj->precio_unit, obj->existencia);
    snprintf( sql, MAX_SQL, strSQL_INSERT, values);
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);
		// al tener clave serial en la bd, se autonumera, obtener el id generado.
	if ( code == PGRES_COMMAND_OK)  {
		res = PQexec(conn, strSQL_SELECT_MAX_ID);
		code = PQresultStatus(res);
		obj->producto_id = atoi(PQgetvalue(res,0,0));
		obj->isNewObj=false;// marcar que esta guardado en la BD.
		PQclear(res);
	}
  }
  else
  {// update
      sprintf(values, strSQL_UPDATE_Param, obj->descripcion, obj->precio_unit, obj->existencia);
      strcpy(where,getKey(obj->producto_id, where));
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
void showObj_productoImpl(void *self)
{
   obj_producto *p=this(self);
   obj_categoria *cat = p->getCategoriaObj(p);
   printf("producto_id: %d - proveedor_id: %d - categoria:%s  Descripcion: %s precio_unit: %.2f  existencia: %d\n",p->producto_id,p->proveedor_id,cat->getNombreCategoria(cat),p->descripcion,p->precio_unit,p->existencia);
}
//----------------------------------------------------
//implementacion de getters
int getProductoId_Impl(void *self)
{ 
  obj_producto *obj = this(self);
  return obj->producto_id;
}
//----------------------------------------------------
int getProveedorId_Impl_Prod(void *self)
{
	obj_producto *obj = this(self);
	return obj->proveedor_id;
}
//----------------------------------------------------
int getCategoriaId_Impl_Prod(void *self)
{
	obj_producto *obj = this(self);
	return obj->categoria_id;	
}
//----------------------------------------------------
char *getDescripcion_Impl(void *self)
{
	obj_producto *obj = this(self);
	return obj->descripcion;	
}
//----------------------------------------------------
float getPrecioUnit_Impl(void *self)
{
	obj_producto *obj = this(self);
	return obj->precio_unit;	
}
//----------------------------------------------------
int getExistencia_Impl(void *self)
{
	obj_producto *obj = this(self);
	return obj->existencia;	
}
//----------------------------------------------------
//implementacion setters
void setProveedorId_Impl(void *self,int proveedor_id)
{
	obj_producto *obj = this(self);	
	obj->proveedor_id = proveedor_id;
}
//----------------------------------------------------
void setCategoriaId_Impl(void *self,int categoria_id)
{ 
	obj_producto *obj = this(self);
	obj->categoria_id = categoria_id;
}
//----------------------------------------------------
void setDescripcion_Impl(void *self,char *descripcion)
{ 
	obj_producto *obj = this(self);
	strcpy(obj->descripcion,descripcion);	
}
//----------------------------------------------------
void setPrecioUnit_Impl(void *self,float precio_unit)
{
	obj_producto *obj = this(self);
	obj->precio_unit = precio_unit;	
}
//----------------------------------------------------
void setExistencia_Impl(void *self,int existencia)
{ 
	obj_producto *obj = this(self);
	obj->existencia = existencia;	
}
//----------------------------------------------------
static void destroyInternal_Impl(void *self)
{//funcion para liberar las referencias a objetos internos
	obj_producto *obj = this(self);
//verificar 
}
void *getCategoriaObjImpl(void *self)
{
     obj_producto *obj = this(self);
     obj_categoria *cat;
     cat = categoria_new();
     cat->findbykey(cat,obj->categoria_id);
     return cat;
 }
//----------------------------------------------------
void *init_producto(void *self, data_set *ds)
{
  obj_producto *obj = this(self);  
  obj->ds = ds;
  obj->isNewObj = true;//marcar como objeto nuevo, si se instancia
  // algunos valores por default
  obj->precio_unit = 0;
  obj->existencia = 0;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getProductoId  = getProductoId_Impl;
  obj->getProveedorId = getProveedorId_Impl_Prod;
  obj->getCategoriaId = getCategoriaId_Impl_Prod;
  obj->getDescripcion = getDescripcion_Impl;
  obj->getPrecioUnit  = getPrecioUnit_Impl;
  obj->getExistencia  = getExistencia_Impl;
  /// setters
  obj->setProveedorId = setProveedorId_Impl;
  obj->setCategoriaId = setCategoriaId_Impl;
  obj->setDescripcion = setDescripcion_Impl;
  obj->setPrecioUnit  =  setPrecioUnit_Impl;
  obj->setExistencia  =  setExistencia_Impl;
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_productoImpl;
  obj->findAll =   findAll_productoImpl;
  obj->saveObj =   saveObj_productoImpl; 
  obj->showObj =   showObj_productoImpl;
  obj->destroyInternal = destroyInternal_Impl;
  obj->getCategoriaObj = getCategoriaObjImpl;
  //relaciones
  
  return obj;
}
//----------------------------------------------------
obj_producto *producto_new()
{
  return (obj_producto *)init_obj(sizeof(obj_producto),sizeof(t_producto), col_producto, CNT_COL_PROD, init_producto);
}
