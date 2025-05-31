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
	data DATE NOT NULL,
	CONSTRAINT pk_venda PRIMARY KEY (idven),
	CONSTRAINT fk_venda_codart FOREIGN KEY (codart) REFERENCES artigo(codart)
);
```

