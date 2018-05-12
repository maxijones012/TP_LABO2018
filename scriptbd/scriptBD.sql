create table empleados(
empleado_id serial NOT NULL,
nombre char(80) NULL,
apellido char(80) NULL,
fecha_nac date NULL,
reporta_a int NULL,
extension int NULL,
CONSTRAINT PK_empleados PRIMARY KEY (empleado_id));

create table proveedores(
proveedor_id int NOT NULL,
nombre_proveedor char(50) NOT NULL,
contacto char(50) NOT NULL,
celu_prov char(12) NULL,
fijo_prov char(12) NULL,
CONSTRAINT PK_proveedores PRIMARY KEY
(proveedor_id ) );

create table categorias(
categoria_id int NOT NULL,
nombre_categoria char(50) NOT NULL,
CONSTRAINT PK_categorias PRIMARY KEY
(categoria_id) ) ;

create table clientes(
cliente_id serial NOT NULL,
cedula_ruc char(10) NOT NULL,
nombre_cia char(30) NOT NULL,
nombre_contacto char(50) NOT NULL,
dir_cliente char(50) NOT NULL,
fax char(12) NULL,
email char(50) NULL,
celular char(12) NULL,
fijo char(12) NULL,
CONSTRAINT PK_clientes PRIMARY KEY
(cliente_id) );


create table productos(
producto_id serial NOT NULL,
proveedor_id int NOT NULL,
categoria_id int NOT NULL,
descripcion char(50) NULL,
precio_unit numeric NOT NULL,
existencia int NOT NULL,
CONSTRAINT PK_productos PRIMARY KEY
(producto_id )) ;


create table ordenes(
orden_id int NOT NULL,
empleado_id int NOT NULL,
cliente_id int NOT NULL,
fecha_orden date NOT NULL,
descuento int NULL,
CONSTRAINT PK_ordenes PRIMARY KEY
(orden_id) );

create table detalle_ordenes(
orden_id int NOT NULL,
detalle_id int NOT NULL,
producto_id int NOT NULL,
cantidad int NOT NULL,
CONSTRAINT PK_detalle_ordenes PRIMARY KEY
(orden_id,detalle_id ) );

ALTER table ordenes
ADD CONSTRAINT fk_ordenes_clien_ord_clientes FOREIGN KEY(cliente_id)
REFERENCES clientes (cliente_id)
on delete restrict on update restrict;

ALTER table ordenes ADD CONSTRAINT fk_ordenes_emple_ord_empledo FOREIGN KEY(empleado_id)
REFERENCES empleados (empleado_id)
on delete restrict on update restrict;

ALTER table detalle_ordenes ADD CONSTRAINT fk_detalle__orden_det_ordenes FOREIGN KEY(orden_id)
REFERENCES ordenes (orden_id)
on delete restrict on update restrict;

ALTER table detalle_ordenes ADD CONSTRAINT fk_detalle__prod_deta_producto FOREIGN KEY(producto_id)
REFERENCES productos (producto_id)
on delete restrict on update restrict;

ALTER table productos ADD CONSTRAINT fk_producto_cate_prod_categori FOREIGN KEY(categoria_id)
REFERENCES categorias (categoria_id)
on delete restrict on update restrict;

ALTER table productos ADD CONSTRAINT fk_producto_prov_prod_proveedor FOREIGN KEY(proveedor_id)
REFERENCES proveedores (proveedor_id)
on delete restrict on update restrict;

ALTER table empleados ADD CONSTRAINT fk_empledo_reporta FOREIGN KEY(reporta_a)
REFERENCES empleados (empleado_id)
on delete restrict on update restrict;

insert into categorias (categoria_id, nombre_categoria) values (100, 'CARNICOS');
insert into categorias (categoria_id, nombre_categoria) values (200, 'LACTEOS');
insert into categorias (categoria_id, nombre_categoria) values (300, 'LIMPIEZA');
insert into categorias (categoria_id, nombre_categoria) values (400, 'HIGINE PERSONAL');
insert into categorias (categoria_id, nombre_categoria) values (500, 'MEDICINAS');
insert into categorias (categoria_id, nombre_categoria) values (600, 'COSMETICOS');
insert into categorias (categoria_id, nombre_categoria) values (700, 'REVISTAS');

insert into proveedores (proveedor_id, nombre_proveedor, contacto,celu_prov,fijo_prov) values
(10, 'DON DIEGO', 'MANUEL ANDRADE', '099234567','2124456');
insert into proveedores (proveedor_id, nombre_proveedor, contacto,celu_prov,fijo_prov) values
(20, 'PRONACA', 'JUAN PEREZ', '0923434467','2124456');
insert into proveedores (proveedor_id, nombre_proveedor, contacto,celu_prov,fijo_prov) values
(30, 'TONY', 'JORGE BRITO', '099234567','2124456');
insert into proveedores (proveedor_id, nombre_proveedor, contacto,celu_prov,fijo_prov) values
(40, 'MIRAFLORES', 'MARIA PAZ', '098124498','2458799');
insert into proveedores (proveedor_id, nombre_proveedor, contacto,celu_prov,fijo_prov) values
(50, 'ALMAY', 'PEDRO GONZALEZ', '097654567','2507190');
insert into proveedores (proveedor_id, nombre_proveedor, contacto,celu_prov,fijo_prov) values
(60, 'REVLON', 'MONICA SALAS', '099245678','2609876');
insert into proveedores (proveedor_id, nombre_proveedor, contacto,celu_prov,fijo_prov) values
(70, 'YANBAL', 'BETY ARIAS', '098124458','2450887');
insert into proveedores (proveedor_id, nombre_proveedor, contacto,celu_prov,fijo_prov) values
(120, 'JURIS', 'MANUEL ANDRADE', '099234567','2124456');
insert into proveedores (proveedor_id, nombre_proveedor, contacto,celu_prov,fijo_prov) values
(80, 'CLEANER', 'MANUEL ANDRADE', '099234567','2124456');
insert into proveedores (proveedor_id, nombre_proveedor, contacto,celu_prov,fijo_prov) values
(90, 'BAYER', 'MANUEL ANDRADE', '099234567','2124456');
insert into proveedores (proveedor_id, nombre_proveedor, contacto,celu_prov,fijo_prov) values
(110, 'PALMOLIVE', 'MANUEL ANDRADE', '099234567','2124456');

INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (1,10,100,'SALCHICHAS VIENESAS',2.60,200);
INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (2,10,100,'SALAMI DE AJO',3.60,300);
INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (3,10,100,'BOTON PARA ASADO',4.70,400);
INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (4,20,100,'SALCHICHAS DE POLLO',2.90,200);
INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (5,20,100,'JAMON DE POLLO',2.80,100);
INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (6,30,200,'YOGURT NATURAL',4.30,80);
INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (7,30,200,'LECHE CHOCOLATE',1.60,90);
INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (8,40,200,'YOGURT DE SABORES',1.60,200);
INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (9,40,200,'CREMA DE LECHE',3.60,30);
INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (10,50,600,'BASE DE MAQUILLAJE',14.70,40);
INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (11,50,600,'RIMMEL',12.90,20);
INSERT INTO PRODUCTOS (producto_id,proveedor_id,categoria_id,descripcion,precio_unit,existencia) VALUES (13,60,600,'SOMBRA DE OJOS',9.80,100);
set datestyle to dmy;

INSERT INTO EMPLEADOS (empleado_id,nombre,apellido,fecha_nac,reporta_a,extension) VALUES (1,'JUAN', 'CRUZ', '18/01/67',null, 231);
INSERT INTO EMPLEADOS (empleado_id,nombre,apellido,fecha_nac,reporta_a,extension) VALUES (2,'MARIO', 'SANCHEZ', '01/03/79',1,144);
INSERT INTO EMPLEADOS (empleado_id,nombre,apellido,fecha_nac,reporta_a,extension) VALUES (3,'VERONICA', 'ARIAS', '23/06/77',1, 234);
INSERT INTO EMPLEADOS (empleado_id,nombre,apellido,fecha_nac,reporta_a,extension) VALUES (4,'PABLO', 'CELY', '28/01/77',2, 567);
INSERT INTO EMPLEADOS (empleado_id,nombre,apellido,fecha_nac,reporta_a,extension) VALUES (5,'DIEGO', 'ANDRADE', '15/05/70',2, 890);
INSERT INTO EMPLEADOS (empleado_id,nombre,apellido,fecha_nac,reporta_a,extension) VALUES (6,'JUAN', 'ANDRADE', '17/11/76',3, 230);
INSERT INTO EMPLEADOS (empleado_id,nombre,apellido,fecha_nac,reporta_a,extension) VALUES (7,'MARIA', 'NOBOA', '21/12/79',3, 261);

INSERT INTO CLIENTES (cliente_id,cedula_ruc,nombre_cia,nombre_contacto,dir_cliente,fax,email,celular,fijo) VALUES   (1,'1890786576','SUPERMERCADO ESTRELLA','JUAN ALBAN','AV.AMAZONAS',NULL,NULL,NULL,NULL);
INSERT INTO CLIENTES (cliente_id,cedula_ruc,nombre_cia,nombre_contacto,dir_cliente,fax,email,celular,fijo) VALUES   (2,'1298765477','EL ROSADO','MARIA CORDERO','AV.AEL INCA',NULL,NULL,NULL,NULL);
INSERT INTO CLIENTES (cliente_id,cedula_ruc,nombre_cia,nombre_contacto,dir_cliente,fax,email,celular,fijo) VALUES   (3,'1009876567','DISTRIBUIDORA PRENSA','PEDRO PINTO','EL PINAR',NULL,NULL,NULL,NULL);
INSERT INTO CLIENTES (cliente_id,cedula_ruc,nombre_cia,nombre_contacto,dir_cliente,fax,email,celular,fijo) VALUES   (4,'1876090006','SU TIENDA','PABLO PONCE','AV.AMAZONAS',NULL,NULL,NULL,NULL);
INSERT INTO CLIENTES (cliente_id,cedula_ruc,nombre_cia,nombre_contacto,dir_cliente,fax,email,celular,fijo) VALUES   (5,'1893456776','SUPERMERCADO DORADO','LORENA PAZ','AV.6 DICIEMBRE',NULL,NULL,NULL,NULL);
INSERT INTO CLIENTES (cliente_id,cedula_ruc,nombre_cia,nombre_contacto,dir_cliente,fax,email,celular,fijo) VALUES   (6,'1678999891','MI COMISARIATO','ROSARIO UTRERAS','AV.AMAZONAS',NULL,NULL,NULL,NULL);
INSERT INTO CLIENTES (cliente_id,cedula_ruc,nombre_cia,nombre_contacto,dir_cliente,fax,email,celular,fijo) VALUES   (7,'1244567888','SUPERMERCADO DESCUENTO','LETICIA ORTEGA','AV.LA PRENSA',NULL,NULL,NULL,NULL);
INSERT INTO CLIENTES (cliente_id,cedula_ruc,nombre_cia,nombre_contacto,dir_cliente,fax,email,celular,fijo) VALUES   (8,'1456799022','EL DESCUENTO','JUAN TORRES','AV.PATRIA',NULL,NULL,NULL,NULL);
INSERT INTO CLIENTES (cliente_id,cedula_ruc,nombre_cia,nombre_contacto,dir_cliente,fax,email,celular,fijo) VALUES   (9,'1845677777','DE LUISE','JORGE PARRA','AV.AMAZONAS',NULL,NULL,NULL,NULL);
INSERT INTO CLIENTES (cliente_id,cedula_ruc,nombre_cia,nombre_contacto,dir_cliente,fax,email,celular,fijo) VALUES   (10,'183445667','YARBANTRELLA','PABLO POLIT','AV.REPUBLICA',NULL,NULL,NULL,NULL);

INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(1,3,4,'17/06/07', 5);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(2,3,4,'02/06/07', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(3,4,5,'05/06/07', 6);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(4,2,6,'06/06/07', 2);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(5,2,7,'09/06/07', NULL);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(6,4,5,'12/06/07', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(7,2,5,'14/06/07', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(8,3,2,'13/06/07', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(9,3,2,'17/06/07', 3);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(10,2,2,'18/06/07', 2);

INSERT INTO detalle_ordenes VALUES(1,1,1,2);
INSERT INTO detalle_ordenes VALUES(1,2,4,1);
INSERT INTO detalle_ordenes VALUES(1,3,6,1);
INSERT INTO detalle_ordenes VALUES(1,4,9,1);
INSERT INTO detalle_ordenes VALUES(2,1,10,10);
INSERT INTO detalle_ordenes VALUES(2,2,13,20);
INSERT INTO detalle_ordenes VALUES(3,1,3,10);
INSERT INTO detalle_ordenes VALUES(4,1,9,12);
INSERT INTO detalle_ordenes VALUES(5,1,1,14);
INSERT INTO detalle_ordenes VALUES(5,2,4,20);
INSERT INTO detalle_ordenes VALUES(6,1,3,12);
INSERT INTO detalle_ordenes VALUES(7,1,11,10);
INSERT INTO detalle_ordenes VALUES(8,1,2,10);
INSERT INTO detalle_ordenes VALUES(8,2,5,14);
INSERT INTO detalle_ordenes VALUES(8,3,7,10);
INSERT INTO detalle_ordenes VALUES(9,1,11,10);
INSERT INTO detalle_ordenes VALUES(10,1,1,5);



-- Inserciones adicionales
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(101,3,4,'17/07/07', 5);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(102,3,4,'02/07/07', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(103,4,5,'05/07/07', 6);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(104,2,6,'06/07/07', 2);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(105,2,7,'09/07/07', NULL);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(106,4,5,'12/07/07', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(107,2,5,'14/07/07', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(108,3,2,'13/07/07', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(109,3,2,'17/07/07', 3);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(110,2,2,'18/07/07', 2);

INSERT INTO detalle_ordenes VALUES(101,1,1,32);
INSERT INTO detalle_ordenes VALUES(101,2,4,31);
INSERT INTO detalle_ordenes VALUES(101,3,6,31);
INSERT INTO detalle_ordenes VALUES(101,4,9,31);
INSERT INTO detalle_ordenes VALUES(102,1,10,310);
INSERT INTO detalle_ordenes VALUES(102,2,13,320);
INSERT INTO detalle_ordenes VALUES(103,1,3,310);
INSERT INTO detalle_ordenes VALUES(104,1,9,312);
INSERT INTO detalle_ordenes VALUES(105,1,1,314);
INSERT INTO detalle_ordenes VALUES(105,2,4,320);
INSERT INTO detalle_ordenes VALUES(106,1,3,312);
INSERT INTO detalle_ordenes VALUES(107,1,11,310);
INSERT INTO detalle_ordenes VALUES(108,1,2,130);
INSERT INTO detalle_ordenes VALUES(108,2,5,134);
INSERT INTO detalle_ordenes VALUES(108,3,7,310);
INSERT INTO detalle_ordenes VALUES(109,1,11,310);
INSERT INTO detalle_ordenes VALUES(110,1,1,35);


INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(201,3,4,'17/07/08', 5);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(202,3,4,'02/07/08', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(203,4,5,'05/07/08', 6);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(204,2,6,'06/07/08', 2);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(205,2,7,'09/07/08', NULL);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(206,4,5,'12/07/08', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(207,2,5,'14/07/08', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(208,3,2,'13/07/08', 10);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(209,3,2,'17/07/08', 3);
INSERT INTO ORDENES (orden_id,empleado_id,cliente_id,fecha_orden,descuento) VALUES(210,2,2,'18/07/08', 2);

INSERT INTO detalle_ordenes VALUES(201,1,1,22);
INSERT INTO detalle_ordenes VALUES(201,2,4,21);
INSERT INTO detalle_ordenes VALUES(201,3,6,21);
INSERT INTO detalle_ordenes VALUES(201,4,9,21);
INSERT INTO detalle_ordenes VALUES(202,1,10,220);
INSERT INTO detalle_ordenes VALUES(202,2,13,220);
INSERT INTO detalle_ordenes VALUES(203,1,3,210);
INSERT INTO detalle_ordenes VALUES(204,1,9,212);
INSERT INTO detalle_ordenes VALUES(205,1,1,124);
INSERT INTO detalle_ordenes VALUES(205,2,4,220);
INSERT INTO detalle_ordenes VALUES(206,1,3,212);
INSERT INTO detalle_ordenes VALUES(207,1,11,210);
INSERT INTO detalle_ordenes VALUES(208,1,2,120);
INSERT INTO detalle_ordenes VALUES(208,2,5,124);
INSERT INTO detalle_ordenes VALUES(208,3,7,120);
INSERT INTO detalle_ordenes VALUES(209,1,11,210);
INSERT INTO detalle_ordenes VALUES(210,1,1,25);


