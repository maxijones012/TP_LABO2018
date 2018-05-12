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




void cli_show(void *c)
{
  obj_cliente *cli = (obj_cliente*)c;
     printf("ID:%d\n",cli->getClienteId(cli));
}

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
  
  /**int k;
  	for (k=1; k<argc; k++){
		printf("i: %d %s\n",k,argv[k]);
	}
  */	
  
  //	printf("%s\n",argv[1]);
  	if  (!strcmp(argv[1],"-a")){
  	//	printf("if 1\n");
			if (!strcmp(argv[2],"-empleado")){
			//printf("if 20\n\n\n\n");				
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
				
				
				//cat -> saveObj(cat);			
				if(!saveObj_empleadoImpl(cat)==1){
					printf("salvo el objeto");	
				}
				
				cat -> showObj(cat);
				
				//cat-> findbykey(cat));
				
				//habria que liberar el objeto
			}  
		}
		
		
		
		
		
		if(!strcmp(argv[2],"-categoria")){
			//obj_categoria *cat1			
			//cat1 = categoria_new();
			
			//setNombreCategoria();
			printf("\n");
			
			
		}
	}
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  //esto estaba comentado
 ///*
  
  emp = empleado_new();

  listObj(emp,NULL,true,NULL);
  printf("\n----------------------------------\n");
  /*prov = proveedor_new();
  listObj(prov,NULL,true,NULL);
  printf("\n----------------------------------\n");
  cat = categoria_new();
  listObj(cat,NULL,true,NULL);
  printf("\n----------------------------------\n"); 
  
  p = producto_new();
  listObj(p,NULL,true,NULL);
  printf("\n----------------------------------\n");
  
  */
  system("PAUSE");	
  return 0;
}
