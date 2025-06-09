package es.udc.ws.app.test.model.appservice;

import es.udc.ws.app.model.course.Course;
import es.udc.ws.app.model.course.SqlCourseDao;
import es.udc.ws.app.model.course.SqlCourseDaoFactory;
import es.udc.ws.app.model.courseservice.CourseService;
import es.udc.ws.app.model.courseservice.CourseServiceFactory;
import es.udc.ws.app.model.courseservice.exceptions.*;
import es.udc.ws.app.model.inscription.Inscription;
import es.udc.ws.app.model.inscription.SqlInscriptionDao;
import es.udc.ws.app.model.inscription.SqlInscriptionDaoFactory;
import es.udc.ws.util.exceptions.InputValidationException;
import es.udc.ws.util.exceptions.InstanceNotFoundException;
import es.udc.ws.util.sql.DataSourceLocator;
import es.udc.ws.util.sql.SimpleDataSource;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import javax.sql.DataSource;

import java.sql.Connection;
import java.sql.SQLException;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

import static es.udc.ws.app.model.util.ModelConstants.*;
import static org.junit.jupiter.api.Assertions.*;

public class AppServiceTest {

    private final long NON_EXISTENT_COURSE_ID = -1;
    private final String VALID_CREDIT_CARD_NUMBER = "1234567890123456";
    private final String INVALID_CREDIT_CARD_NUMBER = "";
    private final String VALID_EMAIL = "hola@gmail.com";
    private final String INVALID_EMAIL = "";
    private static CourseService courseService = null;
    private static SqlCourseDao courseDao = null;
    private static SqlInscriptionDao inscriptionDao = null;


    @BeforeAll
    public static void init() {
        DataSource dataSource = new SimpleDataSource();
        DataSourceLocator.addDataSource(APP_DATA_SOURCE, dataSource);

        courseService = CourseServiceFactory.getService();
        courseDao = SqlCourseDaoFactory.getDao();
        inscriptionDao = SqlInscriptionDaoFactory.getDao();
    }

    private Course getValidCourse(String name) {
        return new Course(name,"Madrid", LocalDateTime.now().plusDays(17),19.95f, (short) 100);
    }

    private Course getValidCourse() {
        return getValidCourse("Fotografia");
    }

    private Course createCourse(Course course) {
        Course addedCourse = null;

        try {
            addedCourse = courseService.addCourse(course);
        } catch (InputValidationException e) {
            throw new RuntimeException(e);
        }
        return addedCourse;
    }

    private void removeCourse(Long courseId) {
        DataSource dataSource = DataSourceLocator.getDataSource(APP_DATA_SOURCE);

        try (Connection connection = dataSource.getConnection()) {
            try {
                connection.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
                connection.setAutoCommit(false);

                courseDao.remove(connection, courseId);
                connection.commit();


            } catch (InstanceNotFoundException e) {
                connection.commit();
                throw new RuntimeException(e);
            } catch(SQLException e) {
                connection.rollback();
                throw new RuntimeException(e);
            } catch (RuntimeException | Error e) {
                connection.rollback();
                throw e;
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    private void updateCourse(Course course) {
        DataSource dataSource = DataSourceLocator.getDataSource(APP_DATA_SOURCE);
        try (Connection connection = dataSource.getConnection()) {
            try {
                connection.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
                connection.setAutoCommit(false);

                courseDao.update(connection, course);
                connection.commit();
            } catch (InstanceNotFoundException e) {
                connection.commit();
                throw new RuntimeException(e);
            } catch (SQLException e) {
                connection.rollback();
                throw new RuntimeException(e);
            } catch (RuntimeException | Error e) {
                connection.rollback();
                throw e;
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    private void updateInscription(Inscription inscription) {
        DataSource dataSource = DataSourceLocator.getDataSource(APP_DATA_SOURCE);
        try (Connection connection = dataSource.getConnection()) {
            try {
                connection.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
                connection.setAutoCommit(false);

                inscriptionDao.update(connection, inscription);
                connection.commit();
            } catch (InstanceNotFoundException e) {
                connection.commit();
                throw new RuntimeException(e);
            } catch (SQLException e) {
                connection.rollback();
                throw new RuntimeException(e);
            } catch (RuntimeException | Error e) {
                connection.rollback();
                throw e;
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    private void removeInscription(Long inscriptionId) {
        DataSource dataSource = DataSourceLocator.getDataSource(APP_DATA_SOURCE);

        try (Connection connection = dataSource.getConnection()) {
            try {
                connection.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
                connection.setAutoCommit(false);

                inscriptionDao.remove(connection, inscriptionId);

                connection.commit();


            } catch (InstanceNotFoundException e) {
                connection.commit();
                throw new RuntimeException(e);
            } catch(SQLException e) {
                connection.rollback();
                throw new RuntimeException(e);
            } catch (RuntimeException | Error e) {
                connection.rollback();
                throw e;
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    @Test
    public void testAddCourseAndFindCourse() throws InputValidationException, InstanceNotFoundException {

        //Creamos un curso válido
        Course course = getValidCourse();
        Course addedCourse = null;

        try {
            // almacenamos la fecha justo antes de haberse creado el curso
            LocalDateTime beforeCreationDate = LocalDateTime.now().withNano(0);
            // creamos el curso
            addedCourse = courseService.addCourse(course);
            // almacenamos la fecha justo despues de haberse creado el curso
            LocalDateTime afterCreationDate = LocalDateTime.now().withNano(0);

            // curso que se ha encontrado
            Course foundCourse = courseService.findCourse(addedCourse.getCourseId());

            // realizamos las comparaciones entre el evento añadido y el que se ha encontrado
            assertEquals(addedCourse, foundCourse);
            assertEquals(foundCourse.getName(), addedCourse.getName());
            assertEquals(foundCourse.getCity(), addedCourse.getCity());
            assertEquals(foundCourse.getPrice(), addedCourse.getPrice());
            assertEquals(foundCourse.getMaxPlaces(), addedCourse.getMaxPlaces());
            assertEquals(foundCourse.getfreePlaces(), addedCourse.getfreePlaces());
            assertEquals(foundCourse.getStartDate(), addedCourse.getStartDate());
            // La fecha de creacion del curso debe estar entre la fecha de justo antes de haberse creado el
            // curso y la fecha de justo despues de haberse creado el curso
            assertTrue((foundCourse.getCreationDate().compareTo(beforeCreationDate) >= 0) &&
                    (foundCourse.getCreationDate().compareTo(afterCreationDate) <= 0));
        } finally {
            if(addedCourse!=null){
                // eliminamos el curso creado para la prueba
                removeCourse(addedCourse.getCourseId());
            }
        }
    }
  
    // Probamos los errores al añadir un curso erroneamente
    @Test
    public void testAddInvalidCourse() {
      // Verificar que no se pueda añadir un curso si el nombre es null
      assertThrows(InputValidationException.class, () -> {
          Course course = getValidCourse();
          course.setName(null);
          Course addedCourse = courseService.addCourse(course);
      });

      // Verificar que no se pueda añadir un curso si el nombre está vacío
      assertThrows(InputValidationException.class, () -> {
          Course course = getValidCourse();
          course.setName("");
          Course addedCourse = courseService.addCourse(course);
      });

      //  Verificar que no se pueda añadir un curso si la ciudad es null
      assertThrows(InputValidationException.class, () -> {
          Course course = getValidCourse();
          course.setCity(null);
          Course addedCourse = courseService.addCourse(course);
      });

      // Verificar que no se pueda añadir un curso si la ciudad está vacía
      assertThrows(InputValidationException.class, () -> {
          Course course = getValidCourse();
          course.setCity("");
          Course addedCourse = courseService.addCourse(course);
      });

      // Verificar que el precio del curso no puede ser negativo
      assertThrows(InputValidationException.class, () -> {
          Course course = getValidCourse();
          course.setPrice((short) -1);
          Course addedCourse = courseService.addCourse(course);
      });

      // Verificar que el precio no exceda el valor máximo permitido
      assertThrows(InputValidationException.class, () -> {
          Course course = getValidCourse();
          course.setPrice((short) (MAX_PRICE + 1));
          Course addedCourse = courseService.addCourse(course);
      });

      // Verificar que los lugares máximos no sean negativos
      assertThrows(InputValidationException.class, () -> {
          Course course = getValidCourse();
          course.setMaxPlaces((short) -1);
          Course addedCourse = courseService.addCourse(course);
      });

      //Verificar que los lugares máximos no excedan el valor máximo permitido
      assertThrows(InputValidationException.class, () -> {
          Course course = getValidCourse();
          course.setMaxPlaces((short) (MAX_PLACES + 1));
          Course addedCourse = courseService.addCourse(course);
      });

      // Verificar que los lugares reservados no sean negativos
      assertThrows(InputValidationException.class, () -> {
          Course course = getValidCourse();
          course.setfreePlaces((short) -1);
          Course addedCourse = courseService.addCourse(course);
      });

      // Verificar que los lugares reservados no excedan el valor máximo permitido
      assertThrows(InputValidationException.class, () -> {
          Course course = getValidCourse();
          course.setfreePlaces((short) (MAX_PLACES + 1));
          Course addedCourse = courseService.addCourse(course);
      });

      // Verificar que la fecha de inicio del curso sea al menos 15 días después de la fecha de creación
      assertThrows(InputValidationException.class, () -> {
          Course course = getValidCourse();
          course.setCreationDate(LocalDateTime.now().withNano(0));
          course.setStartDate(course.getCreationDate().plusDays(10).withNano(0));
          courseService.addCourse(course);
      });

    }

    @Test
    public void testFindNonExistentCourse() {
    //Se lanza excepcion si se busca un curso inexistente
    assertThrows(InstanceNotFoundException.class, () -> courseService.findCourse(NON_EXISTENT_COURSE_ID));
    }

    @Test
    public void findCoursesTest() throws InstanceNotFoundException, InputValidationException {
      List<Course> listCourse;
      Course course1 = null;
      Course course2 = null;
      Course course3 = null;
      Course course4 = null;

      try{
          // Añadimos cursos
          course1 = new Course("Cocina", "Madrid", LocalDateTime.now().plusDays(22),
                  19.95F, (short) 80); // curso exitoso
          course2 = new Course("Informatica", "Barcelona", LocalDateTime.now().plusDays(22),
                  18.95F, (short) 90); // curso que no pertenece a la ciudad buscada
          course3 = new Course("Remo", "Madrid", LocalDateTime.now().plusDays(20),
                  29.95F, (short) 40); // curso exitoso
          course4 = new Course("Fotografia", "Madrid", LocalDateTime.now().plusDays(16),
                  15.95F, (short) 20); // curso que no pertenece a la fecha buscada

          course1 = courseService.addCourse(course1);
          course2 = courseService.addCourse(course2);
          course3 = courseService.addCourse(course3);
          course4 = courseService.addCourse(course4);

          //Buscamos cursos en Madrid con fecha posterior a 19 dias de ahora
          listCourse = courseService.findCourses("Madrid", LocalDateTime.now().plusDays(19));
          assertEquals(2, listCourse.size());
          assertEquals(course3.getName(), listCourse.get(0).getName());
          assertEquals(course1.getName(), listCourse.get(1).getName());

      } finally {
          //Borramos los cursos creados de la BD
          if (course1 != null) {
              removeCourse(course1.getCourseId());
          }
          if (course2 != null) {
              removeCourse(course2.getCourseId());
          }
          if (course3 != null) {
              removeCourse(course3.getCourseId());
          }
          if (course4 != null) {
              removeCourse(course4.getCourseId());
          }
      }

  }

  @Test
  public void testJoinCourseAndFindInscription() throws InputValidationException, NotEnoughPlacesException, InstanceNotFoundException, CourseAlreadyStartedException, OutOfDateException, AlreadyCancelledException, WrongUserException, CourseAlreadyJoinedException {
      List<Inscription> foundInscriptions;

      //Creamos un curso válido
      Course course = getValidCourse();
      Inscription inscription = null;
      Course addedCourse = null;

      try {

          //Añadimos el curso a la BD
          addedCourse = courseService.addCourse(course);
          Long id = addedCourse.getCourseId();
          LocalDateTime beforeJoin = LocalDateTime.now().withNano(0);

          assertThrows(InputValidationException.class, () -> courseService.joinCourse(INVALID_EMAIL, id, VALID_CREDIT_CARD_NUMBER));
          inscription = courseService.joinCourse(VALID_EMAIL, addedCourse.getCourseId(), VALID_CREDIT_CARD_NUMBER);

          LocalDateTime afterJoin = LocalDateTime.now().withNano(0);

          // Encontramos la  inscripcion
          foundInscriptions = courseService.findInscriptions(VALID_EMAIL);
          assertFalse(foundInscriptions.isEmpty());

          // Comprobar si la respuesta encontrada es la esperada
          Inscription foundInscription = foundInscriptions.get(0);

          assertEquals(inscription, foundInscription);
          assertEquals(VALID_CREDIT_CARD_NUMBER, foundInscription.getCreditCardNumber());
          assertEquals(VALID_EMAIL, foundInscription.getUserEmail());
          assertEquals(inscription.getCourseId(), foundInscription.getCourseId());
          assertTrue((foundInscription.getInscriptionDate().compareTo(beforeJoin) >= 0)
                  && (foundInscription.getInscriptionDate().compareTo(afterJoin) <= 0));

      } finally {
          // limpiar inscripcion y el curso de la base de datos
          if(inscription != null) {
              removeInscription(inscription.getInscriptionId());
          }
          if (addedCourse !=null)
              removeCourse(addedCourse.getCourseId());
      }
  }

  @Test
  public void testJoinCourseWithInvalidCreditCard() {
      Course course = createCourse(getValidCourse());
      try {
          //Intentamos inscribir a un usuario con una tarjeta de crédito invalida
          assertThrows(InputValidationException.class, () -> {
              Inscription inscription = courseService.joinCourse(VALID_EMAIL, course.getCourseId(), INVALID_CREDIT_CARD_NUMBER);
              removeInscription(inscription.getInscriptionId());
          });
      } finally {
          //Eliminamos el curso de la BD
          removeCourse(course.getCourseId());
      }
  }

  @Test
  public void testJoinNonExistentCourse() {
      // Definir un curso que no existe
      Course course = new Course(NON_EXISTENT_COURSE_ID, "Curso no existente", "Atlantis", LocalDateTime.now().plusDays(20), 25.99f, (short) 40);
      // Asegurarse de que lanza una InstanceNotFoundException
      assertThrows(InstanceNotFoundException.class, () -> {
          courseService.joinCourse(VALID_EMAIL, course.getCourseId(), VALID_CREDIT_CARD_NUMBER);
      });
  }

  @Test
  public void testJoinCourseWithInvalidEmail() {
      Course course = createCourse(getValidCourse());
      try {
          assertThrows(InputValidationException.class, () -> {
              Inscription inscription = courseService.joinCourse(INVALID_EMAIL, course.getCourseId(), VALID_CREDIT_CARD_NUMBER);
              removeInscription(inscription.getInscriptionId());
          });
      } finally {
          removeCourse(course.getCourseId());
      }
  }

  @Test
  public void findInscriptionsTest() throws InputValidationException, NotEnoughPlacesException, InstanceNotFoundException, CourseAlreadyStartedException, CourseAlreadyJoinedException {
      Course course1 = getValidCourse("Cocina");
      Course course2 = getValidCourse("Fotografia");
      Course course3 = getValidCourse("Pintura");

      Course createdCourse1 = courseService.addCourse(course1);
      Course createdCourse2 = courseService.addCourse(course2);
      Course createdCourse3 = courseService.addCourse(course3);

      //Inscribir al usuario con VALID_EMAIL en los tress cursos
      Inscription inscription1 = courseService.joinCourse(VALID_EMAIL, createdCourse1.getCourseId(), VALID_CREDIT_CARD_NUMBER);
      Inscription inscription2 = courseService.joinCourse(VALID_EMAIL, createdCourse2.getCourseId(), VALID_CREDIT_CARD_NUMBER);
      Inscription inscription3 = courseService.joinCourse(VALID_EMAIL, createdCourse3.getCourseId(), VALID_CREDIT_CARD_NUMBER);

      // Inscribir a un segundo usuario en algun curso
      Inscription inscription4 = courseService.joinCourse("str@gmail.com", createdCourse1.getCourseId(), VALID_CREDIT_CARD_NUMBER);
      Inscription inscription5 = courseService.joinCourse("str@gmail.com", createdCourse3.getCourseId(), VALID_CREDIT_CARD_NUMBER);

      try {

          // Buscar inscripciones del primer usuario y comprobar que coinciden con las inscripciones realizadas
          List<Inscription> userInscriptions = courseService.findInscriptions(VALID_EMAIL);
          List<Inscription> userExpectedInscriptions = new ArrayList<>();

          userExpectedInscriptions.add(inscription1);
          userExpectedInscriptions.add(inscription2);
          userExpectedInscriptions.add(inscription3);

          assertEquals(userInscriptions, userExpectedInscriptions);

          // Buscar inscripciones del segundo usuario y comprobar los resultados esperados
          List<Inscription> userInscriptions2 = courseService.findInscriptions("str@gmail.com");
          List<Inscription> userExpectedInscriptions2 = new ArrayList<>();

          userExpectedInscriptions2.add(inscription4);
          userExpectedInscriptions2.add(inscription5);

          assertEquals(userInscriptions2, userExpectedInscriptions2);
      } finally {

          //Borramos los cursos e inscripciones creadas
          if(inscription1 != null) {
              removeInscription(inscription1.getInscriptionId());
          }
          if(inscription2 != null) {
              removeInscription(inscription2.getInscriptionId());
          }
          if(inscription3 != null) {
              removeInscription(inscription3.getInscriptionId());
          }
          if(inscription4 != null) {
              removeInscription(inscription4.getInscriptionId());
          }
          if(inscription5 != null) {
              removeInscription(inscription5.getInscriptionId());
          }

          removeCourse(createdCourse1.getCourseId());
          removeCourse(createdCourse2.getCourseId());
          removeCourse(createdCourse3.getCourseId());
      }
  }
  @Test
  public void testInvalidSearch() throws InputValidationException {
    //Definimos un ID inválido para el curso
    Long invalidId = -1L;
    Course course = getValidCourse();
    Course addedCourse = null;

    try{

        //Añadimos curso e intentamos busacar con un ID inválido
        addedCourse = courseService.addCourse(course);
        assertThrows(InstanceNotFoundException.class, () -> courseService.findCourse(invalidId));
    } finally {

        //Eliminamos el curso de la BD
        if(addedCourse != null) {
            removeCourse(addedCourse.getCourseId());
        }
    }
}

  @Test
  public void testJoinFullCourse() throws InstanceNotFoundException, InputValidationException {
      //Crea un curso válido
      Course course = createCourse(getValidCourse());
      course.setfreePlaces((short) 0); // Se configura el curso como lleno
      updateCourse(course);

      // Verifica que el valor se haya actualizado en la BD
      Course updatedCourse = courseService.findCourse(course.getCourseId());
      assertEquals(course.getMaxPlaces(), 100); // El curso debería estar lleno
      assertEquals(course.getfreePlaces(),(short) 0);

      try {
          //caso en el que se intenta unir al curso lleno
          assertThrows(NotEnoughPlacesException.class,
                  () -> courseService.joinCourse(VALID_EMAIL, updatedCourse.getCourseId(), VALID_CREDIT_CARD_NUMBER));
      } finally {
          //Elimina el curso de la BD
          removeCourse(course.getCourseId());
      }
  }

  // Cancelar una inscripcion y comprobar que numPlazasReservadas es correcto en course
  @Test
  public void testCancelInscription() throws InputValidationException, NotEnoughPlacesException, InstanceNotFoundException, CourseAlreadyStartedException, OutOfDateException, AlreadyCancelledException, WrongUserException, CourseAlreadyJoinedException {
      //Crea un curso válido
      Course course = getValidCourse();
      Course addedCourse = null;
      Inscription inscription = null;
      try {
          addedCourse = courseService.addCourse(course);
          inscription = courseService.joinCourse(VALID_EMAIL,addedCourse.getCourseId(),VALID_CREDIT_CARD_NUMBER);

          // Comprobar que el número de plazas disponibles es 99 tras la inscripción

          Course course2 = courseService.findCourse(addedCourse.getCourseId());
          assertEquals(99, course2.getfreePlaces());

          // Registra el tiempo actual antes de cancelar la inscripción
          LocalDateTime beforeJoin = LocalDateTime.now().withNano(0);
          courseService.cancelInscription(inscription.getInscriptionId(),VALID_EMAIL);
          LocalDateTime afterJoin = LocalDateTime.now().withNano(0);

          // Comprobar que el número de plazas reservadas vuelve a 0 tras la cancelación
          course2 = courseService.findCourse(addedCourse.getCourseId());
          assertEquals(100, course2.getfreePlaces());

          // Verificar que la inscripción cancelada esté en la lista de inscripciones y la fecha de cancelación sea correcta
          List<Inscription> cancelInscription = courseService.findInscriptions(VALID_EMAIL);
          assertEquals(cancelInscription.get(0).getInscriptionId(),inscription.getInscriptionId());
          assertTrue((cancelInscription.get(0).getCancelDate().compareTo(beforeJoin) >= 0)
                  && (cancelInscription.get(0).getCancelDate().compareTo(afterJoin) <= 0));

      } finally {

          //Borra la inscripción y el curso de la base de datos
          if (inscription != null)
              removeInscription(inscription.getInscriptionId());
          if (addedCourse != null)
              removeCourse(addedCourse.getCourseId());
      }

  }

  // Unirse a un curso ya empezado
  @Test
  public void testCourseAlreadyStartedException() throws InputValidationException, NotEnoughPlacesException, InstanceNotFoundException, CourseAlreadyStartedException, CourseAlreadyJoinedException {
      Course addedCourse = null;
      try {
          //Crea un curso cuya fecha ya ha pasado
          addedCourse = createCourse(getValidCourse());
          addedCourse.setStartDate(LocalDateTime.now().minusDays(2L).withNano(0));
          addedCourse.setCreationDate(LocalDateTime.now().minusDays(22L).withNano(0));
          updateCourse(addedCourse);

          //Intenta unirse al curso
          Long id = addedCourse.getCourseId();
          assertThrows(CourseAlreadyStartedException.class, () ->
                  courseService.joinCourse(VALID_EMAIL,id,VALID_CREDIT_CARD_NUMBER));
      } finally {
          //Borra el curso de la BD
          if (addedCourse != null)
              removeCourse(addedCourse.getCourseId());
      }

  }


  // Unirse a un curso más de una vez
  @Test
  public void testAlreadyJoined() throws InputValidationException, NotEnoughPlacesException, InstanceNotFoundException, CourseAlreadyStartedException, CourseAlreadyJoinedException {
      Course course = getValidCourse();
      Inscription inscription = null;
      Course addedCourse = null;

      try {
          //Crea y añade el curso a la BD
          addedCourse = courseService.addCourse(course);
          Long id = addedCourse.getCourseId();

          //Se une al grupo
          inscription = courseService.joinCourse(VALID_EMAIL, addedCourse.getCourseId(), VALID_CREDIT_CARD_NUMBER);

          //Se vuelve a intentar meter
          assertThrows(CourseAlreadyJoinedException.class, () ->
                  courseService.joinCourse(VALID_EMAIL, id, VALID_CREDIT_CARD_NUMBER));
      } finally {
          // limpiar inscripcion de la base de datos
          if(inscription != null) {
              removeInscription(inscription.getInscriptionId());
          }
          if (addedCourse !=null)
              removeCourse(addedCourse.getCourseId());
      }

  }

      // Cancelar una inscripcion fuera de plazo
    @Test
    public void OutOfDateException() throws InputValidationException, NotEnoughPlacesException, InstanceNotFoundException, CourseAlreadyStartedException, CourseAlreadyJoinedException {

        Inscription inscription = null;
        Course addedCourse = null;
        try {

            //Creamos un curso que comenzará pronto
            addedCourse = createCourse(getValidCourse());
            addedCourse.setStartDate(LocalDateTime.now().plusDays(2L).withNano(0));
            addedCourse.setCreationDate(LocalDateTime.now().minusDays(22L).withNano(0));
            updateCourse(addedCourse);

            //Unimos al grupo
            inscription = courseService.joinCourse(VALID_EMAIL,addedCourse.getCourseId(),
                    VALID_CREDIT_CARD_NUMBER);

            //Intentamos cancelar fuera de plazo
            Long id = inscription.getInscriptionId();
            assertThrows(OutOfDateException.class, () ->
                    courseService.cancelInscription(id,VALID_EMAIL));
        } finally {

            //Borramos la inscripción y el curso de la BD
            if (inscription != null)
                removeInscription(inscription.getInscriptionId());
            if (addedCourse != null)
                removeCourse(addedCourse.getCourseId());

        }
    }
  // Cancelar una inscripcion de otro usuario
  @Test
  public void testWrongUserException() throws NotEnoughPlacesException, InstanceNotFoundException, CourseAlreadyStartedException, InputValidationException, CourseAlreadyJoinedException {
  
      //Creamos un curso válido y lo añadimos a la BD
      Course course = getValidCourse();
      Course addedCourse = courseService.addCourse(course);
  
      //Hacemos una inscripción con un usuario válido
      Inscription inscription = courseService.joinCourse(VALID_EMAIL, addedCourse.getCourseId(), VALID_CREDIT_CARD_NUMBER);
  
      try {
          //Caso en el que intentamos cancelar la inscripción con un email distinto
          assertThrows(WrongUserException.class, () -> courseService.cancelInscription(inscription.getInscriptionId(), "wrongemail@gmail.com"));
      } finally {
          //Eliminamos inscripción y el curso de la BD
          removeInscription(inscription.getInscriptionId());
          removeCourse(addedCourse.getCourseId());
      }
  }

  // Cancelar un curso que ya está cancelado
  @Test
  public void testAlreadyCancelledException() throws InputValidationException, NotEnoughPlacesException, InstanceNotFoundException, CourseAlreadyStartedException, OutOfDateException, AlreadyCancelledException, WrongUserException, CourseAlreadyJoinedException {

      //Creamos un curso válido y lo agregamos a la BD
      Course course = getValidCourse();
      Course addedCourse = null;
      Inscription inscription = null;
      try {
          addedCourse = courseService.addCourse(course);

          //Hacemos una inscripcion y la cancelamos
          inscription = courseService.joinCourse(VALID_EMAIL,addedCourse.getCourseId(),VALID_CREDIT_CARD_NUMBER);
          Long id = inscription.getInscriptionId();
          courseService.cancelInscription(inscription.getInscriptionId(),VALID_EMAIL);

          //Intentamos cancelar de nuevo la inscripción
          assertThrows(AlreadyCancelledException.class, () ->
                  courseService.cancelInscription(id,VALID_EMAIL));
      } finally {

          //eliminamos la inscripción y el curso de la BD
          if (inscription != null)
              removeInscription(inscription.getInscriptionId());
          if (addedCourse != null)
              removeCourse(addedCourse.getCourseId());
      }

  }

  // Encontrar una inscripcion no existente
  @Test
  public void testFindNonExistentInscription() throws InputValidationException {
      //Buscamos inscripciones para un usuario sin inscripciones
      List<Inscription> list= courseService.findInscriptions(VALID_EMAIL);
      assertEquals(0,list.size());
  }
  
}
