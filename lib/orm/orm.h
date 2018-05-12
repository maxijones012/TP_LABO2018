//----------------------------------------------------
// dataset generico
typedef struct { char **columns;int cntCol;int cntRows; void *rows;} data_set;
#define NULL_VALUE -100
//----------------------------------------------------
// Estructura de pseudoobjeto genérico: Object 
typedef struct 
{
 data_set *ds;
 int  (*findAll)(void *self, void **list,char *criteria);
 void (*showObj)(void *self);
 bool (*getIsNewObj)(void *self);
 int  (*saveObj)(void *self);
 void (*destroyInternal)(void *self);
}t_object;
//----------------------------------------------------
// IMPLEMENTACION PARA DAR SOPORTE A COMPORTAMIENTO GENERICO - ABSTRACT.
// Implementacion en  "orm.c"
int exec_get_fromDB(char *,void **, size_t, void(*)(void **, int, PGresult *));
int findAllImpl(void *,void **, char *, const char *, const char *, size_t, void *,void(*)(void **,int, PGresult *),void(*)(void **, int, void *));
char *getFindSQL(const char *, const char *, char *);
void *init_obj(size_t, size_t, const char **, int, void*(*)(void *, data_set *));
void listObj(void *objList, char *criteria, int freeObj,void(*lstFunc)(void *o));
void destroyObj(void *);
void destroyObjList(void **,int);
// Implementacion en "utils.c"
char *getFecha();
char *getFechaHora();
