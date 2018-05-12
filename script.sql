--drop table empleado;
create table empleado(legajo serial primary key, fechaingreso date,apellido varchar(50), nombres varchar(80), telefono varchar(20), domicilio varchar(50));
insert into empleado(fechaingreso,apellido,nombres,telefono,domicilio)values('2005-02-01','Gomez','Ariel Gerardo','154336644','soler 123');