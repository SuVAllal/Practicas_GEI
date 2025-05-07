# Boletín PostgreSQL
## Ejercicio 2
#### Crea un usuario en PostgreSQL identificado por una clave y con el privilegio ``createdb`` . Crea una base de datos con ese usuario.
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
#### Conéctate con python a la base de datos. Comprueba los parámetros de la conexión viendo el valor del campo dsn o el método get_dsn_parameters de la conexión.
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
#### Completa la función ```connect_db()``` para que se realice la conexión con la BD. Usa las credenciales y la BD creadas anteriormente. Desactiva el modo ```autocommit```. La función debe devolver la conexión obtenida.

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
#### Completa la función ```disconnect_db()``` para desconectarte de forma adecuada de la BD.
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
#### Completa la función ```create_table()``` para crear la tabla ```artigo``` con 3 campos: ```codart```, tipo ```int```, clave primaria; ```nomart```, tipo ```varchar(30)``` que no admite nulos, y ```prezoart```, tipo ```numeric(5, 2)``` que admite nulos y tiene una restricción que requiere que los precios deben ser positivos. No hagas ningún control transaccional ni de errores.

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

## Ejercicio 8
#### Ejecuta el programa y crea la tabla ``artigo``. Sin salir del programa, abre una sesión con ``psql`` y mira si la tabla está creada (ejecuta ``\d`` en ``psql`` para listar las tablas del usuario). Vuelve a seleccionar la opción de crear la tabla, y comprueba qué pasa.
>Seguimos el mismo procedimiento para crear la tabla y comprobar si está creada que en el ejercicio anterior.

Al volver a intentar crear la tabla sale lo siguiente:
```python
(python_entorno) vilalsus@LAPTOP-0QJEFQS0:/mnt/e/GEI/3º CURSO/BDA/Prácticas/Py
thon$ python exerbda.py 
Conectando a PosgreSQL...
Conectado.

      -- MENÚ --
1 - Crear táboa artigo   
q - Saír   

Opción> 1
Tabla artigo creada

      -- MENÚ --
1 - Crear táboa artigo   
q - Saír

Opción> 1
Traceback (most recent call last):
  File "/mnt/e/GEI/3º CURSO/BDA/Prácticas/Python/exerbda.py", line 103, in <module>
    main()
  File "/mnt/e/GEI/3º CURSO/BDA/Prácticas/Python/exerbda.py", line 97, in main
    menu(conn)
  File "/mnt/e/GEI/3º CURSO/BDA/Prácticas/Python/exerbda.py", line 85, in menu
    create_table(conn)  
    ^^^^^^^^^^^^^^^^^^
  File "/mnt/e/GEI/3º CURSO/BDA/Prácticas/Python/exerbda.py", line 62, in create_table
    cur.execute(sentencia_create)
psycopg2.errors.DuplicateTable: relation "artigo" already exists

(python_entorno) vilalsus@LAPTOP-0QJEFQS0:/mnt/e/GEI/3º CURSO/BDA/Prácticas/Py
thon$
```

#### Qué ha pasado y por qué:
* Al ejecutar la primera vez la opción "1", se ha creado la tabla ``artigo`` correctamente.
* Luego, sin salir del programa lo hemos intentado otra vez, pero como la tabla ya existía, PostgreSQL ha lanzado un error:
```python
psycopg2.errors.DuplicateTable: relation "artigo" already exists
```
Este error ocurre ya que la función ``create_table`` tal como la hemos escrito intenta crear la tabla sin comprobar si ya existe, y PostgreSQL no permite crearla dos veces (si no usas algo como ``IF NOT EXISTS``).

## Ejercicio 9
#### Vuelve a ejecutar el programa, selecciona la opción de crear la tabla, y luego sal del programa pulsando q. ¿Qué ocurre ahora?
Vuelve a ocurrir lo mismo que en el ejercicio anterior. La única manera de evitar este error es hacer un `DROP TABLE` y borrar la tabla antes de volver a crearla.

## Ejercicio 10
#### Revisa la función ``create_table()``. Incluye el control transaccional y control de errores adecuado.
```python
import sys
import psycopg2
import psycopg2.errorcodes

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

    # Establecemos el nivel de aislamiento (cómo y cuándo los cambios hechos por una transacción son visibles para otras)
    # READ_COMMITTED es el nivel predeterminado de PostgreSQL -> cada consulta dentro de una transacción ve solo los datos que fueron confirmados (commited) antes de iniciar esa consulta
		# Ventaja: es seguro frente a lecturas sucias (no ves datos de transacciones no confirmadas)
		# Desventaja: sí puede ver diferentes versiones de datos si otra transacción confirma mientras ocurre esta
	conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
	try:
		cur = conn.cursor() # Para ejecutar sentencias SQL hay que crear un cursor a partir de la conexión
		cur.execute(sentencia_create)
		conn.commit()
		print("Tabla artigo creada")
	except psycopg2.Error(e) as e:
		if e.pgcode == psycopg2.errorcodes.DUPLICATE_TABLE:
			print(f"La tabla artigo ya existe. No se crea")
		else:
			print(f"Error {e.pgcode}: {e.pgerror}")
		conn.rollback()
	finally:
		cur.close()
```
Otra forma de hacerlo es:
```python
with conn.cursor() as cursor:
	try:
		cursor.execute(sentencia_create)
		conn.commit()
		print("Tabla artigo creada")
	except psycopg2.Error(e) as e:
		if e.pgcode == psycopg2.errorcodes.DUPLICATE_TABLE:
			print(f"La tabla artigo ya existe. No se crea")
		else:
			print(f"Error {e.pgcode}: {e.pgerror}")
		conn.rollback()
```
Si solo usamos el bloque de ``finally`` para cerrar el cursor, podemos optar por la alternativa de usar ``with``, que ya lo cierra por nosotros.
>**NOTA:** Usando `with psycopg2.connect(...) as conn` NO cerraría la conexión (sí que lo haría con psycopg v3).

## Ejercicio 11
#### Crea una nueva opción de menú para eliminar (`drop`) la tabla, e impleméntala en una función. A partir de ahora haz siempre un control correcto de las transacciones y de las excepciones.
```python
import sys
import psycopg2
import pyscopg2.errorcodes

def drop_table(conn):
	"""
	Elimina la tabla artigo
	:param conn: la conexión abierta a la bd
	:return: Nada
	"""

	sentencia = """
		drop table artigo
	"""

	conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
	with conn.cursor() as cursor:
		try:
			cursor.execute(sentencia)
			conn.commit()
			print(f"Tabla artigo eliminada")
		except psycopg2.Error(e) as e:
			if e.pgcode == psycopg2.errorcodes.UNDEFINED_TABLE:
				print(f"La tabla artigo no existe. No se borra")
			else:
				print(f"Error: {e.pgcode}: {e.pgerror}")
			conn.rollback()


## ------------------------------------------------------------
def menu(conn):
	"""
	Imprime un menú de opciones, solicita la opción y ejecuta la función asociada.
	'q' para salir.
	"""

	MENU_TEXT = """
		-- MENÚ --
1 - Crear tabla artigo
2 - Eliminar tabla artigo
q - Salir
"""

	while True:
		print(MENU_TEXT)
		tecla = input('Opción> ')
		if tecla == 'q':
			break
		elif tecla == '1':
			create_table(conn)
		elif tecla == '2':
			drop_table(conn)
```

## Ejercicio 12
#### Crea una nueva función que pida por teclado un código, nombre y precio (debe ser posible omitir el precio) e inserte una fila en la tabla artigo. Haz un control de errores de forma que la persona que use el programa no tenga que comprender conceptos como claves primarias o restricciones del modelo relacional. Puedes usar la función `input(<prompt>)` para recoger datos como strings, y las funciones de conversión `int(<string>)` y `float(<string>)`. No es necesario hacer un control exhaustivo de la entrada de datos.
```python
def add_row(conn):
	"""
	Pide por teclado código, nombre y precio e inserta el artículo
	:param conn: la conexión abierta a la bd
	:return: Nada
	"""

	scodigo = input('Código: ') # recogemos el código como un string
	# Convertimos el string en un int para insertarlo en la bd
	if scodigo == "":
		codigo = None
	else:
		codigo = int(scodigo)

	snombre = input('Nombre del producto: ')
	# Otra forma de escribir el if-else:
	nombre = None if snombre == "" else snombre

	sprecio = input('Precio del producto: ')
	precio = None if sprecio == "" else float(sprecio)

	sentencia = """
		insert into artigo(codart, nomart, prezoart)
			values(%(codigo)s, %(nombre)s, %(precio)s)
	"""
	# De esta forma le ponemos nombres a los valores

	conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
	wit conn.cursor() as cursor:
		try:
			cursor.execute(sentencia, {'codigo': codigo, 'nombre': nombre, 'precio': precio})
			conn.commit()
			print(f"Artículo añadido.")
		except psycopg2.Error as e:
			if e.pgcode == psycopg2.errorcodes.UNIQUE_VIOLATION:
				print(f"El artículo de código {codigo} ya existe. No se añade.)
			elif e.pgcode == psycopg2.errorcodes.NOT_NULL_VIOLATION:
				if e.dialog.column_name == "codart":
					print(f"El código del artículo es obligatorio.")
				elif e.diag.column_name == "nomart":
					print(f"El nombre del artículo es obligatorio.")
			elif e.pgcode == psycopg2.errorcodes.CHECK_VIOLATION:
				print(f"El precio del artículo debe ser positivo.")
			else:
				print(f"Error {e.pgcode}: {e.pgerror}")
			conn.rollback()


# ¡Añadir la nueva opción al menú!
```

Para comprobar que funciona nos conectamos a nuestro usuario en PostgreSQL y hacemos un `select` en la tabla:
```bash
vilalsus=> \dt artigo
         List of relations
 Schema |  Name  | Type  |  Owner
--------+--------+-------+----------
 public | artigo | table | vilalsus
(1 row)

vilalsus=> SELECT * FROM artigo;
 codart | nomart | prezoart 
--------+--------+----------
     15 | Champú |     5.00
(1 row)
```

## Ejercicio 13
#### Crea una opción para borrar un artículo a partir del código. Indica si se borró o si el código del artículo no existía. No olvides el control transaccional y de errores, pero recuerda que si una sentencia `delete` no borra ninguna fila porque no las encuentra (por ejemplo, ninguna satisface el filtro del `where`), eso no es un error.
```python
def delete_row(conn):
	"""
	Pide por teclado el código de un artículo y lo elimina
	:param conn: la conexión abierta a la bd
	:return: Nada
	"""

	scodigo = input('Código del artículo a eliminar: ')
	codigo = None if scodigo == "" else int(scodigo)

	sentencia = """
		delete from artigo
		where codart = %s
	"""

	conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
	with conn.cursor() as cursor:
		try:
			cursor.execute(sentencia, (codigo,))
			conn.commit()

			# Miramos el nº de filas afectadas por la sentencia para
			# saber si alguna fila satisfacía el filtro del where
			if cursor.rowcount == 0:
				print(f"El artículo con código {codigo} no existe.")
			else:
				print(f"Artículo eliminado.")
		except psycopg2.Error as e:
			print(f"Error {e.pgcode}: {e.pgerror}")
			conn.rollback()

# ¡Añadir la nueva opción al menú!
```

Para comprobar que funciona eliminamos alguna de las filas que creamos con la opción de `add_row()` y hacemos select en la tabla para ver si se ha eliminado:
```bash
      -- MENÚ --
1 - Crear tabla artigo
2 - Eliminar tabla artigo
3 - Añadir artículo
4 - Eliminar artículo
q - Salir

Opción> 3
Código: 1
Nombre del producto: Auriculares
Precio del producto: 25.99
Artículo añadido.

vilalsus=> SELECT * FROM artigo;
 codart |   nomart    | prezoart 
--------+-------------+----------
     15 | Champú      |     5.00
      1 | Auriculares |    25.99
(2 rows)

#--------------------------------------

      -- MENÚ --
1 - Crear tabla artigo
2 - Eliminar tabla artigo
3 - Añadir artículo
4 - Eliminar artículo
q - Salir

Opción> 4
Código del artículo a eliminar: 1
Artículo eliminado.


vilalsus=> SELECT * FROM artigo;
 codart | nomart | prezoart 
--------+--------+----------
     15 | Champú |     5.00
(1 row)
```


## Ejercicio 14
#### Crea una opción para borrar todos los artículos cuyo nombre incluya un determinado texto, que pedirás por teclado. Indica cuántos artículos se borraron.
```python
def delete_by_name_fragment(conn):
	"""
	Pide por teclado un fragmento de texto y elimina todos los artículos cuyo nombre lo contenga.
	:param conn: la conexión abierta a la bd
	:return: Nada
	"""

	fragmento = input('Texto a buscar para eliminar: ')

	# ILIKE es como LIKE pero no distingue entre mayúsculas y minúsculas
	sentencia = """
		DELETE FROM artigo
		WHERE nomart ILIKE %s
	"""

	conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
	with conn.cursor() as cursor:
		try:
			cursor.execute(sentencia, ('%' + fragmento + '%',)) # buscamos cualquier artículo cuyo nombre CONTENGA (por eso va entre %) el texto introducido 
			eliminados = cursor.rowcount
			conn.commit()
			print(f"Se han eliminado {eliminados} artículos(s).")
		except psycopg2.Error as e:
			print(f"Error {e.pgcode}: {e.pgerror}")
			conn.rollback()

# ¡Añadir la nueva opción al menú!
```

Para comprobar que funciona:
```bash
vilalsus=> SELECT * FROM artigo;
 codart |         nomart          | prezoart 
--------+-------------------------+----------
     15 | Champú                  |     5.00
      1 | Pan                     |     0.67
      2 | Pancito rico            |     0.99
      3 | Supermega pan delicioso |     2.99
(4 rows)

# ------------------------------------------

      -- MENÚ --
1 - Crear tabla artigo
2 - Eliminar tabla artigo
3 - Añadir artículo
4 - Eliminar artículo
5 - Eliminar artículos por nombre
q - Salir

Opción> 5   
Texto a buscar para eliminar: pan
Se han eliminado 3 artículo(s).

# ------------------------------------------
vilalsus=> SELECT * FROM artigo;
 codart | nomart | prezoart 
--------+--------+----------
     15 | Champú |     5.00
(1 row)
```


## Ejercicio 15
#### Crea una opción que indique cuántos artículos hay almacenados en la base de datos.
> `fetchone()` devuelve la siguiente fila del resultado de la consulta como una tupla:
> - Si hacemos `SELECT COUNT(*) FROM artigo;` entonces devuelve `(2,)` 
> - Si hacemos `SELECT codart, nomart FROM artigo;` devolvería:
> `print(cursor.fetchone())` -> (1, 'Lápiz')
> `print(cursor.fetchone())` -> (2, 'Bolígrafo')
> `print(cursor.fetchone())` -> None (si no hay más filas)   


```python
def count_articles(conn):
	"""
	Cuenta cuántos artículos hay almacenados en la base de datos.
	:param conn: la conexión abierta a la bd
	:return: Nada
	"""

	sentencia = """
		SELECT COUNT(*) FROM artigo
	"""

	conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
	with conn.cursor() as cursor:
		try:
			cursor.execute(sentencia)
			resultado = cursor.fetchone() # devuelve la siguiente fila del resultado de la consulta como una tupla, en este caso devuelve (2,) si hay 2 artículos en la BD
			total = resultado[0] # guardamos solo el número
			print(f"Hay {total} artículo(s) en la base de datos.")
		except psycopg2.Error as e:
			print(f"Error {e.pgcode}: {e.pgerror}")
			conn.rollback()


# ¡Añadir la nueva opción al menú!
```


## Ejercicio 16
#### Crea una opción que muestre el detalle de un artículo (todos sus datos) a partir de su código, que debes pedir por teclado. Ten en cuenta que el código introducido puede no existir.
```python
def show_row(conn):
    """
    Pide por teclado el código de un artículo y muestra sus detalles
    :param conn: la conexión abierta a la bd
    :return: Nada
    """

    scodigo = input('Código del artículo: ')
    codigo = None if scodigo == "" else int(scodigo)
    
    sentencia = """
        select nomart, prezoart
        from artigo
        where codart = %(codigo)s
    """

    conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
    with conn.cursor(cursor_factory=psycopg2.extras.DictCursor) as cursor: # configuramos el cursor para que devuelva los resultados como diccionarios
        try:
            cursor.execute(sentencia, {'codigo': codigo})
            resultado = cursor.fetchone()
            if resultado: # si el código existía nos devuelve un resultado
                # Al configurar el cursor ahora podemos acceder a los datos por el nombre de la columna
                precio = resultado['prezoart'] if resultado['prezoart'] else "Desconocido" # RECUERDA QUE SE PODÍAN AÑADIR ARTÍCULOS SIN PRECIO
                print(f"Código: {codigo}. Nombre: {resultado['nomart']}. Precio: {precio}")
            else:
                print(f"El artículo con el código {codigo} no existe.")
        except psycopg2.Error as e:
            print(f"Error {e.pgcode}: {e.pgerror}")
            conn.rollback()


# ¡Añadir la nueva opción al menú!
```


## Ejercicio 17
#### Crea una opción que muestre un listado completo de todos los artículos. Usa un bucle y la función `fetchone()` del cursor.
```python
def show_all_rows(conn):
	"""
	Muestra todos los artículos de la tabla artigo uno por uno usando fetchone().
	:param conn: la conexión abierta a la bd
	:return: Nada
	"""

	sentencia = """
		select codart, nomart, prezoart
		from artigo
		order by codart
	"""

	conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
	with conn.cursor(cursor_factory=psycopg2.extras.DictCursor) as cursor:
		try:
			cursor.execute(sentencia)
			fila = cursor.fetchone()
			if not fila:
				print(f"No hay artículos en la base de datos.")
				return
			
			print(f"\nListado de artículos:")
			while fila:
				precio = fila['prezoart'] if fila['prezoart'] else 'Desconocido'
				print(f"Código: {fila['codart']}. Nombre: {fila['nomart']}. Precio: {precio}")
				fila = cursor.fetchone()
				
		except psycopg2.Error as e:
			print(f"Error {e.pgcode}: {e.pgerror}")
			conn.rollback()


# ¡Añadir la nueva opción al menú!
```

**NOTA SOBRE `fetchone()` y `fetchall()`:**
* `fetchone()` devuelve la siguiente fila, o `None` si no existe.
* `fetchall()` devuelve una lista con todas las filas restantes (inicialmente serán todas las filas).
- Cuando la consulta devuelve una sola fila, usaremos `fetchone()`.
- Cuando la consulta devuelve una colección de filas tenemos dos opciones:
	- Usar `cur.fetchall()` y un bucle `for` para recorrer la colección.
		- Ventaja: es muy sencillo.
		- Desventaja: puede tener una gran demanda de memoria, ya que recupera todas las filas.
```python
cur.execute("""select id, nome, datanac from persona""")
rows = cur.fetchall()
for row in rows:
	print(row)
```

- Usar ```cur.fetchone()``` en un bucle:
		- Ventaja: menor demanda de recursos.
		- Desventaja: código ligeramente más complejo.
```python
cur.execute("""select id, nome, datanac from persona""")
row = cur.fetchone()
while row:
	print(row)
	row = cur.fetchone()
```