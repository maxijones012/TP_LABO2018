#ifndef EMP_INCLUDED
#define EMP_INCLUDED

#define CNT_COL_EMP 6
#define MAX_KEY 40
   
typedef struct {
    int empleado_id;
    char fecha_nac[MAXFECHA];
    char apellido[MAX];
    char nombre[MAX];
    int reporta_a;
    int extension;
} t_empleado;
//----------------------------------------------------
typedef struct {
    char **columns;
    int cntCol;
    int cntRows; 
    t_empleado *rows;  
} data_set_empleado;
//----------------------------------------------------
typedef struct {
    data_set *ds;
    int (*findAll)(void *self, void **list,char *criteria); 
    void (*showObj)(void *self);
	bool (*getIsNewObj)(void *self);
    int (*saveObj)(void *self);
	void (*destroyInternal)(void *self);
    int (*findbykey)(void *self,int empleado_id);

    //-- getters
    int (*getEmpleadoId)(void *self);
    char *(*getFechaNac)(void *self);
    char *(*getNombre)(void *self);
    char *(*getApellido)(void *self);
    int (*getReportaA)(void *self);
    int (*getExtension)(void *self);
    //-- setters
    void (*setFechaNac)(void *self,char *fechanac);
    void (*setNombre)(void *self,char *nombre);
    void (*setApellido)(void *self,char *apellido);
    void (*setReportaA)(void *self,int reporta_a);
    void (*setExtension)(void *self,int extension);
    /*implementacion de relaciones*/
	void *(*get_Supervisor)(void *self);
	char *(*apellido_nombre)(void*self);
    int empleado_id;
    char fecha_nac[MAXFECHA];    
    char nombre[MAX];
    char apellido[MAX];
    int reporta_a;
    int extension;
    bool isNewObj;
}obj_empleado;

// funcionalidad publica que se implementa en empleado.c
extern obj_empleado *empleado_new();
#endif
