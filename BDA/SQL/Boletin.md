# Boletín de ejercicios de SQL
## 1. Introducción
#### 1. Empezaremos por crear varios elementos en nuestro usuario de Oracle. Usaremos los elementos más adelante, y también nos permitirá hacer alguna consulta sobre el catálogo de Oracle. Crea la tabla `artigo` con campos `codart numeric(3), nomart varchar(20)` y `prezoart numeric(5, 2)`. La clave primaria es `codart`.
```SQL
CREATE TABLE artigo (
	codart NUMERIC(3),
	nomart VARCHAR(20),
	prezoart NUMERIC(5,2),
	CONSTRAINT pk_artigo PRIMARY KEY (codart)
);
```

#### 2. Crea una tabla `venda` con los siguientes campos. Ningún campo debe admitir nulos. Recuerda nombrar las restricciones (puedes omitir el nombre para las not null).
```
IDVEN NUMBER(6), clave primaria
CODART NUMBER(3), clave foránea referenciando ARTIGO, borrado en cascada
PREZOVENT NUMBER(5,2)
CANTVEN NUMBER(3)
DATA DATE, debe tener como valor predeterminado la fecha actual
```

```SQL
CREATE TABLE venda (
	idven NUMBER(6),
	codart NUMBER(3) NOT NULL,
	prezoven NUMBER(5,2) NOT NULL,
	cantven NUMBER(3) NOT NULL,
	data DATE DEFAULT SYSDATE NOT NULL,
	CONSTRAINT pk_venda PRIMARY KEY (idven),
	CONSTRAINT fk_venda_codart FOREIGN KEY (codart) REFERENCES artigo(codart) ON DELETE CASCADE
);
```

#### 3. Oracle utiliza secuencias (`SEQUENCE`) para generar números de forma similar a los "auto increment" de otros gestores. Crea una secuencia de nombre `SEQ_ARTIGO`. La sintaxis básica para la creación es:
```SQL
CREATE SEQUENCE <nome>
	[MINVALUE <valor-min>] [MAXVALUE <valor-max>]
	[START WITH <valor-inicial>] [INCREMENT BY <incremento>]
```
#### Puedes dejar todos los parámetros en su valor predeterminado.
#### Para usar la secuencia, usaremos `SEQ_ARTIGO.NEXTVAL` para obtener un nuevo número, y `SEQ_ARTIGO.CURRVAL` para recuperar el último generado (en una sesión es obligatorio usar por lo menos una vez `NEXTVAL` antes de usar `CURRVAL`).
```SQL
CREATE SEQUENCE seq_artigo;
```

#### 4. Usa la secuencia para generar el código del artículo e inserta las filas para los siguientes productos, con sus precios:
```
Folios 3.75
Calculadora 17.00
Ordenador <desconocido>
```
#### Haz `commit` para confirmar la transacción, y comprueba los valores insertados.
```SQL
INSERT INTO artigo (codart, nomart, prezoart) VALUES (seq_artigo.nextval, 'Folios', 3.75);
INSERT INTO artigo (codart, nomart, prezoart) VALUES (seq_artigo.nextval, 'Calculadora', 17.00);
INSERT INTO artigo (codart, nomart, prezoart) VALUES (seq_artigo.nextval, 'Ordenador', null);

COMMIT;

SELECT * FROM artigo;

    CODART NOMART                 PREZOART
---------- -------------------- ----------
        21 Folios                     3,75
        22 Calculadora                  17
        23 Ordenador
```

**¿Qué está pasando?** Permite añadir el elemento 'Ordenador' aunque no digamos el precio ya que en la creación de la tabla la única restricción de `NOT NULL` es para la clave primaria `CODART`.

#### 5. Crea una vista de nombre `VART4` que obtenga todos los datos de los artículos que tengan un precio superior a 4.
```SQL
CREATE VIEW vart4
	AS SELECT *
		FROM artigo
		WHERE prezoart > 4;
```

## 2. Catálogo
#### 1. La vista que almacena información sobre el propio catálogo de Oracle es `DICTIONARY` o su sinónimo `DICT`. Examina su estructura y comprueba si aparecen `USER_TABLES, ALL_TABLES` y `DBA_TABLES`.
```SQL
DESC dict;
-- También es válido:
DESCRIBE dict;

-- Muestra lo siguiente:
        Nombre             ¿Nulo?        Tipo
------------------------- -------- -----------------
 TABLE_NAME                          VARCHAR2(128)
 COMMENTS                            VARCHAR2(4000)

-- El contenido del diccionario de datos siempre está en mayúsculas
SELECT * FROM dict WHERE table_name='USER_TABLES';
TABLE_NAME
-------------------------------------------------
COMMENTS
-------------------------------------------------
USER_TABLES

SELECT * FROM dict WHERE table_name='ALL_TABLES';
TABLE_NAME
-------------------------------------------------
COMMENTS
-------------------------------------------------
ALL_TABLES

SELECT * FROM dict WHERE table_name='DBA_TABLES';
ninguna fila seleccionada
```

**¿Qué está pasando?** El usuario actual tiene acceso a las vistas `USER_TABLES` y `ALL_TABLES`, por eso aparecen en `DICT`. Sin embargo, no tiene privilegios para acceder a `DBA_TABLES`, que solo está disponible para usuarios con permisos de administrador (por ejemplo, con rol `DBA`).

Además, aunque tengamos creadas las tablas `artigo, venta` y la vista `vart4`, no aparecen ya que `DICT` no muestra las tablas y vistas del usuario, sino solo las vistas del diccionario del sistema (el catálogo). Las tablas `artigo, venta` y la vista `vart4` están creadas en el esquema del usuario, no en el diccionario del sistema/catálogo. Podríamos consultarlas usando: `USER_TABLES, USER_VIEWS` o `USER_OBJECTS`.

#### 2. En Oracle podemos ver información sobre los objetos de nuestro usuario mediante diversas vistas. Comprueba las diferencias que hay entre `TAB, USER_TABLES/TABS, USER_CATALOG/CAT`. Haz uso de DESCRIBE y SELECT para comprobarlo.
```SQL
DESC tab;
Nombre             ¿Nulo?  Tipo
 ---------------- -------- -------------------------------
 TNAME            NOT NULL VARCHAR2(128)
 TABTYPE                   VARCHAR2(13)
 CLUSTERID                 NUMBER

DESC tabs;
 Nombre            ¿Nulo?  Tipo
 ---------------- -------- --------------------------------------------
 TABLE_NAME       NOT NULL VARCHAR2(128)
 TABLESPACE_NAME           VARCHAR2(30)
 CLUSTER_NAME              VARCHAR2(128)
 IOT_NAME                  VARCHAR2(128)
 STATUS                    VARCHAR2(8)
 PCT_FREE                  NUMBER
 PCT_USED                  NUMBER
-- ...

DESC cat;
Nombre             ¿Nulo?  Tipo
 ------------ --- -------- --------------------------------------------
 TABLE_NAME       NOT NULL VARCHAR2(128)
 TABLE_TYPE                VARCHAR2(11)
 
SELECT * FROM tab;
-- Muestra el contenido de las columnas TNAME, TABTYPE y CLUSTERID

SELECT * FROM tabs;
-- Muestra múltiples columnas como: TABLE_NAME, TABLESPACE_NAME, STATUS, CACHE, BLOCKS... etc.

SELECT * FROM cat;
-- Muestra el contenido de las columnas TABLE_NAME y TABLE_TYPE
TABLE_NAME                TABLE_TYPE
------------------------- ----------------
SEQ_ARTIGO                SEQUENCE
VART4                     VIEW
-- ...
```

**¿Qué diferencias hay entre `TAB, TABS/USER_TABLES, CAT`?** 
- TAB:
	- Es una vista que muestra todas las tablas y vistas del usuario.
	- No muestra detalles técnicos como espacio en disco o estado.
- TABS o USER_TABLES:
	- Muestra **solo** las tablas del usuario.
	- Contiene información más detallada.
- CAT o USER_CATALOG:
	- Vista que muestra tablas, vistas y **secuencias** del usuario.
	- No muestra detalles técnicos.

| Vista  | ¿Qué muestra?               | Objetos incluidos     | Nivel de detalle | Posibles usos                                                                                                                                   |
| ------ | --------------------------- | --------------------- | ---------------- | ----------------------------------------------------------------------------------------------------------------------------------------------- |
| `TAB`  | Tablas y vistas             | TABLE, VIEW           | Básico           | Visión rápida y simple de tablas y vistas.<br>Útil para saber qué objetos tenemos sin ver más detalles.                                         |
| `TABS` | Solo tablas                 | TABLE                 | Detallado        | Detalle técnico para gestión, optimización y mantenimiento de tablas. <br>Importante para DBA o para hacer auditorías de espacio y rendimiento. |
| `CAT`  | Tablas, vistas y secuencias | TABLE, VIEW, SEQUENCE | Básico           | Inventario general de tablas, vistas y secuencias del esquema.<br>Útil para ver rápidamente todos los tipos de objetos que tenemos.             |

#### 3. La vista `USER_TAB_COLUMNS` y su sinónimo `COLS` contienen información sobre las columnas. Obtén mediante ellas información sobre las columnas de la tabla artigo. El resultado debe ser similar a este:
```
NOMBRE   TIPO      NULOS?
-------  -----     ----------- 
CODART   NUMBER    N 
NOMART   VARCHAR2  Y 
PREZOART NUMBER    Y
```

```SQL
-- Para tener el mismo formato que el resultado dado:
column NOMBRE format a30
column TIPO format a20
column "NULOS?" format a10

SELECT column_name as NOMBRE, DATA_TYPE as TIPO, NULLABLE as "NULOS?"
FROM cols
WHERE table_name='ARTIGO';
```

#### 4. La vista `USER_OBJECTS` y su sinónimo `OBJ` almacenan información sobre los objetos del usuario. Examina la estructura de esa vista y obtén el nombre y tipo de tus objetos.
```SQL
SELECT OBJECT_NAME as NOMBRE, OBJECT_TYPE as TIPO
FROM obj;
```

## 3. VISTAS
#### 1. Inserta la fila `(6, 'Libro BD', 1)` a través de la vista `VART4`. ¿Qué ocurre? Anula la transacción.
```SQL
INSERT INTO vart4 VALUES(6, 'Libro BD', 1);
-- Inserta la fila, generando una fila migratoria
SELECT * FROM vart4;
    CODART NOMART                 PREZOART
---------- -------------------- ----------
        22 Calculadora                  17
-- Aunque la hayamos insertado a través de la vista no aparece, ya que su precio es menor que 4.
SELECT * FROM artigo;
    CODART NOMART                 PREZOART
---------- -------------------- ----------
        21 Folios                     3,75
        22 Calculadora                  17
        23 Ordenador
         6 Libro BD                      1
-- En cambio, al mirar la tabla completa, sí está insertada (fila migratoria).

ROLLBACK;

SELECT * FROM artigo;
    CODART NOMART                 PREZOART
---------- -------------------- ----------
        21 Folios                     3,75
        22 Calculadora                  17
        23 Ordenador
-- Después de anular la transacción ya no aparece, pues no se han "guardado" los cambios.
```

#### 2. Cambia la vista `VART4` que muestra los artículos de más de 4 euros, de forma que la vista impida insertar filas de artículos que no cumplan con esa restricción (no queremos filas migratorias), y comprueba que efectivamente funciona tratando de insertar de nuevo la fila `(6, 'Libro BD', 1)`.
Para modificar vistas en Oracle **NO** se usa `ALTER VIEW`, sino `CREATE OR REPLACE VIEW`.

```SQL
CREATE OR REPLACE VIEW vart4 
AS SELECT *
	FROM artigo
	WHERE prezoart > 4
	WITH CHECK OPTION;

INSERT INTO vart4 VALUES(6, 'Libro BD', 1);
            *
ERROR en línea 1:
ORA-01402: violación de la cláusula WHERE en la vista WITH CHECK OPTION
```

## 4. Restricciones
#### 1. Asegúrate de que las tablas `artigo` y `venda` no contienen ninguna fila. Añade una restricción, de nombre `ch_art_prezo_pos` que garantice que el precio de los artículos es positivo. La restricción debe ser aplazable e inicialmente aplazada.

```SQL
-- Vaciamos el contenido de artigo (venda ya está vacía)
DELETE FROM artigo;

-- Añadimos la restricción
ALTER TABLE artigo
ADD CONSTRAINT ch_art_prezo_pos CHECK (prezoart > 0)
DEFERRABLE INITIALLY DEFERRED; -- la hacemos aplazable e inicialmente aplazada
```

#### 2. Confirma, buscando en el catálogo, que la restricción está correctamente creada.
```SQL
-- Así vemos todas las restricciones de la tabla 'ARTIGO'
SELECT constraint_name, contraint_type, deferrable, deferred
FROM user_constraints
WHERE table_name='ARTIGO';

CONSTRAINT_NAME          C DEFERRABLE     DEFERRED
----------------------   - -------------- ---------
CH_ART_PREZO_POS         C DEFERRABLE     DEFERRED   -- C de CHECK
PK_ARTIGO                P NOT DEFERRABLE IMMEDIATE  -- P de PRIMARY

-- O también para ver concretamente esa restricción:
SELECT constraint_name, contraint_type, deferrable, deferred
FROM user_constraints
WHERE contraint_name='CH_ART_PREZO_POS';

CONSTRAINT_NAME          C DEFERRABLE     DEFERRED
----------------------   - -------------- ---------
CH_ART_PREZO_POS         C DEFERRABLE     DEFERRED
```

#### 3. Para los siguientes ejercicios, intenta explicar qué es lo que sucede y por qué. Añade la fila `(1, Folios, 3.75)` a la tabla artigo.
>**NOTA:** Para que todos tengamos los mismos valores, no vamos a usar la secuencia creada en los siguientes ejercicios.

```SQL
INSERT INTO artigo (codart, nomart, prezoart) VALUES (1, 'Folios', 3.75);
```

**¿Qué ocurre cuando se ejecuta la sentencia?**
- El SGBD comprueba que Artigo existe y que es una tabla válida del usuario.
- Comprueba que el usuario tiene permisos de `INSERT` sobre Artigo.
- Comprueba que los nombres de las columnas `(codart, nomart, prezoart)` existen en la tabla.
- Comprueba que los valores tienen tipos compatibles: `1` número, `'Folios'` cadena, `3.75` número con decimales.
- Comprueba que se cumplen las restricciones **inmediatas**: `NOT NULL` en la clave primaria.
- Como la restricción sobre el precio está desplazada no se comprueba en ese momento, sino al hacer `COMMIT`.
- La fila se inserta temporalmente aunque la restricción `DEFERRED` se cumpla o no.

#### 4. Confirma los cambios de la transacción actual.
```SQL
COMMIT;
```

**¿Qué está pasando en la base de datos?**
- El SGBD comprueba si hay modificaciones pendientes (`INSERT, UPDATE` o `DELETE`) en la sesión actual.
- Para cada fila modificada, comprueba que se cumplen **todas** las restricciones, incluidas las `DEFERRED` como `prezoart > 0`.
- Como todas las restricciones se cumplen, los cambios son confirmados permanentemente: la transacción actual termina, los datos dejan de estar en estado "temporal" y el resto de usuarios ya pueden ver los cambios.
