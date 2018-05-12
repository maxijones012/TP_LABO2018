#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "cliente.h"

//columnas de la tabla: clientes
static const char *campos = "cliente_id,cedula_ruc,nombre_cia,nombre_contacto,dir_cliente,fax,email,celular,fijo";
static const char *tabla = "clientes";
static const char *col_clientes[]={"cliente_id","cedula_ruc","nombre_cia","nombre_contacto","dir_cliente","fax","email","celular","fijo"};

static const char *strSQL_SELECT_MAX_ID = "select max(cliente_id) from clientes;";
static const char *strSQL_KEY = "cliente_id = %d";
static const char *strSQL_INSERT = "insert into clientes(cedula_ruc,nombre_contacto,nombre_cia,dir_cliente,fax,email,celular,fijo)values(%s);";
static const char *strSQL_INSERT_Param = "'%s','%s','%s','%s','%s','%s','%s','%s'";
static const char *strSQL_UPDATE = "update clientes set %s where %s;";
static const char *strSQL_UPDATE_Param ="cedula_ruc = '%s', nombre_contacto ='%s', nombre_cia = '%s', dir_cliente='%s', fax='%s'";

//----------------------------------------------------
//Obtener la cadena SQL para con uso de la clave
static char *getKey(int k,char *where)
{
   snprintf( where, MAX_SQL, strSQL_KEY ,k);
   return  where;
}
//----------------------------------------------------
static obj_cliente *this(void *self)
{
   return (obj_cliente*)self;
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
void fillClienteFromDB(void **rw , int rowi,PGresult *res)
{
    t_cliente *dsclientes;
    dsclientes = &(( (t_cliente *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD
	dsclientes->cliente_id = atoi(PQgetvalue(res,rowi,0));
    strcpy( dsclientes->cedula_ruc ,rtrim(PQgetvalue(res,rowi,1),' '));
    strcpy( dsclientes->nombre_cia ,rtrim(PQgetvalue(res,rowi,2),' '));
    strcpy( dsclientes->nombre_contacto,rtrim(PQgetvalue(res,rowi,3),' '));
	strcpy( dsclientes->dir_cliente,rtrim(PQgetvalue(res,rowi,4),' '));
    strcpy( dsclientes->fax,rtrim(PQgetvalue(res,rowi,5),' '));
	strcpy( dsclientes->email,rtrim(PQgetvalue(res,rowi,6),' '));
    strcpy( dsclientes->celular,rtrim(PQgetvalue(res,rowi,7),' '));
	strcpy( dsclientes->fijo,rtrim(PQgetvalue(res,rowi,8),' '));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_cliente
void fillObjCliente(obj_cliente *p,  t_cliente rwitm)
{  
      p->cliente_id = rwitm.cliente_id;
      strcpy( p->cedula_ruc,rwitm.cedula_ruc);
      strcpy( p->nombre_contacto,rwitm.nombre_contacto);
	  strcpy( p->nombre_cia,rwitm.nombre_cia);
	  strcpy( p->dir_cliente,rwitm.dir_cliente);
      strcpy( p->fax,rwitm.fax);
	  strcpy( p->email,rwitm.email);
	  strcpy( p->celular,rwitm.celular);
	  strcpy( p->fijo,rwitm.fijo);      
	  p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_cliente
void fillRowsCliente(void **list, int size,void *data)
{
     int i;
     obj_cliente *d;
     // pedir memoria para el listado en memoria de clientes obj_cliente
    *list = (obj_cliente **)malloc(sizeof(obj_cliente*)* size);
    for(i=0;i<size;++i)
    {
      d = cliente_new();
      fillObjCliente(d,((t_cliente *)data)[i]);
      ((obj_cliente **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
int exec_get_cliente(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_cliente),fillClienteFromDB);
}
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset
void fill_dataset_cliente(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(sizeof(t_cliente)* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     {
      ((t_cliente *)ds->rows)[i].cliente_id =((t_cliente *)data)[i].cliente_id;
      strcpy( ((t_cliente *)ds->rows)[i].cedula_ruc,((t_cliente *)data)[i].cedula_ruc);
	  strcpy( ((t_cliente *)ds->rows)[i].nombre_contacto,((t_cliente *)data)[i].nombre_contacto);
      strcpy( ((t_cliente *)ds->rows)[i].nombre_cia,((t_cliente *)data)[i].nombre_cia);
	  strcpy( ((t_cliente *)ds->rows)[i].dir_cliente,((t_cliente *)data)[i].dir_cliente);
	  strcpy( ((t_cliente *)ds->rows)[i].fax,((t_cliente *)data)[i].fax);
	  strcpy( ((t_cliente *)ds->rows)[i].email,((t_cliente *)data)[i].email);
	  strcpy( ((t_cliente *)ds->rows)[i].celular,((t_cliente *)data)[i].celular);
	  strcpy( ((t_cliente *)ds->rows)[i].fijo,((t_cliente *)data)[i].fijo);	  
     }
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
int findAll_clientesImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, campos, tabla, sizeof(t_cliente), ((data_set_cliente*)((obj_cliente*)self)->ds)->rows,fillClienteFromDB,fillRowsCliente);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_cliente *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para cliente
int find_clientesImpl(void *self, int k)
{
   int size=0; void *data;  
   obj_cliente *obj = this(self);
   char where[MAX_WHERE_SQL],sql[MAX_SQL];
   strcpy(where,getKey(k,where));
   strcpy(sql ,getFindSQL(campos, tabla, where));
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((data_set_cliente*)obj->ds)->rows;
   size = exec_get_cliente(sql,&data);
   fill_dataset_cliente(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjCliente(obj,((t_cliente *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
int saveObj_clientesImpl(void *self)
{
   PGconn *conn;
   PGresult *res; 
   int code=0;
   char values[MAX_WHERE_SQL];  
   char where[MAX_WHERE_SQL];
   char sql[MAX_SQL];
   obj_cliente *obj = this(self);
    //Conexion con la base de datos 
   if ((conn = connectdb(SERVER,PORT,DB,USER,PASS))== NULL) {
      exit (-1); 
   }
   //Si esta marcado como nuevo objeto, es decir se creo instancia y no fue obtenido de la BD,
   if(obj->getIsNewObj(obj))
   {// insert    
    sprintf(values, strSQL_INSERT_Param, obj->cedula_ruc,obj->nombre_contacto,obj->nombre_cia, obj->dir_cliente, obj->fax);
    snprintf( sql, MAX_SQL, strSQL_INSERT, values);
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);
	// al tener clave serial en la bd, se autonumera, obtener el id generado.
	if ( code == PGRES_COMMAND_OK)  {
		res = PQexec(conn, strSQL_SELECT_MAX_ID);
		code = PQresultStatus(res);
		obj->cliente_id = atoi(PQgetvalue(res,0,0));
		obj->isNewObj=false;// marcar que esta guardado en la BD.
		PQclear(res);
	}
  }
  else
  {// update
      sprintf(values, strSQL_UPDATE_Param, obj->cedula_ruc, obj->nombre_contacto,obj->nombre_cia, obj->dir_cliente, obj->fax);
      strcpy(where,getKey(obj->cliente_id,where));
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
void showObj_clientesImpl(void *self)
{
     obj_cliente *self_o=this(self);
   printf("cliente_id: %d  Nombre cia:%s dir_cliente: %s\n",self_o->cliente_id,self_o->nombre_cia,self_o->dir_cliente);
}
//----------------------------------------------------
//implementacion de getters
int getClienteId_Impl(void *self)
{ 
  obj_cliente *obj = this(self);
  return obj->cliente_id;
}
//----------------------------------------------------
char *getCedulaRuc_Impl(void *self)
{
	obj_cliente *obj = this(self);
	return obj->cedula_ruc;
}
//----------------------------------------------------
char *getNombreCia_Impl(void *self)
{
	obj_cliente *obj = this(self);
	return obj->nombre_cia;	
}
//----------------------------------------------------
char *getNombreCont_Impl(void *self)
{
	obj_cliente *obj = this(self);
	return obj->nombre_contacto;	
}
//----------------------------------------------------
char *getDirCliente_Impl(void *self)
{
	obj_cliente *obj = this(self);
	return obj->dir_cliente;	
}
//----------------------------------------------------
char *getFax_Impl(void *self)
{
	obj_cliente *obj = this(self);
	return obj->fax;	
}
//----------------------------------------------------
char *getEmail_Impl(void *self)
{
	obj_cliente *obj = this(self);
	return obj->email;	
}
//----------------------------------------------------
char *getCelular_Impl(void *self)
{
	obj_cliente *obj = this(self);
	return obj->celular;	
}
//----------------------------------------------------
char *getFijo_Impl(void *self)
{
	obj_cliente *obj = this(self);
	return obj->fijo;	
}
//----------------------------------------------------
//implementacion setters
void setCedulaRuc_Impl(void *self,char *cedula_ruc)
{
	obj_cliente *obj = this(self);	
	strcpy(obj->cedula_ruc,cedula_ruc);
}
//----------------------------------------------------
void setNombreCia_Impl(void *self,char *nombre_cia)
{ 
	obj_cliente *obj = this(self);
	strcpy(obj->nombre_cia,nombre_cia);
}
//----------------------------------------------------
void setNombreCont_Impl(void *self,char *nombre_contacto)
{ 
	obj_cliente *obj = this(self);
	strcpy(obj->nombre_contacto,nombre_contacto);	
}
//----------------------------------------------------
void setDirCliente_Impl(void *self,char *dir_cliente)
{
	obj_cliente *obj = this(self);
	strcpy(obj->dir_cliente,dir_cliente);
}
//----------------------------------------------------
void setFax_Impl(void *self,char *fax)
{ 
	obj_cliente *obj = this(self);
	strcpy(obj->fax,fax);
}
//----------------------------------------------------
void setEmail_Impl(void *self,char *email)
{ 
	obj_cliente *obj = this(self);
	strcpy(obj->email,email);
}
//----------------------------------------------------
void setCelular_Impl(void *self,char *celular)
{ 
	obj_cliente *obj = this(self);
	strcpy(obj->celular,celular);
}
//----------------------------------------------------
void setFijo_Impl(void *self,char *fijo)
{ 
	obj_cliente *obj = this(self);
	strcpy(obj->fijo,fijo);
}
//----------------------------------------------------
void *init_cliente(void *self, data_set *ds)
{
  obj_cliente *obj;
  obj = (obj_cliente *)self; 
  obj->ds = ds;
  obj->isNewObj = true;//marcar como objeto nuevo, si se instancia
  // Inicializar handlers de getters y setters
  /// getters
  obj->getClienteId  =  getClienteId_Impl;
  obj->getCedulaRuc  =  getCedulaRuc_Impl;
  obj->getNombreCia  =  getNombreCia_Impl;
  obj->getNombreCont =  getNombreCont_Impl;
  obj->getDirCliente =  getDirCliente_Impl;
  obj->getFax 		 =  getFax_Impl;
  obj->getEmail 	 =  getEmail_Impl;
  obj->getCelular 	 =  getCelular_Impl;
  obj->getFijo	 	 =  getFijo_Impl;
  /// setters
  obj->setCedulaRuc =  setCedulaRuc_Impl;
  obj->setNombreCia =  setNombreCia_Impl;
  obj->setNombreCont =   setNombreCont_Impl;
  obj->setDirCliente =   setDirCliente_Impl;
  obj->setFax =  setFax_Impl;
  obj->setEmail =  setEmail_Impl;
  obj->setCelular =  setCelular_Impl;
  obj->setFijo =  setFijo_Impl;
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_clientesImpl;
  obj->findAll =   findAll_clientesImpl;
  obj->saveObj =   saveObj_clientesImpl; 
  obj->showObj =   showObj_clientesImpl;
  
  //relaciones
  
  return obj;
}
//----------------------------------------------------
obj_cliente *cliente_new()
{
  return (obj_cliente *)init_obj(sizeof(obj_cliente),sizeof(t_cliente), col_clientes, CNT_COL_CLI, init_cliente);
}
