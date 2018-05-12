#include <string.h>
#include <errno.h>

#include "../../config/config.h"
#include "../../lib/utils/utils.h"
#include "../../lib/orm/orm.h"
#include "empleado.h"

//columnas de la tabla: empleado
static const char *campos = "empleado_id,fecha_nac,nombre,apellido,reporta_a,extension";
static const char *tabla = "empleados";
static const char *col_empleado[]={"empleado_id","fecha_nac","nombre","apellido","reporta_a","extension"};

static const char *strSQL_SELECT_MAX_ID = "select max(empleado_id) from empleados;";
static const char *strSQL_KEY = "empleado_id = %d";
static const char *strSQL_INSERT = "insert into empleados(fecha_nac,apellido,nombre,reporta_a,extension)values(%s);";
static const char *strSQL_INSERT_Param = "'%s','%s','%s',%d,%d";
static const char *strSQL_INSERT_Param_NULL_Reporta_a = "'%s','%s','%s',null,%d";
static const char *strSQL_UPDATE = "update empleados set %s where %s;";
static const char *strSQL_UPDATE_Param ="fecha_nac = '%s', apellido ='%s', nombre = '%s', reporta_a=%d, extension=%d";
static const char *strSQL_UPDATE_Param_NULL_Reporta_a ="fecha_nac = '%s', apellido ='%s',nombre = '%s',reporta_a=null, extension=%d";

//----------------------------------------------------
//Obtener la cadena SQL para con uso de la clave
static char *getKey(int k,char *where)
{
   snprintf( where, MAX_SQL, strSQL_KEY ,k);
   return  where;
}
//----------------------------------------------------
static obj_empleado *this(void *self)
{
   return (obj_empleado*)self;
}
//----------------------------------------------------
//Copiar a puntero de arreglo en posicion dada, desde un result set de base de datos.
void fillEmpleadoFromDB(void **rw , int rowi,PGresult *res)
{
    t_empleado *dsempleado;
    dsempleado = &(( (t_empleado *) *rw)[rowi]);
    //leer valor desde estructura obtenida de la BD
	dsempleado->empleado_id = atoi(PQgetvalue(res,rowi,0));
    strcpy( dsempleado->fecha_nac ,rtrim(PQgetvalue(res,rowi,1),' '));
    strcpy( dsempleado->nombre ,rtrim(PQgetvalue(res,rowi,2),' '));
    strcpy( dsempleado->apellido,rtrim(PQgetvalue(res,rowi,3),' '));
    dsempleado->reporta_a = atoi(PQgetvalue(res,rowi,4));
    dsempleado->extension = atoi(PQgetvalue(res,rowi,5));
}       
//----------------------------------------------------
//Copiar datos desde una variable de struct => puntero a obj_empleado
void fillObjEmpleado(obj_empleado *p,  t_empleado rwitm)
{  
      p->empleado_id = rwitm.empleado_id;
      strcpy( p->fecha_nac,rwitm.fecha_nac);
      strcpy( p->apellido,rwitm.apellido);
	  strcpy( p->nombre,rwitm.nombre);
      p->reporta_a = rwitm.reporta_a;
      p->extension = rwitm.extension;
	  p->isNewObj = false; // marcar que ya existe correspondencia en la base de datos en saveObj
}
//----------------------------------------------------
//Copiar en arreglo dinamico de tipo obj_empleado
void fillRowsEmpleado(void **list, int size,void *data)
{
     int i;
     obj_empleado *d;
     // pedir memoria para el listado en memoria de empleados obj_empleado
    *list = (obj_empleado **)malloc(sizeof(obj_empleado*)* size);
    for(i=0;i<size;++i)
    {
      d = empleado_new();
      fillObjEmpleado(d,((t_empleado *)data)[i]);
      ((obj_empleado **)*list)[i] = d;
    }
 }
//----------------------------------------------------
//ejecutar consulta SQL en la base y obtener result set para cargar en memoria, invocacion personalizada a un codigo generico.
int exec_get_empleado(char *sql,void **rw)
{
  return exec_get_fromDB(sql,rw, sizeof(t_empleado),fillEmpleadoFromDB);
}
//----------------------------------------------------
//Copiar las tuplas a una instancia de dataset
void fill_dataset_empleado(data_set *ds,void *data, int sz)
{
 int i;
 ds->rows = malloc(sizeof(t_empleado)* sz);
 ds->cntRows=sz;
     for(i=0;i<sz;++i)
     {
      ((t_empleado *)ds->rows)[i].empleado_id =((t_empleado *)data)[i].empleado_id;
      strcpy( ((t_empleado *)ds->rows)[i].fecha_nac,((t_empleado *)data)[i].fecha_nac);
	  strcpy( ((t_empleado *)ds->rows)[i].apellido,((t_empleado *)data)[i].apellido);
      strcpy( ((t_empleado *)ds->rows)[i].nombre,((t_empleado *)data)[i].nombre);
       ((t_empleado *)ds->rows)[i].reporta_a = ((t_empleado *)data)[i].reporta_a;
       ((t_empleado *)ds->rows)[i].extension = ((t_empleado *)data)[i].extension;
     }
}
//----------------------------------------------------
// implementacion para copiar toda la informacion segun un criterio ejecutado en la base de datos
int findAll_empleadoImpl(void *self,void **list, char *criteria)
{
  return findAllImpl(self,list, criteria, campos, tabla, sizeof(t_empleado), ((data_set_empleado*)((obj_empleado*)self)->ds)->rows,fillEmpleadoFromDB,fillRowsEmpleado);
}
//----------------------------------------------------
static bool getIsNewObj_Impl(void *self)
{
	obj_empleado *obj = this(self);
	return obj->isNewObj;
}
//----------------------------------------------------
// implementacion de metodos para empleado
int find_empleadoImpl(void *self, int k)
{
   int size=0; void *data;  
   obj_empleado *obj = this(self);
   char where[MAX_WHERE_SQL],sql[MAX_SQL];
   strcpy(where,getKey(k,where));
   strcpy(sql ,getFindSQL(campos, tabla, where));
   //ejecutar consulta sql de seleccion, con criterio where
   data = ((data_set_empleado*)obj->ds)->rows;
   size = exec_get_empleado(sql,&data);
   fill_dataset_empleado(obj->ds,data,size);
   // setear datos a la instancia....
   if(size>0)
   	 fillObjEmpleado(obj,((t_empleado *)data)[0]);
   else
     size = -1;   
   return size;
}
//----------------------------------------------------
int saveObj_empleadoImpl(void *self)
{
   PGconn *conn;
   PGresult *res; 
   int code=0;
   char values[MAX_WHERE_SQL];  
   char where[MAX_WHERE_SQL];
   char sql[MAX_SQL];
   obj_empleado *obj = this(self);
    //Conexion con la base de datos 
   if ((conn = connectdb(SERVER,PORT,DB,USER,PASS))== NULL) {
      exit (-1); 
   }
   //Si esta marcado como nuevo objeto, es decir se creo instancia y no fue obtenido de la BD,
   if(obj->getIsNewObj(obj))
   {// insert    
    if(obj->reporta_a==0 || obj->reporta_a==NULL_VALUE ) // ver si inserta null en reporta_a
		sprintf(values, strSQL_INSERT_Param_NULL_Reporta_a, obj->fecha_nac,obj->apellido,obj->nombre, obj->extension);
	else
		sprintf(values, strSQL_INSERT_Param, obj->fecha_nac,obj->apellido,obj->nombre, obj->reporta_a, obj->extension);
    snprintf( sql, MAX_SQL, strSQL_INSERT, values);
    res = PQexec(conn, sql);
    code = PQresultStatus(res);
    PQclear(res);
	// al tener clave serial en la bd, se autonumera, obtener el id generado.
	if ( code == PGRES_COMMAND_OK)  {
		res = PQexec(conn, strSQL_SELECT_MAX_ID);
		code = PQresultStatus(res);
		obj->empleado_id = atoi(PQgetvalue(res,0,0));
		obj->isNewObj=false;// marcar que esta guardado en la BD.
		PQclear(res);
	}
  }
  else
  {// update
      if(obj->reporta_a==0 || obj->reporta_a==NULL_VALUE) // ver si inserta null en reporta_a
            sprintf(values, strSQL_UPDATE_Param_NULL_Reporta_a, obj->fecha_nac, obj->apellido,obj->nombre, obj->extension); 
      else
            sprintf(values, strSQL_UPDATE_Param, obj->fecha_nac, obj->apellido,obj->nombre, obj->reporta_a, obj->extension);
      strcpy(where,getKey(obj->empleado_id,where));
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
void showObj_empleadoImpl(void *self)
{
     obj_empleado *self_o=this(self);
     obj_empleado *sup;
     char supervisor[MAX];
     if(self_o->reporta_a>0)
     {
        sup = (obj_empleado*)self_o->get_Supervisor(self);
        strcpy(supervisor , sup->apellido_nombre(sup));
     }
     else
        strcpy(supervisor,"-sin supervisor-");
   printf("empleado_id: %d  Apellido y nombre:%s reporta_a: %s  extension: %d\n",self_o->empleado_id,self_o->apellido_nombre(self_o),supervisor,self_o->extension);
}
//----------------------------------------------------
void *getSupervisor_empleadoImpl(void *self)
{
	obj_empleado* o;
    obj_empleado *p;
    o = (obj_empleado*) self;   
    p = empleado_new();
    p->findbykey(p,o->reporta_a);     
    return p; 
}
//----------------------------------------------------
char *nombreyApellido_empleadoImpl(void *self)
{
    char *nomap;
    obj_empleado* o;
    o = (obj_empleado*) self;
    nomap = o->apellido;
    strcat(nomap  ,", ");
    strcat(nomap, o->nombre);
    return nomap;
}
//----------------------------------------------------
//implementacion de getters
int getEmpleadoId_Impl(void *self)
{ 
  obj_empleado *obj = this(self);
  return obj->empleado_id;
}
//----------------------------------------------------
char *getFechaNac_Impl(void *self)
{
	obj_empleado *obj = this(self);
	return obj->fecha_nac;
}
//----------------------------------------------------
char *getNombre_Impl(void *self)
{
	obj_empleado *obj = this(self);
	return obj->nombre;	
}
//----------------------------------------------------
char *getApellido_Impl(void *self)
{
	obj_empleado *obj = this(self);
	return obj->apellido;	
}
//----------------------------------------------------
int getReportaA_Impl(void *self)
{
	obj_empleado *obj = this(self);
	return obj->reporta_a;	
}
//----------------------------------------------------
int getExtension_Impl(void *self)
{
	obj_empleado *obj = this(self);
	return obj->extension;	
}
//----------------------------------------------------
//implementacion setters
void setFechaNac_Impl(void *self,char *fechanac)
{
	obj_empleado *obj = this(self);	
}
//----------------------------------------------------
void setNombre_Impl(void *self,char *nombre)
{ 
	obj_empleado *obj = this(self);
	strcpy(obj->nombre,nombre);
}
//----------------------------------------------------
void setApellido_Impl(void *self,char *apellido)
{ 
	obj_empleado *obj = this(self);
	strcpy(obj->apellido,apellido);	
}
//----------------------------------------------------
void setReportaA_Impl(void *self,int reporta_a)
{
	obj_empleado *obj = this(self);
	obj->reporta_a = reporta_a;	
}
//----------------------------------------------------
void setExtension_Impl(void *self,int extension)
{ 
	obj_empleado *obj = this(self);
	obj->extension = extension;	
}
//----------------------------------------------------
void *init_empleado(void *self, data_set *ds)
{
  obj_empleado *obj;
  obj = (obj_empleado *)self; 
  obj->ds = ds;
  obj->isNewObj = true;//marcar como objeto nuevo, si se instancia
  // inicializar algunos campos por default
  obj->reporta_a = NULL_VALUE;
  obj->extension = 0;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getEmpleadoId = getEmpleadoId_Impl;
  obj->getFechaNac =   getFechaNac_Impl;
  obj->getNombre = 	   getNombre_Impl;
  obj->getApellido =   getApellido_Impl;
  obj->getReportaA =   getReportaA_Impl;
  obj->getExtension =  getExtension_Impl;
  /// setters
  obj->setFechaNac =   setFechaNac_Impl;
  obj->setNombre =     setNombre_Impl;
  obj->setApellido =   setApellido_Impl;
  obj->setReportaA =   setReportaA_Impl;
  obj->setExtension =  setExtension_Impl;
  //incializacion de la interfaz de la entidad
  obj->getIsNewObj =   getIsNewObj_Impl;
  obj->findbykey = find_empleadoImpl;
  obj->findAll =   findAll_empleadoImpl;
  obj->saveObj =   saveObj_empleadoImpl; 
  obj->showObj =   showObj_empleadoImpl;
  obj->apellido_nombre = nombreyApellido_empleadoImpl;
  //relaciones
  obj->get_Supervisor = (void *)getSupervisor_empleadoImpl;
  return obj;
}
//----------------------------------------------------
obj_empleado *empleado_new()
{
  return (obj_empleado *)init_obj(sizeof(obj_empleado),sizeof(t_empleado), col_empleado, CNT_COL_EMP, init_empleado);
}
