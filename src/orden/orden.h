#ifndef ORD_INCLUDED
#define ORD_INCLUDED

#define CNT_COL_ORD 5
#define MAX_KEY 40
   
typedef struct {
    int orden_id;
    int empleado_id;
    int cliente_id;
    char fecha_orden[MAX12];
    int descuento;
} t_orden;
//----------------------------------------------------
typedef struct {
    char **columns;
    int cntCol;
    int cntRows; 
    t_orden *rows;  
} data_set_orden;
//----------------------------------------------------
typedef struct {
    data_set *ds;
    int (*findAll)(void *self, void **list,char *criteria); 
    void (*showObj)(void *self);
	bool (*getIsNewObj)(void *self);
    int (*saveObj)(void *self);
	void (*destroyInternal)(void *self);
    int (*findbykey)(void *self,int orden_id);

    //-- getters
    int (*getOrdenId)(void *self);
    int (*getEmpleadoId)(void *self);
    int (*getClienteId)(void *self);
    char *(*getFechaOrden)(void *self);
    int (*getDescuento)(void *self);
    //-- setters
    void (*setEmpleadoId)(void *self,int orden_id);
    void (*setClienteId)(void *self,int cliente_id);
    void (*setFechaOrden)(void *self,char *fecha_orden);
    void (*setDescuento)(void *self,int descuento);
    /*implementacion de relaciones*/
	void *(*getEmpleadoObj)(void *self);
    int orden_id;
    int empleado_id;
    int cliente_id;
    char fecha_orden[MAX12];
    int descuento;
    bool isNewObj;
}obj_orden;

// funcionalidad publica que se implementa en orden.c
extern obj_orden *orden_new();
#endif
