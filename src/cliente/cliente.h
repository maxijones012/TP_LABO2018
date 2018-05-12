#ifndef CLI_INCLUDED
#define CLI_INCLUDED

#define CNT_COL_CLI 9
#define MAX_KEY 40
   
typedef struct {
    int  cliente_id;
    char cedula_ruc[MAX10];
    char nombre_cia[MAX30];
    char nombre_contacto[MAX50];
    char dir_cliente[MAX50];
    char fax[MAX12];
	char email[MAX50];
	char celular[MAX12];
	char fijo[MAX12];
} t_cliente;
//----------------------------------------------------
typedef struct {
    char **columns;
    int cntCol;
    int cntRows; 
    t_cliente *rows;  
} data_set_cliente;
//----------------------------------------------------
typedef struct {
    data_set *ds;
    int (*findAll)(void *self, void **list,char *criteria); 
	void (*showObj)(void *self);
	int (*saveObj)(void *self);
	void (*destroyInternal)(void *self);
    int (*findbykey)(void *self,int cliente_id);
    //-- getters
    int (*getClienteId)(void *self);
    char *(*getCedulaRuc)(void *self);
    char *(*getNombreCia)(void *self);
    char *(*getNombreCont)(void *self);
	char *(*getDirCliente)(void *self);
	char *(*getFax)(void *self);
	char *(*getEmail)(void *self);
	char *(*getCelular)(void *self);
	char *(*getFijo)(void *self);
	bool (*getIsNewObj)(void *self);
    //-- setters
    void (*setCedulaRuc)(void *self,char *cedula_ruc);
    void (*setNombreCia)(void *self,char *nombre_cia);
	void (*setNombreCont)(void *self,char *nombre_contacto);
	void (*setDirCliente)(void *self,char *dir_cliente);
	void (*setFax)(void *self,char *fax);
	void (*setEmail)(void *self,char *email);
	void (*setCelular)(void *self,char *celular);
	void (*setFijo)(void *self,char *fijo);
    /*implementacion de relaciones*/	
    int  cliente_id;
    char cedula_ruc[MAX10];
    char nombre_cia[MAX30];
    char nombre_contacto[MAX50];
    char dir_cliente[MAX50];
    char fax[MAX12];
	char email[MAX50];
	char celular[MAX12];
	char fijo[MAX12];
    bool isNewObj;
}obj_cliente;

// funcionalidad publica que se implementa en cliente.c
extern obj_cliente *cliente_new();
#endif
