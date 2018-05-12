#ifndef PROD_INCLUDED
#define PROD_INCLUDED

#define CNT_COL_PROD 6
#define MAX_KEY 40
   
typedef struct {
    int producto_id;
    int proveedor_id;
    int categoria_id;
    char descripcion[MAX50];
    float precio_unit;
    int existencia;
} t_producto;
//----------------------------------------------------
typedef struct {
    char **columns;
    int cntCol;
    int cntRows; 
    t_producto *rows;  
} data_set_producto;
//----------------------------------------------------
typedef struct {
    data_set *ds;
    int (*findAll)(void *self, void **list,char *criteria); 
    void (*showObj)(void *self);
	bool (*getIsNewObj)(void *self);
    int (*saveObj)(void *self);
	void (*destroyInternal)(void *self);
    int (*findbykey)(void *self,int producto_id);

    //-- getters
    int (*getProductoId)(void *self);
    int (*getProveedorId)(void *self);
    int (*getCategoriaId)(void *self);
    char *(*getDescripcion)(void *self);
    float (*getPrecioUnit)(void *self);
    int (*getExistencia)(void *self);
    //-- setters
    void (*setProveedorId)(void *self,int producto_id);
    void (*setCategoriaId)(void *self,int categoria_id);
    void (*setDescripcion)(void *self,char *descripcion);
    void (*setPrecioUnit)(void *self,float precio_unit);
    void (*setExistencia)(void *self,int existencia);
    void *(*getCategoriaObj)(void *self);
    /*implementacion de relaciones*/
    int producto_id;
    int proveedor_id;
    int categoria_id;
    char descripcion[MAX50];
    float precio_unit;
    int existencia;
    bool isNewObj;
}obj_producto;

// funcionalidad publica que se implementa en producto.c
extern obj_producto *producto_new();
#endif
