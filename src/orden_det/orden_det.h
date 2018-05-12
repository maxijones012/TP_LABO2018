#ifndef ORDDET_INCLUDED
#define ORDDET_INCLUDED

#define CNT_COL_ORDDET 4
#define MAX_KEY 40
   
typedef struct {
    int orden_id;
    int detalle_id;
    int producto_id;
    int cantidad;
} t_orden_det;
//----------------------------------------------------
typedef struct {
    char **columns;
    int cntCol;
    int cntRows; 
    t_orden_det *rows;  
} data_set_orden_det;
//----------------------------------------------------
typedef struct {
    data_set *ds;
    int (*findAll)(void *self, void **list,char *criteria); 
    void (*showObj)(void *self);
	bool (*getIsNewObj)(void *self);
    int (*saveObj)(void *self);
	void (*destroyInternal)(void *self);
    int (*findbykey)(void *self,int orden_id,int detalle_id);

    //-- getters
    int (*getOrdenId)(void *self);
    int (*getDetalleId)(void *self);
    int (*getProductoId)(void *self);
    int (*getCantidad)(void *self);
    //-- setters
    void (*setDetalleId)(void *self,int orden_id);
    void (*setProductoId)(void *self,int producto_id);
    void (*setCantidad)(void *self,int cantidad);
    /*implementacion de relaciones*/
    int orden_id;
    int detalle_id;
    int producto_id;    
    int cantidad;
    bool isNewObj;
}obj_orden_det;

// funcionalidad publica que se implementa en orden.c
extern obj_orden_det *orden_det_new();
#endif
