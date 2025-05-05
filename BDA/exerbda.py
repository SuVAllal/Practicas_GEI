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
# 2. Obtenemos una conexión con la base de datos
def connect_db():
    """
     Se conecta a la BD predeterminada del usuario (DSN vacío)
     :return: La conexión con la BD (o se sale del programa al no conseguirla)
     """
    try:
        conn = psycopg2.connect("")
        conn.autocommit = False
        return conn
    except psycopg2.Error(e) as e:
        print(f"Error de conexión: {e}")
        sys.exit(1)


## ------------------------------------------------------------
def disconnect_db(conn):
    """
    Se desconecta de la BD.
    :param conn: la conexión abierta a la BD
    :return: nada
    """
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
    try:
        cur = conn.cursor() # Para ejecutar sentencias SQL hay que crear un cursor a partir de la conexión
        cur.execute(sentencia_create)
        conn.commit()
        print("Tabla artigo creada")
    except psycopg2.Error(e) as e:
        if e.pgcode == psycopg2.errorcodes.DUPLICATE_TABLE:
            print(f"La tabla artigo ya existe. No se crea.")
        else:
            print(f"Error {e.pgcode}: {e.pgerror}")
        conn.rollback()
    finally:
        cur.close()


## ------------------------------------------------------------
def menu(conn):
    """
    Imprime un menú de opcións, solicita a opción e executa a función asociada.
    'q' para saír.
    """
    MENU_TEXT = """
      -- MENÚ --
1 - Crear táboa artigo   
q - Saír   
"""
    while True:
        print(MENU_TEXT)
        tecla = input('Opción> ')
        if tecla == 'q':
            break
        elif tecla == '1':
            create_table(conn)  
            
            
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
