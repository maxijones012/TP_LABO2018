#ifndef CAT_INCLUDED
#define CAT_INCLUDED

#define CNT_COL_CAT 2
#define MAX_KEY 40
   
typedef struct {
    int categoria_id;
    char nombre_categoria[MAX];
} t_categoria;
//----------------------------------------------------
typedef struct {
    char **columns;
    int cntCol;
    int cntRows; 
    t_categoria *rows;  
} data_set_categoria;
//----------------------------------------------------
typedef struct {
    data_set *ds;
    int (*findAll)(void *self, void **list,char *criteria); 
    void (*showObj)(void *self);
	bool (*getIsNewObj)(void *self);
    int (*saveObj)(void *self);
	void (*destroyInternal)(void *self);
    int (*findbykey)(void *self,int categoria_id);
    //-- getters
    int (*getCategoriaId)(void *self);
    char *(*getNombreCategoria)(void *self);
    //-- setters
    void (*setNombreCategoria)(void *self,char *NombreCategoria);
    int categoria_id;
    char nombre_categoria[MAX];
    bool isNewObj;
}obj_categoria;

// funcionalidad publica que se implementa en categoria.c
extern obj_categoria *categoria_new ();
#endif
