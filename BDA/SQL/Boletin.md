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
	AS SELECT codart, nomart, prezoart
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
SELECT constraint_name, constraint_type, deferrable, deferred
FROM user_constraints
WHERE table_name='ARTIGO';

CONSTRAINT_NAME          C DEFERRABLE     DEFERRED
----------------------   - -------------- ---------
CH_ART_PREZO_POS         C DEFERRABLE     DEFERRED   -- C de CHECK
PK_ARTIGO                P NOT DEFERRABLE IMMEDIATE  -- P de PRIMARY

-- O también para ver concretamente esa restricción:
SELECT constraint_name, constraint_type, deferrable, deferred
FROM user_constraints
WHERE constraint_name='CH_ART_PREZO_POS';

CONSTRAINT_NAME          C DEFERRABLE     DEFERRED
----------------------   - -------------- ---------
CH_ART_PREZO_POS         C DEFERRABLE     DEFERRED
```

#### 3. Para los siguientes ejercicios, intenta explicar qué es lo que sucede y por qué. Añade la fila `(1, Folios, 3.75)` a la tabla artigo.
>**NOTA:** Para que todos tengamos los mismos valores, no vamos a usar la secuencia creada en los siguientes ejercicios.

```SQL
INSERT INTO artigo (codart, nomart, prezoart) VALUES (1, 'Folios', 3.75);
1 fila creada.
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
Confirmación terminada.
```

**¿Qué está pasando en la base de datos?**
- El SGBD comprueba si hay modificaciones pendientes (`INSERT, UPDATE` o `DELETE`) en la sesión actual.
- Para cada fila modificada, comprueba que se cumplen **todas** las restricciones, incluidas las `DEFERRED` como `prezoart > 0`.
- Como todas las restricciones se cumplen, los cambios son confirmados permanentemente: la transacción actual termina, los datos dejan de estar en estado "temporal" y el resto de usuarios ya pueden ver los cambios.

#### 5. Añade la fila `(2, Pluma, 0)`.
```SQL
INSERT INTO artigo(codart, nomart, prezoart) VALUES (2, 'Pluma', 0);
1 fila creada

-- Podemos comprobar que está insertada TEMPORALMENTE
SELECT * FROM artigo;
    CODART NOMART                 PREZOART
---------- -------------------- ----------
         1 Folios                     3,75
         2 Pluma                         0
```

- El SGBD comprueba que la tabla Artigo existe y que el usuario tiene permiso de inserción.
- Comprueba que los nombres de las columnas son correctos.
- Comprueba que se cumplen las restricciones **inmediatas**.
- La restricción `prezoart > 0` no se comprueba de momento, porque está `DEFERRED`.
- La fila se inserta temporalmente.

#### 6. Confirma los cambios de la transacción actual.
```SQL
COMMIT;
*
ERROR en línea 1:
ORA-02091: transacción con rollback
ORA-02290: restricción de control (SCOTT.CH_ART_PREZO_POS) violada

-- Podemos comprobar que efectivamente, NO se insertó
SELECT * FROM artigo;
    CODART NOMART                 PREZOART
---------- -------------------- ----------
         1 Folios                     3,75
```

- El SGBD detecta que hay modificaciones pendientes en la transacción.
- Antes de confirmar los cambios comprueba **todas** las restricciones, incluidas las `DEFERRED`.
- Comprueba la restricción `CHECK (prezoart > 0)`: **No se cumple**, lo que provoca una violación de la restricción. Oracle lanza un error (`ORA-02290`) y se cancelan **todos** los cambios de la transacción con un `ROLLBACK`.

#### 7. Inserta las filas siguientes: `(2, Pluma, 80)` y `(3, Libreta, 0)`. Examina las filas que hay en `artigo`.
```SQL
INSERT INTO artigo(codart, nomart, prezoart) VALUES (2, 'Pluma', 80);
INSERT INTO artigo(codart, nomart, prezoart) VALUES (3, 'Libreta', 0);

SELECT * FROM artigo;
    CODART NOMART                 PREZOART
---------- -------------------- ----------
         1 Folios                     3,75
         2 Pluma                        80
         3 Libreta                       0
```

Se sigue el mismo proceso que en los ejercicios anteriores, y se insertan temporalmente ya que ambas cumplen las restricciones inmediatas de la tabla.

#### 8. Actualiza el precio de la libreta a 3,20. Confirma los cambios.
```SQL
UPDATE artigo
SET prezoart = 3.20
WHERE nomart = 'Libreta';
1 fila actualizada.

COMMIT;
Confirmación terminada.

SELECT * FROM artigo;
    CODART NOMART                 PREZOART
---------- -------------------- ----------
         1 Folios                     3,75
         2 Pluma                        80
         3 Libreta                     3,2
```

- El SGBD busca la fila en Artigo donde `nomart = 'Libreta'`.
- La encuentra y actualiza el valor `prezoart` a `3.20`.
- Como la restricción `prezoart > 0` es `DEFERRED` **no se hace ninguna comprobación en ese momento**, igual que cuando la insertamos.
- Al hacer el `COMMIT` el SGBD comprueba que se cumplen todas las restricciones, inmediatas y desplazadas.
- Como todas las filas cumplen con la restricción de `prezoart` no hay errores, se realiza el `COMMIT` y se confirman los cambios convirtiéndose en permanentes y terminando la transacción.
> Es importante destacar que el `UPDATE` corrigió la violación de la restricción de `prezoart`.

#### 9. Crea una tabla `proba` con un campo `id` de tipo `int`, clave primaria.
```SQL
CREATE TABLE proba (
	id NUMBER(3),
	CONSTRAINT pk_proba PRIMARY KEY (id)
);
```

#### 10. Inserta en `artigo` la fila `(4, Boli, 0.9)`. Inserta en la tabla `proba` la fila `(1)`. Inserta en `artigo` la fila `(5, Calculadora, -3)`. Confirma los datos. ¿Qué ocurre? ¿Hay datos en `prueba`?
```SQL
INSERT INTO artigo (codart, nomart, prezoart) VALUES (4, 'Boli', 0.9);
INSERT INTO proba (id) VALUES (1);
INSERT INTO artigo (codart, nomart, prezoart) VALUES (5, 'Calculadora', -3);

SELECT * FROM proba;
        ID
----------
         1

SELECT * FROM artigo;
    CODART NOMART                 PREZOART
---------- -------------------- ----------
         1 Folios                     3,75
         2 Pluma                        80
         3 Libreta                     3,2
         4 Boli                         ,9
         5 Calculadora                  -3

COMMIT;
*
ERROR en línea 1:
ORA-02091: transacción con rollback
ORA-02290: restricción de control (SCOTT.CH_ART_PREZO_POS) violada
```

- El SGBD ejecuta los `INSERT` correctamente. (La fila con 'Boli' y la de `prueba` son válidas, pero la de 'Calculadora' no, pero al ser una restricción desplazada se acepta temporalmente).
- Al ejecutar el `COMMIT`, Oracle:
	- Revisa todas las restricciones, inmediatas y desplazadas.
	- Encuentra la violación de la restricción en la fila de 'Calculadora', lanza un error y provoca un `ROLLBACK` de la **transacción completa**, deshaciendo **todos los cambios** hechos en esa transacción (es decir, los tres `INSERT` aunque los otros dos fueran correctos, ninguno de ellos se hace permanente).

```SQL
SELECT * FROM proba;
-- desapareció la fila insertada a pesar de ser correcta
ninguna fila seleccionada.

SELECT * FROM artigo;
-- lo mismo aquí, desapareció la correcta y la incorrecta
    CODART NOMART                 PREZOART
---------- -------------------- ----------
         1 Folios                     3,75
         2 Pluma                        80
         3 Libreta                     3,2
```

#### 12. Intenta insertar la calculadora de nuevo, con el mismo precio.
```SQL
INSERT INTO artigo (codart, nomart, prezoart) VALUES (5, 'Calculadora', -3);
1 fila creada.
```

Se inserta temporalmente la fila.

#### 13. Pon la restricción `ch_art_prezo_pos` en modo inmediato.
```SQL
SET CONSTRAINT ch_art_prezo_pos IMMEDIATE; -- Afecta solo a la transacción actual, cuando termine la transacción volverá a ser DEFERRED
*
ERROR en línea 1:
ORA-02290: restricción de control (SCOTT.CH_ART_PREZO_POS) violada

SELECT * FROM artigo;
    CODART NOMART                 PREZOART
---------- -------------------- ----------
         1 Folios                     3,75
         2 Pluma                        80
         3 Libreta                     3,2
         5 Calculadora                  -3
```

- Al cambiar el modo de la restricción, Oracle obliga a comprobar **en ese momento** todas las filas afectadas por esa restricción en la transacción actual.
- Encuentra que la fila de 'Calculadora' no cumple la restricción.
- Oracle lanza un error indicando que se violó la restricción, **pero no hace `ROLLBACK`, la transacción sigue abierta** y los datos incorrectos siguen presentes, pero no se confirmarán a menos que corrijamos el error.

>**NOTA:** Si repetimos el ejercicio anterior poniendo la restricción en modo inmediato, las filas correctas de 'Boli' y `proba` sí se insertarían, pero la incorrecta de 'Calculadora' no, es decir, desaparecerían solo las filas incorrectas.

#### 14. Modifica el precio de la calculadora, poniendo 10. Vuelve a intentar poner la restricción en modo inmediato. Confirma los cambios.
```SQL
UPDATE artigo
SET prezoart = 10
WHERE nomart = 'Calculadora';
1 fila actualizada.

SET CONSTRAINT ch_art_prezo_pos IMMEDIATE;
Restricción definida.
-- Fijarse que ahora no salta ningún error o violación de la restricción debido a que corregimos el error

COMMIT;
Confirmación terminada.
```

> En el momento del `COMMIT` Oracle comprueba las restricciones pendientes (es decir, las desplazadas, pues las inmediatas se comprueba en el momento en el que se ejecuta la sentencia a ejecutar), es decir, cuando se cambió la restricción a `IMMEDIATE` Oracle hizo las comprobaciones, entonces en el momento del `COMMIT` no vuelve a repetir esas comprobaciones a menos que detecte cambios sin comprobar (restricciones desplazadas).

Al terminar la transacción con el `COMMIT` la restricción vuelve automáticamente a su modo por defecto: `DEFERRED`.

#### 15. El estándar SQL permite especificar, para las claves foráneas (solo tiene sentido para las compuestas de varios atributos), la cláusula `MATCH` con las opciones `SIMPLE, PARTIAL` y `FULL`. Oracle no permite esta especificación, pero su comportamiento corresponde a una de esas 3 opciones. Averigua, creando tablas e insertando datos, a qué opción corresponde.
Recordemos cómo funciona cada opción del `MATCH`:
- `MATCH SIMPLE`: si existe un nulo, acepta la clave aunque algún otro valor no sea correcto. Si todos los valores son correctos, la acepta.
- `MATCH PARTIAL`: si existe un nulo, pero alguno de los valores de la clave foránea no es correcto, no lo acepta. Si todos los valores son correctos, la acepta.
- `MATCH FULL`: solo acepta si todos los valores son correctos o si todos son nulos.

Es decir, para saber qué opción de `MATCH` usa Oracle, podemos probar las siguientes combinaciones:
- Valor correcto, valor correcto (así puede ser cualquiera de los tres).
- NULL, NULL (así puede ser cualquiera de los tres).
- Valor incorrecto, valor incorrecto (así puede ser cualquiera de los tres).
- Valor correcto, NULL (así solo puede ser `SIMPLE` o `PARTIAL`).
- Valor incorrecto, NULL (así solo puede ser `SIMPLE`, si no la acepta es `PARTIAL` o `FULL`).

```SQL
CREATE TABLE maestra (
	a number(3),
	b number(3),
	CONSTRAINT pk_maestra PRIMARY KEY (a,b)
);

CREATE TABLE esclava (
	c number(3),
	a number(3),
	b number(3),
	CONSTRAINT fk_maestra FOREIGN KEY (a,b) REFERENCES maestra(a,b)
);

INSERT INTO maestra VALUES (1, 2);
INSERT INTO esclava VALUES (101, 1, 2); -- Funciona, puede ser cualquiera de los tres
INSERT INTO esclava VALUES (101, NULL, 2); -- Funciona, NO es FULL
INSERT INTO esclava VALUES (101, NULL, NULL); -- Funciona, puede ser cualquiera de los tres
INSERT INTO esclava VALUES (101, 3, 4); -- No funciona, violación de clave foránea
INSERT INTO esclava VALUES (101, 5, NULL); -- Funciona, Oracle usa MATCH SIMPLE
```

> **NOTA:** Oracle usa como predeterminado `MATCH SIMPLE`, pero también tiene implementado el `MATCH FULL`.

## 5. Papelera
#### 1. Elimina por completo la tabla `proba` (tanto el contenido como la definición). Examina las tablas usando `TAB`.
```SQL
DROP TABLE proba;
Tabla borrada.

SELECT * FROM TAB WHERE TNAME = 'PROBA';
ninguna fila seleccionada -- No aparece en las tablas del usuario, se ha eliminado
```

#### 2. Mira si hay algo en la papelera de reciclaje. Restaura la tabla `proba` y examina de nuevo `TAB`.
```SQL
SELECT * FROM recyclebin WHERE original_name = 'PROBA';

FLASHBACK TABLE proba TO BEFORE DROP;
Flashback terminado.

SELECT * FROM TAB WHERE TNAME = 'PROBA';
-- Ahora sí aparece, ya no está eliminada
TNAME        TABTYPE        CLUSTERID
------------ ------------- ----------
PROBA        TABLE
```

#### 3. Elimina por completo la tabla `proba` (contenido y definición) de forma que desaparezca (que no quede en la papelera). Puedes hacerlo de dos formas distintas.
```SQL
-- Primera forma:
DROP TABLE proba PURGE; -- No se puede recuperar con FLASHBACK TABLE

-- Segunda forma:
ALTER SESSION SET recyclebin = OFF; -- Desactiva temporalmente la papelera, por lo que al borrar la tabla esta no irá allí
DROP TABLE proba;
```

#### 4. Vacía la papelera de reciclaje (aunque ya esté vacía).
```SQL
PURGE recyclebin;

SELECT * FROM recyclebin;
ninguna fila seleccionada.
```

## 6. Actividad
#### 1. Oracle 19c implementa la cláusula `generated [always] as identity`, que internamente usa una secuencia y un trigger para generar valores para una columna. Vamos a hacerlo manualmente: utiliza la secuencia `seq_artigo` y trata de crear un trigger para simular ese comportamiento. Comprueba que funciona.
```SQL
CREATE OR REPLACE TRIGGER t_artigo_codart
	BEFORE INSERT ON artigo
	FOR EACH ROW
	BEGIN
		:new.codart := seq_artigo.nextval;
	END;
/

Disparador creado

INSERT INTO artigo(nomart, prezoart) VALUES ('Cuaderno', 1.25);
*
ERROR en línea 1:
ORA-00001: restricción única (SCOTT.SYS_C00185534) violada

SELECT seq_artigo.currval from dual;
   CURRVAL
----------
         2
         

-- ...

SELECT seq_artigo.currval from dual;
   CURRVAL
----------
         3

INSERT INTO artigo(nomart, prezoart) VALUES ('Cuaderno', 1.25);
1 fila creada -- ahora sí funciona

-- ¡CUIDADO! Como la secuencia empieza en 1 y tenemos ya los valores '1', '2' y '3' insertados en la tabla, dará error las primeras veces (hasta que el seq_artigo.currval = 4).
```

**¿Qué hace el trigger?**
- Se activa antes de cada `INSERT` sobre `artigo`.
- Asigna automáticamente un valor a `codart` usando la secuencia `seq_artigo`.

**⚠️ Cosas a tener en cuenta:**
- Debe ser un trigger `BEFORE UPDATE` para interceptar el cambio antes de que falle por la restricción.
#### 2. Oracle no implementa la acción referencial `ON UPDATE CASCADE`, pero podemos simular esa acción utilizando un trigger. Crea un trigger para que la clave foránea de `venda` que referencia a `artigo` implemente la acción referencial de actualización en cascada.
```SQL
CREATE OR REPLACE TRIGGER t_venda_on_update
	BEFORE UPDATE ON artigo
	FOR EACH ROW
	BEGIN
		UPDATE venda
		SET codart = :new.codart
		WHERE codart = :old.codart;
	END;
/	
```

**¿Qué hace el trigger?**
- Se actualiza **antes** de actualizar `artigo`.
- Cuando se cambia `codart` en `artigo`, busca en `venda` todas las filas con ese valor antiguo y las actualiza al nuevo.

**⚠️ Cosas a tener en cuenta:**
- Debe ser un trigger `BEFORE UPDATE` para interceptar el cambio antes de que falle por la restricción.
- Esto funcionará siempre que:
	- No haya restricciones que impidan temporalmente los cambios en `venda` (como claves ajenas en otras tablas).
	- `codart` no sea la clave primaria en `venda` (o si lo es, que la actualización no cause duplicados).

## 7. Seguridad
#### 1. En la base de datos de docencia están creados los usuarios `u1` y `u2`, ambos con `clave` como contraseña. Tu usuario tiene creada la tabla `artigo`. Da permisos al usuario `u1` para seleccionar de la tabla, con la potestad de pasárselo a otros.
```SQL
GRANT SELECT ON artigo TO u1 WITH GRANT OPTION;
Concesión terminada correctamente.
```

#### 2. Comprueba que `u1` puede acceder a la tabla. Haz que `u1` otorgue el permiso de selección sobre la tabla `artigo` a `u2`. Comprueba que `u2` puede acceder.
```bash
# Para acceder al usuario 1, desde terminal:
rlwrap sqlplus u1/clave@BDDOCENCIA
```
```SQL
GRANT SELECT ON scott.artigo TO u2;
Concesión terminada correctamente.

-- Ahora, desde 'u2':
SELECT * FROM scott.artigo;
    CODART NOMART                 PREZOART
---------- -------------------- ----------
         2 Pluma                        80
         3 Libreta                     3,2
         1 Folios                     3,75
         4 Cuaderno                   1,25
```

#### 3. Haz que tu usuario revoque el privilegio a `u2`. ¿Qué ocurre?
```SQL
REVOKE SELECT ON artigo FROM u2;
*
ERROR en línea 1:
ORA-01927: no se puede revocar (REVOKE) privilegios que no se han otorgado
```

Este error ocurre porque se intenta revocar un privilegio que no fue otorgado directamente por el usuario. En Oracle, solo quien otorga el privilegio puede revocarlo, a menos que se revoque en cascada quitando el privilegio al intermediario (`u1`), lo que anula todos los permisos derivados de él (`u2`).

#### 4. Haz que tu usuario revoque el privilegio a `u1`. ¿Qué ocurre?
```SQL
REVOKE SELECT ON artigo FROM u1;
Revocación terminada correctamente.
```

Al ejecutar la sentencia, se revocó el privilegio de selección concedido a `u1`. Esta revocación provoca una **revocación en cascada**: Oracle también recova automáticamente el permiso de `SELECT` que `u1` había concedido a `u2`.
Como resultado, ni `u1` ni `u2` pueden seguir consultando la tabla `artigo`.

## 8. Optimización
#### 2. Obtén el plan de ejecución para la consulta que muestra todos los datos de todos los empleados. Utiliza tanto `explain plan` como `set autotrace`. Revisa las diferencias entre ambas aproximaciones.
```SQL
-- Usando explain plan:
EXPLAIN PLAN FOR
	SELECT * FROM emp; -- la consulta no se ejecuta, solo se genera el plan de ejecución

-- Podemos consultar el plan así:
SELECT * 
FROM TABLE(DBMS_XPLAN.DISPLAY);

PLAN_TABLE_OUTPUT
-------------------------------------------
Plan hash value: 3956160932

--------------------------------------------------------------------------
| Id  | Operation         | Name | Rows  | Bytes | Cost (%CPU)| Time     |
--------------------------------------------------------------------------
|   0 | SELECT STATEMENT  |      |    14 |   532 |     3   (0)| 00:00:01 |
|   1 |  TABLE ACCESS FULL| EMP  |    14 |   532 |     3   (0)| 00:00:01 |
--------------------------------------------------------------------------

8 filas seleccionadas.
```

Al ejecutar `EXPLAIN PLAN FOR SELECT * FROM emp;`, la consulta no se ejecuta realmente, sino que Oracle genera y guarda un plan de ejecución estimado en la tabla especial `PLAN_TABLE`. Este plan indica cómo Oracle **planea** acceder a los datos si se llegara a ejecutar la consulta.

🔍 **¿Qué vemos en el plan de ejecución?**
- Id 0: representa la consulta en sí (`SELECT` en este caso).
- Id 1: es el paso donde Oracle accede directamente a la tabla `emp`.
- La operación `TABLE ACCESS FULL` indica que Oracle **lee toda la tabla `EMP` sin usar índices**.
- `Rows = 14` es una estimación del número de filas.
- `Cost = 3` es un valor interno que Oracle usa para comparar la eficiencia (menor es mejor).
- El `%CPU` y el `Time` son también estimaciones para comparar planes.

```SQL
-- Usando set autotrace:
SET AUTOTRACE TRACEONLY EXPLAIN; -- Muestra solo el plan, sin resultados ni estadísitcas

SELECT * FROM emp;
Plan de Ejecución
----------------------------------------------------------
Plan hash value: 3956160932

--------------------------------------------------------------------------
| Id  | Operation         | Name | Rows  | Bytes | Cost (%CPU)| Time     |
--------------------------------------------------------------------------
|   0 | SELECT STATEMENT  |      |    14 |   532 |     3   (0)| 00:00:01 |
|   1 |  TABLE ACCESS FULL| EMP  |    14 |   532 |     3   (0)| 00:00:01 |
--------------------------------------------------------------------------
```

Al ejecutar `SET AUTOTRACE TRACEONLY EXPLAIN`, se le indica a Oracle que, al lanzar una consulta, no muestre resultados ni estadísticas reales de ejecución, sino solo el plan de ejecución. A diferencia de con `EXPLAIN PLAN`, no se requiere consultar `DBMS_XPLAN`, ya que muestra el plan directamente.

> **NOTA:** cada consulta que se lance será ejecutada (esto es especialmente importante si la consulta modifica datos).

#### 3. Muestra el plan de ejecución para la consulta que selecciona todos los datos del empleado 7902.
```SQL
-- Como ya tenemos AUTOTRACE activado, hacemos la consulta directamente
SELECT *
FROM emp
WHERE empno = 7902;

Plan de Ejecución
----------------------------------------------------------
Plan hash value: 2949544139

---------------------------------------------------------------------------
| Id  | Operation                   | Name   | Rows  | Bytes | Cost (%CPU)| Time     |
---------------------------------------------------------------------------
|   0 | SELECT STATEMENT            |        |     1 |    38 |     1   (0)| 00:00:01 |
|   1 |  TABLE ACCESS BY INDEX ROWID| EMP    |     1 |    38 |     1   (0)| 00:00:01 |
|*  2 |   INDEX UNIQUE SCAN         | PK_EMP |     1 |       |     0   (0)| 00:00:01 |
---------------------------------------------------------------------------
Predicate Information (identified by operation id):
---------------------------------------------------
   2 - access("EMPNO"=7902)
```

🔍 **¿Qué hace este plan de ejecución?**
- **Operación 2:** `INDEX UNIQUE SCAN` indica que Oracle accede directamente al índice `PK_EMP` (el de la clave primaria), para localizar el valor `7902` en la columna `empno`.
- **Operación 1:** Una vez localizado el índice, se usa `TABLE ACCESS BY INDEX ROWID` para acceder al bloque de datos real de la tabla `emp`.
- Se estima `Rows = 1` ya que `empno` es una clave única (no hay múltiples filas con su valor, solo una).
- El plan tiene un coste muy bajo (`Cost = 1`), ya que el acceso es directo y muy eficiente al usar índices.

#### 4. Haz lo mismo, pero fuerza a que se utilice una exploración completa de la tabla `emp`. Revisa las diferencias con el plan del ejercicio anterior. Fíjate especialmente en la información sobre los predicados.
> **NOTA:** Un predicado es cualquier condición que filtra filas en una consulta. En un plan de ejecución, Oracle indica cómo y cuándo aplica ese filtro en `PREDICATE INFORMATION`.

```SQL
-- Usamos un hint sin alias para forzar la exploración completa de la tabla:
SELECT /*+ FULL(emp) */ * FROM emp WHERE empno = 7902;

Plan de Ejecución
----------------------------------------------------------
Plan hash value: 3956160932

--------------------------------------------------------------------------
| Id  | Operation         | Name | Rows  | Bytes | Cost (%CPU)| Time     |
--------------------------------------------------------------------------
|   0 | SELECT STATEMENT  |      |     1 |    38 |     3   (0)| 00:00:01 |
|*  1 |  TABLE ACCESS FULL| EMP  |     1 |    38 |     3   (0)| 00:00:01 |
--------------------------------------------------------------------------

Predicate Information (identified by operation id):
---------------------------------------------------

   1 - filter("EMPNO"=7902)

-- Para usar el hint usando alias:
SELECT /*+ FULL(e) */ FROM emp e WHERE empno = 7902;
```

🔍 **¿Qué hace este plan de ejecución?**
En esta consulta hemos usado el *hint* `/*+ FULL(emp) */` para forzar a Oracle a ignorar el índice de la clave primaria (`PK_EMP`) y realizar en su lugar una exploración completa de la tabla.
Como resultado, Oracle accede a todos los registros de la tabla y aplica el filtrado `WHERE empno = 7902` en memoria, fila a fila.
- La operación principal es `TABLE ACCESS FULL`, lo que significa que Oracle lee todas las filas de la tabla.
- Aunque solo se espera encontrar 1 fila, el plan indica que Oracle debe recorrer las 14 filas de la tabla y filtrar por condición.

**Diferencias con el plan anterior:**
- En el primer plan se accede mediante índices (`INDEX UNIQUE SCAN`), mientras que en este plan se escanean todas las filas (`TABLE ACCESS FULL`).
- El primer plan es más eficiente, ya que es un acceso directo; mientras que en este plan la eficiencia es menor ya que se realiza una lectura total.
- **Tipo de predicado:** en el primer plan el predicado es `access("EMPNO"=7902)`, es decir, usa el índice para encontrar directamente las filas que cumplen la condición, sin mirar las demás filas. En el segundo plan el predicado es `filter("EMPNO"=7902)`, es decir, Oracle lee primero las filas, y luego aplica el filtro evaluando la condición fila por fila.
- El coste estimado del primer plan es 1, mientras que el coste del segundo es 3.

> **PREDICADOS:** `access` indica condiciones usadas para **acceder** a los datos (usualmente mediante índices), mientras que `filter` son condiciones evaluadas **después de obtener los datos** (usualmente en escaneos completos de tabla).

> **HINTS CON/SIN ALIAS:** si usas un alias en la consulta (`FROM emp e`) se debe usar el mismo alias en los hints, o el hint será ignorado por el optimizador.

#### 5. Aunque la consulta parezca absurda, busca los códigos de los empleados 7902, 7839 y 5432 (obtén solo el campo `empno`, para verificar cuáles de ellos existen). Muestra el plan de ejecución.
```SQL
SELECT empno FROM emp WHERE empno IN (7902, 7839, 5432);
Plan de Ejecución
----------------------------------------------------------
Plan hash value: 908504414

-----------------------------------------------------------------------------
| Id  | Operation          | Name   | Rows  | Bytes | Cost (%CPU)| Time     |
-----------------------------------------------------------------------------
|   0 | SELECT STATEMENT   |        |     2 |     8 |     1   (0)| 00:00:01 |
|   1 |  INLIST ITERATOR   |        |       |       |            |          |
|*  2 |   INDEX UNIQUE SCAN| PK_EMP |     2 |     8 |     1   (0)| 00:00:01 |
-----------------------------------------------------------------------------

Predicate Information (identified by operation id):
---------------------------------------------------

   2 - access("EMPNO"=5432 OR "EMPNO"=7839 OR "EMPNO"=7902)
```

- `INDEX UNIQUE SCAN` sobre `PK_EMP`: Oracle accede al índice `PK_EMP` porque `EMPNO` es la clave primaria.
- `INLIST ITERATOR`: esta operación permite a Oracle reutilizar el acceso por índice para varios valores dentro del `IN`, sin repetir todo el plan para cada valor.
- `access("EMPNO"=5432 OR "EMPNO"=7839 OR "EMPNO"=7902)` muestra que se está buscando con una condición `OR`.
- De los tres valores, solo dos existen en la tabla, por eso la estimación de filas es `Rows = 2`.

Oracle optimiza la búsqueda múltiple en una columna indexada usando `INLIST ITERATOR`, lo que transforma esta condición en varias búsquedas por índice optimizadas por este operador. Esto permite resolver la consulta en un solo paso, accediendo directamente el índice `PK_EMP`.

#### 6. ¿Y qué pasa si queremos obtener todos los códigos de empleados?
```SQL
SELECT empno FROM emp;
Plan de Ejecución
----------------------------------------------------------
Plan hash value: 179099197

---------------------------------------------------------------------------
| Id  | Operation        | Name   | Rows  | Bytes | Cost (%CPU)| Time     |
---------------------------------------------------------------------------
|   0 | SELECT STATEMENT |        |    14 |    56 |     1   (0)| 00:00:01 |
|   1 |  INDEX FULL SCAN | PK_EMP |    14 |    56 |     1   (0)| 00:00:01 |
---------------------------------------------------------------------------
```

Como Oracle solo necesita la columna `empno` (que ya está en el índice `PK_EMP`), detecta que no es necesario leer todos los datos de la tabla (lo cual es menos eficiente) y optimiza el acceso usando únicamente el índice. `INDEX FULL SCAN` escanea completamente el índice para leer todos los valores de la columna `empno`.

#### 7. Busca ahora el código y salario del empleado "SMITH". Muestra el plan.
```SQL
SELECT empno, sal FROM emp WHERE ename = 'SMITH';

Plan de Ejecución
----------------------------------------------------------
Plan hash value: 3956160932

--------------------------------------------------------------------------
| Id  | Operation         | Name | Rows  | Bytes | Cost (%CPU)| Time     |
--------------------------------------------------------------------------
|   0 | SELECT STATEMENT  |      |     1 |    14 |     3   (0)| 00:00:01 |
|*  1 |  TABLE ACCESS FULL| EMP  |     1 |    14 |     3   (0)| 00:00:01 |
--------------------------------------------------------------------------

Predicate Information (identified by operation id):
---------------------------------------------------

   1 - filter("ENAME"='SMITH')
```

- Oracle ejecuta una búsqueda completa de la tabla, ya que no hay un índice disponible para optimizar el acceso. Es decir, Oracle revisa fila por fila hasta encontrar aquella(s) (Oracle no sabe que solo hay una, puede haber varias) que cumplan la condición `ENAME = 'SMITH'`.
- El coste es 3, es bajo porque la tabla es pequeña, pero si creciera, esta estrategia sería ineficiente.



