#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h> 
#include <string.h>
#include <malloc.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "config/config.h"
#include "lib/orm/orm.h"
#include "lib/utils/utils.h"

#include "src/empleado/empleado.h"
#include "src/categoria/categoria.h"
#include "src/cliente/cliente.h"
#include "src/proveedor/proveedor.h"
#include "src/producto/producto.h"
#include "src/orden/orden.h"
#include "src/orden_det/orden_det.h"


/*Declaraciones*/
void ingresarEmpleado(int argc, char *argv[]);
void ingresarCategoria(int argc, char *argv[]);
//void mostrarArgumentos(int argc, char *argv[]){


void cli_show(void *c)
{
  	obj_cliente *cli = (obj_cliente*)c;
    	printf("ID:%d\n",cli->getClienteId(cli));
}



//--------------------------------------------------------------------
int main(int argc, char *argv[])
{
	obj_empleado *emp,*e_row;
	obj_categoria *cat,*c_row;
	obj_cliente *cli,*cli_row;
	obj_proveedor *prov  ,*pv_row;
	obj_producto *p,*p_rw;
	obj_orden *o;
	obj_orden_det *odet;
	  
  	int result;
  	int i=0,size=0;
  	void *list;
  
  	//mostrarArgumentos(argc, argv);
  
  
  	if  (!strcmp(argv[1],"-a")){
			if (!strcmp(argv[2],"-empleado")){
			ingresarEmpleado(argc, argv);		
		}

		if(!strcmp(argv[2],"-categoria")){
			ingresarCategoria(argc, argv);
		}
	}else{
		if(!strcmp(argv[1],"-l")){
			emp = empleado_new();
  			listObj(emp,NULL,true,NULL);
			
			listObj(prov,NULL,true,NULL);
			printf("\n----------------------------------\n");
			cat = categoria_new();
			listObj(cat,NULL,true,NULL);
			printf("\n----------------------------------\n"); 
			
			p = producto_new();
			listObj(p,NULL,true,NULL);
			printf("\n----------------------------------\n");
			
		}
	
	}
	
  
  
  
  	//esto estaba comentado
	cat = categoria_new();
	listObj(cat,NULL,true,NULL);  
  	printf("\n----------------------------------\n");
  	system("PAUSE");	
  	return 0;
}

//-------------------------------------------------------------
void ingresarEmpleado(int argc, char *argv[]){
		
			//printf("argv[4]: %s\n ", argv[4]);
	
			char *nombre; char *apellido; int reportar_a; int extension; char *fecha_nac;
			int i=3;
  			for (i; i< argc; i++){
				switch (i){				
					case 3: 
						nombre = argv[i];					
					case 4: 
						apellido = argv[i];
					case 5:
						fecha_nac = argv[i];
					case 6:	
						reportar_a = atoi(argv[i]);
					case 7:
						extension = atoi(argv[i]);
				}
				//creamos el objeto
				obj_empleado *cat;
				cat = empleado_new();
				
				
				//printf("apellido : %s\n", apellido);
				cat -> setNombre(cat, nombre);
				
				cat -> setApellido(cat, apellido);
				
				cat -> setFechaNac(cat, fecha_nac);
				
				cat -> setReportaA(cat, reportar_a);
				
				cat -> setExtension(cat, extension);
				
				
				cat -> saveObj(cat);			
				/*if(!saveObj_empleadoImpl(cat)==1){
				//	printf("salvo el objeto");	
				}*/
				
				cat -> showObj(cat);
				
				//cat-> findbykey(cat));
				
				//habria que liberar el objeto
			}
};


//-------------------------------------------------------------
void ingresarCategoria(int argc, char *argv[]){
		obj_categoria *cat1;			
		cat1 = categoria_new();
		
		cat1 -> setNombreCategoria(cat1, argv[3]);
		printf("\n");
		
		cat1 -> saveObj(cat1);
		cat1 -> showObj(cat1);
};

//-------------------------------------------------------------

/*
void mostrarArgumentos(int argc, char *argv[]){
	int k;
  	for (k=1; k<argc; k++)
		printf("i: %d %s\n",k,argv[k]);
	

};
*/














