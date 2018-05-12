#ifndef PROV_INCLUDED
#define PROV_INCLUDED

#define CNT_COL_PROV 6
#define MAX_KEY 40
   
typedef struct {
    int proveedor_id;
    char nombre_proveedor[MAX50];    
    char contacto[MAX50];
    char celu_prov[MAX12];
    char fijo_prov[MAX12];
} t_proveedor;
//----------------------------------------------------
typedef struct {
    char **columns;
    int cntCol;
    int cntRows; 
    t_proveedor *rows;  
} data_set_proveedor;
//----------------------------------------------------
typedef struct {
    data_set *ds;
    int  (*findAll)(void *self, void **list,char *criteria); 
    void (*showObj)(void *self);
	bool (*getIsNewObj)(void *self);
    int  (*saveObj)(void *self);
	void (*destroyInternal)(void *self);
    int  (*findbykey)(void *self,int proveedor_id);
    //-- getters
    int (*getProveedorId)(void *self);
    char *(*getNombreProveedor)(void *self);
    char *(*getContacto)(void *self);
    char *(*getCeluProv)(void *self);
    char *(*getFijoProv)(void *self);
	
    //-- setters
    void (*setNombreProveedor)(void *self,char *nombre_proveedor);
    void (*setContacto)(void *self,char *contacto);
    void (*setCeluProv)(void *self,char *celu_prov);
    void (*setFijoProv)(void *self,char *fijo_prov);    
    /*implementacion de relaciones*/
	int proveedor_id;
    char nombre_proveedor[MAX50];    
    char contacto[MAX50];
    char celu_prov[MAX12];
    char fijo_prov[MAX12];
    bool isNewObj;
}obj_proveedor;

// funcionalidad publica que se implementa en proveedor.c
extern obj_proveedor *proveedor_new();
#endif
