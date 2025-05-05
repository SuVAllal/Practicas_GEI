# Boletín PostgreSQL
## Ejercicio 2
Crea un usuario en PostgreSQL identificado por una clave y con el privilegio ``createdb`` . Crea una base de datos con ese usuario.
```
vilalsus@LAPTOP-0QJEFQS0:~/bda_python$ sudo -i

root@LAPTOP-0QJEFQS0:~# su - postgres -c psql

postgres=# create user vilalsus with password 'vilalsus';
CREATE ROLE
postgres=# ALTER USER vilalsus WITH CREATEDB;
ALTER USER
```
Si nos conectamos usando 
```
psql -h localhost -U vilalsus
```
Intentará conectarnos a una BD con el mismo nombre que el usuario, como de momento no existe, dará error. Para poder crear la BD, entraremos primero a la BD de PostgreSQL, que siempre está disponible, y desde ahí crearemos la nueva BD:
```
root@LAPTOP-0QJEFQS0:~# psql -U vilalsus h localhost -d postgres
postgres=> CREATE DATABASE vilalsus;
```

## Ejercicio 3
Conéctate con python a la base de datos. Comprueba los parámetros de la conexión viendo el valor del campo dsn o el método get_dsn_parameters de la conexión.
```python
>>> import psycopg2
# Parámetros de conexión
>>>  conn = psycopg2.connect(host='localhost', user='vilalsus', password='vilalsus', database='vilalsus')
# Mostrar el DSN (cadena de conexión)
>>>  print("DSN:", conn.dsn)
DSN: user=vilalsus password=xxx dbname=vilalsus host=localhost
# Mostrar parámetros detallados
>>>  print("Parámetros de conexión:", conn.get_dsn_parameters())
Parámetros de conexión: {'user': 'vilalsus', 'channel_binding': 'prefer', 'dbname': 'vilalsus', 'host': 'localhost', 'port': '5432', 'options': '', 'sslmode': 'prefer', 'sslcompression': '0', 'sslcertmode': 'allow', 'sslsni': '1', 'ssl_min_protocol_version': 'TLSv1.2', 'gssencmode': 'prefer', 'krbsrvname': 'postgres', 'gssdelegation': '0', 'target_session_attrs': 'any', 'load_balance_hosts': 'disable'}
# Cerrar la conexión
>>> conn.close()
```

## Ejercicio 4
El script es una plantilla que se irá completando en siguientes ejercicios. De momento lo que hace es:
1. **Importa psycopg2:** para conectarse a PostgreSQL.
2. Define tres funciones clave:
	* ```connect_db()``` y ```disconnect_db()``` están vacías por ahora.
	* ```create_table()``` solo imprime un mensaje ("Tabla artículo creada"), no hace nada en la BD.
3. ```menu()``` muestra un menú interactivo simple con dos opciones: crear tabla (opción 1) p salir (q).
4. ```main()``` llama a ```connect_db()```, muestra el menú y luego cierra la conexión.

## Ejercicio 5
Completa la función ```connect_db()``` para que se realice la conexión con la BD. Usa las credenciales y la BD creadas anteriormente. Desactiva el modo ```autocommit```.
La función debe devolver la conexión obtenida.

> En [psycopg2.errors – Exception classes mapping PostgreSQL errors — Psycopg 2.9.10 documentation](https://www.psycopg.org/docs/errors.html) se puede ver una lista de errores (que corresponden con el estándar SQLSTATE) expuesta por psycopg2.
##### Formato para las excepciones:
```python
try:
	# Acciones
except <ListaTiposExcepcion> [as <NombreExcepcion> ]:
	# Gestión de NombreExcepcion
[else:
	# Esta parte se ejecuta si no pasa por la parte "except" ]
[finally:
	# Esta parte se ejecuta siempre, ocurra o no un error ]
```

Ejercicio:
```python
import sys
import psycopg2

def connect_db():
	"""
	Se conecta a la BD predeterminada del usuario (DSN vacío)
	:return: La conexión con la BD (o se sale del programa al no conseguirla)
	"""

	try:
		conn = pyscopg2.connect("")
		conn.autocommit = False
		return conn
	except psycopg2.Error(e) as e:
		print(f"Error de conexión: {e}")
		sys.exit(1)

```


## Ejercicio 6
Completa la función ```disconnect_db()``` para desconectarte de forma adecuada de la BD.
```python
def disconnect_db(conn):
	"""
	Se desconecta de la BD.
	:param conn: la conexión abierta a la BD
	:return: nada
	"""
	conn.close()
```

## Ejercicio 7
Completa la función ```create_table()``` para crear la tabla ```artigo``` con 3 campos: ```codart```, tipo ```int```, clave primaria; ```nomart```, tipo ```varchar(30)``` que no admite nulos, y ```prezoart```, tipo ```numeric(5, 2)``` que admite nulos y tiene una restricción que requiere que los precios deben ser positivos.
No hagas ningún control transaccional ni de errores.

| Campo    | Tipo          | Restricciones                   |
| -------- | ------------- | ------------------------------- |
| codart   | int           | Clave primaria                  |
| nomart   | varchar(30)   | No admite nulos                 |
| prezoart | numeric(5, 2) | Admite nulos, debe ser positivo |
```python
import sys
import psycopg2

def create_table(conn):
	"""
	Crea la tabla artigo (codart, nomart, prezoart)
	:param conn: la conexión abierta a la bd
	:return: Nada
	"""

	# Guardamos la sentencia a ejecutar en una variable aparte por comodidad
	sentencia_create = """
		create table artigo(
			codart int constraint artigo_pkey primary key,
			nomart varchar(30) not null,
			prezoart numeric(5,2) constraint ch_art_prezo_pos check (prezoart > 0)
		)
	"""

	cur = conn.cursor() # Para ejecutar sentencias SQL hay que crear un cursor a partir de la conexión
	cur.execute(sentencia_create)
	conn.commit()

	print("Tabla artigo creada)
```

#### Comprobamos que funciona:
1. **Entramos al entorno virtual donde tengamos el script:**
```bash
source python_entorno/bin/activate
cd ~/bda_python
```
2. **Ejecuta el script:**
```bash
python exerbda.py
```
3. **Sigue el menú del programa:**
```
  -- MENÚ --
1 - Crear táboa artigo   
q - Saír   
Opción>

```
* Pulsa ```1``` y luego Enter para crear la tabla.
* Debería mostrarse el mensaje:
```
Tabla artigo creada
```
* Luego pulsa ``q`` para salir del menú.
4. **Verifica en PostgreSQL que la tabla existe:**
```bash
(python_entorno) vilalsus@LAPTOP-0QJEFQS0:~/bda_python$ sudo -i
root@LAPTOP-0QJEFQS0:~ psql -h localhost -U vilalsus

vilalsus=> \dt # para ver las tablas de esta BD
         List of relations
 Schema |  Name  | Type  |  Owner   
--------+--------+-------+----------
 public | artigo | table | vilalsus
(1 row)

vilalsus=> \d artigo # para ver los detalles de la tabla artigo
                       Table "public.artigo"
  Column  |         Type          | Collation | Nullable | Default
----------+-----------------------+-----------+----------+---------
 codart   | integer               |           | not null |
 nomart   | character varying(30) |           | not null |
 prezoart | numeric(5,2)          |           |          |
Indexes:
    "artigo_pkey" PRIMARY KEY, btree (codart)
Check constraints:
    "ch_art_prezo_pos" CHECK (prezoart > 0::numeric)
```
