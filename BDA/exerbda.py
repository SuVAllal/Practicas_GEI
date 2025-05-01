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
    Crea a táboa artigo (codart, nomart, prezoart)
    :param conn: a conexión aberta á bd
    :return: Nada
    """
    print("Táboa artigo creada")


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
