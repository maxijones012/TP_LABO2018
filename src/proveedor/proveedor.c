#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "proveedor.h"

//columnas de la tabla: proveedor
static const char *campos = "proveedor_id,nombre_proveedor,contacto,celu_prov,fijo_prov";
static const char *tabla = "proveedores";
static const char *col_proveedor[]={"proveedor_id","nombre_proveedor","contacto","celu_prov","fijo_prov"};

static const char *strSQL_SELECT_MAX_ID = "select max(proveedor_id) from proveedores;";
static const char *strSQL_KEY = "proveedor_id = %d";
static const char *strSQL_INSERT = "insert into proveedores(proveedor_id,nombre_proveedor,celu_prov,contacto,fijo_prov)values(%s);";
static const char *strSQL_INSERT_Param = "'%s','%s','%s','%s'";
static const char *strSQL_UPDATE = "update proveedores set %s where %s;";
static const char *strSQL_UPDATE_Param ="nombre_proveedor = '%s', celu_prov ='%s', contacto = '%s', fijo_prov='%s'";

//----------------------------------------------------
//Obtener la cadena SQL para con uso de la clave
static char *getKey(int k,char *where)
{
   snprintf( where, MAX_SQL, strSQL_KEY ,k);
   return  where;
}
//----------------------------------------------------
static obj_proveedor *this(void *self)
{
   return (obj_proveedor*)self;
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
void fillProveedorFromDB(void **rw , int rowi,PGresult *res)
{
    t_proveedor *dsproveedor;
    dsproveedor = &(( (t_proveedor *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD
	dsproveedor->proveedor_id = atoi(PQgetvalue(res,rowi,0));
    strcpy( dsproveedor->nombre_proveedor ,rtrim(PQgetvalue(res,rowi,1),' '));
    strcpy( dsproveedor->contacto ,rtrim(PQgetvalue(res,rowi,2),' '));
    strcpy( dsproveedor->celu_prov,rtrim(PQgetvalue(res,rowi,3),' '));
    strcpy( dsproveedor->fijo_prov , rtrim(PQgetvalue(res,rowi,4),' '));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_proveedor
void fillObjProveedor(obj_proveedor *p,  t_proveedor rwitm)
{  
      p->proveedor_id = rwitm.proveedor_id;
      strcpy( p->nombre_proveedor,rwitm.nombre_proveedor);
      strcpy( p->celu_prov,rwitm.celu_prov);
	  strcpy( p->contacto,rwitm.contacto);
      strcpy( p->fijo_prov , rwitm.fijo_prov);
	  p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_proveedor
void fillRowsProveedor(void **list, int size,void *data)
{
     int i;
     obj_proveedor *d;
     // pedir memoria para el listado en memoria de proveedores obj_proveedor
    *list = (obj_proveedor **)malloc(sizeof(obj_proveedor*)* size);
    for(i=0;i<size;++i)
    {
      d = proveedor_new();
      fillObjProveedor(d,((t_proveedor *)data)[i]);
      ((obj_proveedor **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
int exec_get_proveedor(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_proveedor),fillProveedorFromDB);
}
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset
void fill_dataset_proveedor(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(sizeof(t_proveedor)* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     {
      ((t_proveedor *)ds->rows)[i].proveedor_id =((t_proveedor *)data)[i].proveedor_id;
      strcpy( ((t_proveedor *)ds->rows)[i].nombre_proveedor,((t_proveedor *)data)[i].nombre_proveedor);
	  strcpy( ((t_proveedor *)ds->rows)[i].celu_prov,((t_proveedor *)data)[i].celu_prov);
      strcpy( ((t_proveedor *)ds->rows)[i].contacto,((t_proveedor *)data)[i].contacto);
      strcpy( ((t_proveedor *)ds->rows)[i].fijo_prov , ((t_proveedor *)data)[i].fijo_prov);
     }
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
int findAll_proveedorImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, campos, tabla, sizeof(t_proveedor), ((data_set_proveedor*)((obj_proveedor*)self)->ds)->rows,fillProveedorFromDB,fillRowsProveedor);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_proveedor *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para proveedor
int find_proveedorImpl(void *self, int k)
{
   int size=0; void *data;  
   obj_proveedor *obj = this(self);
   char where[MAX_WHERE_SQL],sql[MAX_SQL];
   strcpy(where,getKey(k,where));
   strcpy(sql ,getFindSQL(campos, tabla, where));
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((data_set_proveedor*)obj->ds)->rows;
   size = exec_get_proveedor(sql,&data);
   fill_dataset_proveedor(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjProveedor(obj,((t_proveedor *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
int saveObj_proveedorImpl(void *self)
{
   PGconn *conn;
   PGresult *res; 
   int code=0;
   char values[MAX_WHERE_SQL];  
   char where[MAX_WHERE_SQL];
   char sql[MAX_SQL];
   int newIdProveedor;
   obj_proveedor *obj = this(self);   
    //Conexion con la base de datos 
   if ((conn = connectdb(SERVER,PORT,DB,USER,PASS))== NULL) {
      exit (-1); 
   }
   //Si esta marcado como nuevo objeto, es decir se creo instancia y no fue obtenido de la BD,
   if(obj->getIsNewObj(obj))
   {// insert    
	res = PQexec(conn, strSQL_SELECT_MAX_ID);
	code = PQresultStatus(res);
	newIdProveedor = atoi(PQgetvalue(res,0,0));	
	PQclear(res);	
	obj->proveedor_id =++newIdProveedor;
    sprintf(values, strSQL_INSERT_Param,obj->proveedor_id, obj->nombre_proveedor,obj->celu_prov,obj->contacto, obj->fijo_prov);
    snprintf( sql, MAX_SQL, strSQL_INSERT, values);
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);
	obj->isNewObj=false;// marcar que esta guardado en la BD.
  }
  else
  {// update
      sprintf(values, strSQL_UPDATE_Param, obj->nombre_proveedor, obj->celu_prov,obj->contacto, obj->fijo_prov);
      strcpy(where,getKey(obj->proveedor_id,where));
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
void showObj_proveedorImpl(void *self)
{
     obj_proveedor *p=this(self);
     printf("proveedor_id: %d  Nombre: %s Contacto: %s fijo_prov: %s  celu_prov: %s\n",p->proveedor_id,p->nombre_proveedor,p->contacto,p->fijo_prov,p->celu_prov);
}
//----------------------------------------------------
//implementacion de getters
int getProveedorId_Impl(void *self)
{ 
  obj_proveedor *obj = this(self);
  return obj->proveedor_id;
}
//----------------------------------------------------
char *getNombreProveedor_Impl(void *self)
{
	obj_proveedor *obj = this(self);
	return obj->nombre_proveedor;
}
//----------------------------------------------------
char *getContacto_Impl(void *self)
{
	obj_proveedor *obj = this(self);
	return obj->contacto;	
}
//----------------------------------------------------
char *getCeluProv_Impl(void *self)
{
	obj_proveedor *obj = this(self);
	return obj->celu_prov;	
}
//----------------------------------------------------
char *getFijoProv_Impl(void *self)
{
	obj_proveedor *obj = this(self);
	return obj->fijo_prov;	
}
//----------------------------------------------------
//implementacion setters
void setNombreProveedor_Impl(void *self,char *fechanac)
{
	obj_proveedor *obj = this(self);	
	strcpy(obj->nombre_proveedor,fechanac);
}
//----------------------------------------------------
void setContacto_Impl(void *self,char *contacto)
{ 
	obj_proveedor *obj = this(self);
	strcpy(obj->contacto,contacto);
}
//----------------------------------------------------
void setCeluProv_Impl(void *self,char *celu_prov)
{ 
	obj_proveedor *obj = this(self);
	strcpy(obj->celu_prov,celu_prov);	
}
//----------------------------------------------------
void setFijoProv_Impl(void *self,char *fijo_prov)
{
	obj_proveedor *obj = this(self);
	strcpy(obj->fijo_prov , fijo_prov);	
}
//----------------------------------------------------
void *init_proveedor(void *self, data_set *ds)
{
  obj_proveedor *obj;
  obj = (obj_proveedor *)self; 
  obj->ds = ds;
  obj->isNewObj = true;//marcar como objeto nuevo, si se instancia
  // Inicializar handlers de getters y setters
  /// getters
  obj->getProveedorId = getProveedorId_Impl;
  obj->getNombreProveedor =   getNombreProveedor_Impl;
  obj->getContacto = 	   getContacto_Impl;
  obj->getCeluProv =   getCeluProv_Impl;
  obj->getFijoProv =   getFijoProv_Impl;
  /// setters
  obj->setNombreProveedor =   setNombreProveedor_Impl;
  obj->setContacto =     setContacto_Impl;
  obj->setCeluProv =   setCeluProv_Impl;
  obj->setFijoProv =   setFijoProv_Impl;
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_proveedorImpl;
  obj->findAll =   findAll_proveedorImpl;
  obj->saveObj =   saveObj_proveedorImpl; 
  obj->showObj =   showObj_proveedorImpl;
  //relaciones
  
  return obj;
}
//----------------------------------------------------
obj_proveedor *proveedor_new()
{
  return (obj_proveedor *)init_obj(sizeof(obj_proveedor),sizeof(t_proveedor), col_proveedor, CNT_COL_PROV, init_proveedor);
}
