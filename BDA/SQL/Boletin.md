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

