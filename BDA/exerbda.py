#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# exerbda.py: 
# Programa python para completar siguiendo el boletín de ejercicios de BDA.
#
# Autor: Miguel Rodríguez Penabad (miguel.penabad@udc.es)
# Fecha de creación: 19-01-2021
# Modificado por: Susana Vilarnovo Allal (s.vallal@udc.es)
# Curso 2024-2025

# 1. Importamos el adaptador específico para el SGBD (PostgreSQL)
import sys
import psycopg2
import psycopg2.extras
import psycopg2.errorcodes


## ------------------------------------------------------------
def connect_db():
    """
     Se conecta a la BD predeterminada del usuario (DSN vacío)
     :return: La conexión con la BD (o se sale del programa al no conseguirla)
     """
    try:
        conn = psycopg2.connect("")
        conn.autocommit = False
        return conn
    except psycopg2.Error as e:
        print(f"Error de conexión: {e}")
        sys.exit(1)


## ------------------------------------------------------------
def disconnect_db(conn):
    """
    Se desconecta de la BD. Hace antes un commit de la transacción activa.
    :param conn: la conexión abierta a la BD
    :return: nada
    """
    conn.commit()
    conn.close()


## ------------------------------------------------------------
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
    except psycopg2.Error as e:
        if e.pgcode == psycopg2.errorcodes.DUPLICATE_TABLE:
            print(f"La tabla artigo ya existe. No se crea.")
        else:
            print(f"Error {e.pgcode}: {e.pgerror}")
        conn.rollback()
    finally:
        cur.close()


## ------------------------------------------------------------
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
        except psycopg2.Error as e:
            if e.pgcode == psycopg2.errorcodes.UNDEFINED_TABLE:
                print(f"La tabla artigo no existe. No se borra.")
            else:
                print(f"Error {e.pgcode}: {e.pgerror}")
            conn.rollback()


## ------------------------------------------------------------
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
    
    conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
    with conn.cursor() as cursor:
        try:
            cursor.execute(sentencia, {'codigo': codigo, 'nombre': nombre, 'precio': precio})
            conn.commit()
            print(f"Artículo añadido.")
        except psycopg2.Error as e:
            if e.pgcode == psycopg2.errorcodes.UNIQUE_VIOLATION:
                print(f"El artículo de código {codigo} ya existe. No se añade.")
            elif e.pgcode == psycopg2.errorcodes.NOT_NULL_VIOLATION:
                if e.diag.column_name == "codart":
                    print(f"El código del artículo es obligatorio.")
                elif e.diag.column_name == "nomart":
                    print(f"El nombre del artículo es obligatorio.")
            elif e.pgcode == psycopg2.errorcodes.CHECK_VIOLATION:
                print(f"El precio del artículo debe ser positivo.")
            else:
                print(f"Error {e.pgcode}: {e.pgerror}")
            conn.rollback()
            
            
## ------------------------------------------------------------
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
            
            # Miramos el nº de filas afectadas por la sentencia para
            # saber si alguna fila satisfacía el filtro del where
            if cursor.rowcount == 0:
                print(f"El artículo con código {codigo} no existe.")
            else:
                print(f"Artículo eliminado.")
            
            conn.commit()
        except psycopg2.Error as e:
            print(f"Error {e.pgcode}: {e.perror}")
            conn.rollback()
            

## ------------------------------------------------------------
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
            print(f"Se han eliminado {eliminados} artículo(s).")
            conn.commit()
        except psycopg2.Error as e:
            print(f"Error {e.pgcode}: {e.pgerror}")
            conn.rollback()


## ------------------------------------------------------------
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
            resultado = cursor.fetchone() # devuelve la siguiente fila del resultado de la consulta como una tupla, en este caso devuelve (4,) si hay 4 artículos, p.e
            total = resultado[0]
            print(f"Hay {total} artículo(s) en la base de datos.")
            conn.commit()
        except psycopg2.Error as e:
            print(f"Error {e.pgcode}: {e.pgerror}")
            conn.rollback()
            

## ------------------------------------------------------------
def show_row(conn, control_tx=True):
    """
    Pide por teclado el código de un artículo y muestra sus detalles
    :param conn: la conexión abierta a la bd
    :param control_tx: indica si debemos hacer o no control transaccional
    :return: El código del artículo, si existe. None en otro caso.
    """
    
    scodigo = input('Código del artículo: ')
    codigo = None if scodigo == "" else int(scodigo)
    
    sentencia = """
        select nomart, prezoart
        from artigo
        where codart = %(codigo)s
    """
    
    if control_tx:
        conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
    
    retorno = None
    
    with conn.cursor(cursor_factory=psycopg2.extras.DictCursor) as cursor: # configuramos el cursor para que devuelva los resultados como diccionarios
        try:
            cursor.execute(sentencia, {'codigo': codigo})
            resultado = cursor.fetchone()
            if resultado: # si el código existía nos devuelve un resultado
                # Al configurar el cursor ahora podemos acceder a los datos por el nombre de la columna
                precio = resultado['prezoart'] if resultado['prezoart'] else "Desconocido" # RECUERDA QUE SE PODÍAN AÑADIR ARTÍCULOS SIN PRECIO
                print(f"Código: {codigo}. Nombre: {resultado['nomart']}. Precio: {precio}")
                retorno = codigo
            else:
                print(f"El artículo con el código {codigo} no existe.")
            
            if control_tx:
                conn.commit()
                
        except psycopg2.Error as e:
            print(f"Error {e.pgcode}: {e.pgerror}")
            if control_tx:
                conn.rollback()
                
        return retorno
                

## ------------------------------------------------------------
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
            conn.commit()
                
        except psycopg2.Error as e:
            print(f"Error {e.pgcode}: {e.pgerror}")
            conn.rollback()


## ------------------------------------------------------------
def show_by_price(conn):
    """
    Pide un precio por teclado y muestra los detalles de los artículos 
    que son más caros. Muestra también cuántos artículos se encontraron.
    :param conn: la conexión abierta a la bd
    :return: Nada
    """
    
    sprecio = input('Precio base: ')
    precio = None if sprecio == "" else float(sprecio)
    
    sentencia = """
        select codart, nomart, prezoart
        from artigo
        where prezoart > %(precio)s
    """
    
    conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
    with conn.cursor(cursor_factory = psycopg2.extras.DictCursor) as cursor:
        try:
            cursor.execute(sentencia, {'precio': precio})
            rows = cursor.fetchall()
            for row in rows:
                precio = row['prezoart'] if row['prezoart'] else 'Desconocido'
                print(f"Código: {row['codart']}. Nombre: {row['nomart']}. Precio: {precio}")
            
            print(f"Hay {cursor.rowcount} artículos que cuestan más de {sprecio}.") # usamos sprecio y no precio ya que precio se ha modificado al imprimir las filas, ya no es el dado, sino el último impreso
            conn.commit()
        except psycopg2.Error as e:
            print(f"Error {e.pgcode}: {e.pgerror}")
            conn.rollback()


## ------------------------------------------------------------
def update_row(conn):
    """
    Pide por teclado el código. Si el artículo existe, muestra sus detalles,
    pide el nuevo nombre y precio y actualiza el artículo.
    :param conn: la conexión abierta a la bd
    :return: Nada
    """
    
    conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_READ_COMMITTED
    codigo = show_row(conn, control_tx=False)
    
    if codigo is None:
        conn.rollback()
        return
    
    snombre = input('Nuevo nombre: ')
    nombre = None if snombre == "" else snombre
    
    sprecio = input('Nuevo precio: ')
    precio = None if sprecio == "" else float(sprecio)
    
    sentencia = """
        update artigo
        set nomart = %(nombre)s, prezoart = %(precio)s
        where codart = %(codigo)s
    """
    
    with conn.cursor() as cursor:
        try:
            cursor.execute(sentencia, {'nombre': nombre, 'precio': precio, 'codigo': codigo})
            conn.commit()
            print(f"Artículo actualizado.")
        except psycopg2.Error as e:
            if e.pgcode == psycopg2.errorcodes.NOT_NULL_VIOLATION:
                print(f"El nombre del artículo es obligatorio.")
            elif e.pgcode == psycopg2.errorcodes.CHECK_VIOLATION:
                print(f"El precio debe ser positivo.")
            else:
                print(f"Error {e.pgcode}: {e.pgerror}")
            conn.rollback()


## ------------------------------------------------------------
def update_price(conn):
	"""
	Pide por teclado el código. Si el artículo existe, muestra sus detalles,
	pide un porcentaje de incremento de precio y actualiza el artículo.
	:param conn: la conexión abierta a la bd
	:return: Nada
	"""

	# Este caso lo hacemos con SERIALIZABLE por motivos didácticos
	conn.isolation_level = psycopg2.extensions.ISOLATION_LEVEL_SERIALIZABLE
	
	codigo = show_row(conn, control_tx=False)
	if codigo is None:
		conn.rollback()
		return
		
	sincremento = input('Incremento (%): ')
	incremento = None if sincremento == "" else float(sincremento)
	
	sentencia = """
		update artigo
		set prezoart = prezoart + prezoart * %(inc)s / 100.0
		where codart = %(codigo)s
	"""
	
	with conn.cursor() as cursor:
		try:
			cursor.execute(sentencia, {'inc': incremento, 'codigo': codigo})
			# Esta línea tiene un fin didáctico (no es funcional)
			# Mientras está en pausa la transacción, podemos abrir otra terminal
			# y forzar un SERIALIZATION_FAILURE
			input("PULSA ENTER")
			conn.commit()
			print("Artículo actualizado")
		except psycopg2.Error as e:
			if e.pgcode == psycopg2.errorcodes.SERIALIZATION_FAILURE:
				print("El precio del artículo fue modificado por otro usuario y no corresponde con el precio mostrado.")
			elif e.pgcode == psycopg2.errorcodes.CHECK_VIOLATION:
				print("El precio debe ser positivo.")
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
3 - Añadir artículo
4 - Eliminar artículo
5 - Eliminar artículos por nombre
6 - Contar artículos
7 - Mostrar artículo
8 - Listar todos los artículos
9 - Mostrar por precio
u - Actualizar artículo
p - Actualizar precio
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
        elif tecla == '3':
            add_row(conn)
        elif tecla == '4':
            delete_row(conn)
        elif tecla == '5':
            delete_by_name_fragment(conn)
        elif tecla == '6':
            count_articles(conn)
        elif tecla == '7':
            show_row(conn)
        elif tecla == '8':
            show_all_rows(conn)
        elif tecla == '9':
            show_by_price(conn)
        elif tecla == 'u':
            update_row(conn)
        elif tecla == 'p':
            update_price(conn)
            

## ------------------------------------------------------------
def main():
    """
    Función principal. Conecta á bd e executa o menú.
    Cando sae do menú, desconecta da bd e remata o programa
    """
    print('Conectando a PosgreSQL...')
    conn = connect_db()
    print('Conectado.')
    menu(conn)
    disconnect_db(conn)

## ------------------------------------------------------------

if __name__ == '__main__':
    main()
