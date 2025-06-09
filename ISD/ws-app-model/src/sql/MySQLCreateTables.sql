-- ----------------------------------------------------------------------------
-- Model
-------------------------------------------------------------------------------
DROP TABLE Inscription;
DROP TABLE Course;


-- ----------------------------------------------------------------------------
-- Cursos
-- Un id para el curso, su nombre y la ciudad donde se celebra, la fecha y
-- hora de comienzo, el precio, el maximo numero de plazas, la fecha y hora
-- en la que se ha dado de alta el curso, y el numero deplazas reservadas
-------------------------------------------------------------------------------

CREATE TABLE Course (
                        courseId BIGINT NOT NULL AUTO_INCREMENT,
                        name VARCHAR(255) COLLATE latin1_bin NOT NULL,
                        city VARCHAR(255) COLLATE latin1_bin NOT NULL,
                        startDate DATETIME NOT NULL,
                        price FLOAT NOT NULL,
                        maxPlaces SMALLINT NOT NULL,
                        freePlaces SMALLINT NOT NULL,
                        creationDate DATETIME NOT NULL,
                        CONSTRAINT CoursePK PRIMARY KEY(courseId),
                        CONSTRAINT validMaxPlaces CHECK ( maxPlaces >= 0),
                        CONSTRAINT validfreePlaces CHECK ( freePlaces >= 0)
) ENGINE = InnoDB;


-- ----------------------------------------------------------------------------
-- Inscripciones
-- Un id para la inscripcion, el id del curso al que se inscribe, el email
-- del usuario que se inscribe al curso, su numero de tarjeta, la fecha y
-- hora en la que se inscribio, y la fecha y hora en la que cancela la
-- inscripcion en caso de hacerlo
-------------------------------------------------------------------------------

CREATE TABLE Inscription (
                             inscriptionId BIGINT NOT NULL AUTO_INCREMENT,
                             courseId BIGINT NOT NULL,
                             userEmail VARCHAR(100) NOT NULL,
                             creditCardNumber VARCHAR(16) NOT NULL,
                             inscriptionDate DATETIME NOT NULL,
                             cancelDate DATETIME,
                             CONSTRAINT InscriptionPK PRIMARY KEY(inscriptionId),
                             CONSTRAINT InscriptionCourseFK FOREIGN KEY(courseId)
                                 REFERENCES Course(courseId) ON DELETE CASCADE
) ENGINE = InnoDB;