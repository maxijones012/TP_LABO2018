cat = categoria_new();
  cat->setNombreCategoria(cat,"CAT-PRUEBA");
  cat->saveObj(cat);
  /*
  o = orden_new();
  o->findbykey(o,1);
  // acceder al objeto relacionado de la ordern,el empleado.
  ((obj_empleado *)o->getEmpleadoObj(o))->showObj(o->getEmpleadoObj(o));
  destroyObj(o);
  */
/*
  cli = cliente_new();
  listObj(cli,NULL,true,NULL);  
//  listObj(cli,NULL,true,cli_show);  
//  listObj(cli,NULL,true,cli->showObj);
//  listObj(cli,NULL,true,NULL);

  printf("\n----------------------------------\n");
  emp = empleado_new();
  listObj(emp,NULL,true,NULL);
  printf("\n----------------------------------\n");
  prov = proveedor_new();
  listObj(prov,NULL,true,NULL);
  printf("\n----------------------------------\n");
  cat = categoria_new();
  listObj(cat,NULL,true,NULL);
  printf("\n----------------------------------\n");
  p = producto_new();
  listObj(p,NULL,true,NULL);
  printf("\n----------------------------------\n");
  
  p = producto_new();
  if(p->findbykey(p,7))
  {
	  p->setDescripcion(p,"LECHE CHOCOLATADA");
	  p->setExistencia(p,p->getExistencia(p)*2);
	  p->saveObj(p);
	  destroyObj(p);
  }*/
/*  odet = orden_det_new();
  listObj(odet,NULL,true,NULL);
  */
  
/*
  cli = cliente_new();
  size = cli->findAll(cli,&list,NULL);
  for(i=0;i<size;++i)
  {
    cli_row = ((obj_cliente **)list)[i];
    cli_row->showObj(cli_row);
  }
  
  destroyObjList(list,size);
  destroyObj(cli);
  
   
  emp = empleado_new();
    // codigo para recorrer listado de todos los empleados segun un criterio dado.
//  size= emp->findAll(emp,&list,"nombre like 'MA%' order by empleado_id asc");
  size= emp->findAll(emp,&list,NULL);
  for(i=0;i<size;++i)
  {
    e_row = ((obj_empleado **)list)[i];
    e_row->showObj(e_row);
  }
  // librerar memoria

  destroyObjList(list,size);
  destroyObj(emp);
  
  prov = proveedor_new();
  size = prov->findAll(prov,&list,NULL);
  for(i=0;i<size;++i)
  {
    pv_row = ((obj_proveedor **)list)[i];
    pv_row->showObj(pv_row);
  }
  destroyObjList(list,size);
  destroyObj(prov);
*/
  /*
///  Listar todas las categorias
  cat = categoria_new();
    // codigo para recorrer listado de todas las categorias
  size= cat->findAll(cat,&list,NULL);
  for(i=0;i<size;++i)
  {
    c_row = ((obj_categoria **)list)[i];
    if(i==2)
    {
      c_row->setNombreCategoria(c_row,"PROBANDO");
      c_row->saveObj(c_row);
    } 
    c_row->showObj(c_row);
  }
  // librerar memoria
  destroyObjList(list,size);
  destroyObj(cat);
*/
/*
  if(emp->findbykey(emp,8))
  {
      char newApellido[MAX];
      printf("%s %s\n",emp->getApellido(emp),emp->getNombre(emp));
      strcpy(newApellido,emp->getApellido(emp));
      strcat(newApellido,"...");
          
      emp->setApellido(emp,newApellido);
      emp->saveObj(emp);
      printf("%s %s\n",emp->getApellido(emp),emp->getNombre(emp));
//     emp->showObj(emp);
     //result = emp->saveObj(emp,0,"1977-05-13","SANDOVAL","Augusto",0,0,false);
  }
  */
  /*
   emp = empleado_new();
  // crear instancia y guardar en la base
  emp->setApellido(emp,"Santander");
  emp->setNombre(emp,"Marcelo Alejandro");
  emp->setFechaNac(emp,"1977-05-13");
  //.. hay algunos valores por defecto
  emp->saveObj(emp);
  
  // despues del insert, 
  printf("%d\n",emp->empleado_id);
*/