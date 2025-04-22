# Boletín PostgreSQL
## Ejercicio 2
Crea un usuario en PostgreSQL identificado por una clave y con el privilegio createdb. Crea una base de datos con ese usuario.
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
