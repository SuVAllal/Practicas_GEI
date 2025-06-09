package es.udc.ws.app.model.courseservice;

import es.udc.ws.app.model.course.Course;
import es.udc.ws.app.model.course.SqlCourseDao;
import es.udc.ws.app.model.course.SqlCourseDaoFactory;
import es.udc.ws.app.model.courseservice.exceptions.*;
import es.udc.ws.app.model.inscription.Inscription;
import es.udc.ws.app.model.inscription.SqlInscriptionDao;
import es.udc.ws.app.model.inscription.SqlInscriptionDaoFactory;
import es.udc.ws.util.exceptions.InputValidationException;
import es.udc.ws.util.exceptions.InstanceNotFoundException;
import es.udc.ws.util.sql.DataSourceLocator;
import es.udc.ws.util.validation.PropertyValidator;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.SQLException;
import java.time.LocalDateTime;
import java.time.temporal.ChronoUnit;
import java.util.List;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static es.udc.ws.app.model.util.ModelConstants.*;

public class CourseServiceImpl implements CourseService {
    private final DataSource dataSource;
    private SqlCourseDao courseDao = null;
    private SqlInscriptionDao inscriptionDao = null;

    public CourseServiceImpl(){
        dataSource = DataSourceLocator.getDataSource(APP_DATA_SOURCE);
        courseDao = SqlCourseDaoFactory.getDao();
        inscriptionDao = SqlInscriptionDaoFactory.getDao();
    }

    //Validar curso
    private void validateCourse(Course course) throws InputValidationException {
        PropertyValidator.validateMandatoryString("name", course.getName());
        PropertyValidator.validateMandatoryString("city", course.getCity());
        PropertyValidator.validateDouble("price", course.getPrice(), 0, MAX_PRICE);
        PropertyValidator.validateLong("maxPlaces", course.getMaxPlaces(), 0, MAX_PLACES);
        PropertyValidator.validateLong("freePlaces", course.getfreePlaces(), 0, MAX_PLACES);
    }

    // Validar email
    private static boolean isValidEmail(String userEmail) {
        /* EL correo debe contener letras o digitos antes del @, debe contener un @,
         despues del @ debe haber digitos o caracteres. Debe terminar con una letra o digito */
        String patron = "^[A-Za-z0-9+_.-]+@[A-Za-z0-9.-]+$";
        Pattern pattern = Pattern.compile(patron);
        Matcher matcher = pattern.matcher(userEmail);
        return matcher.matches();
    }

    /*
     * [FUNC-1] addCourse: dar de alta un curso
     *                 En caso de introducir erroneamente algun campo del curso, se lanzaria una InputValidationException
     *                 En caso de no poder establecer la conexion, se lanzaria una RuntimeException
     */
    @Override
    public Course addCourse(Course course) throws InputValidationException {
        validateCourse(course); // si no es válido ya sale

        // Obtiene la fecha y hora actuales
        LocalDateTime creationDate = LocalDateTime.now().withNano(0);
        course.setCreationDate(creationDate);

        // Si el numero de plazas es incorrecto
        if(course.getMaxPlaces() <= 0) {
            throw new InputValidationException("Invalid " + course.getMaxPlaces() + ". Places of course can't be 0.");
        }

        // Verifica que startDate no esté en el pasado
        if(course.getStartDate().isBefore(LocalDateTime.now())) {
            throw new InputValidationException("Invalid " + course.getStartDate() + ". You can't start a course in a past date.");
        }

        // Verifica que startDate sea al menos 15 días posterior a creationDate
        if(course.getStartDate().isBefore(creationDate.plusDays(15))) {
            throw new InputValidationException("Invalid " + course.getStartDate() + ". The start date must be at least 15 days after the creation date.");
        }

        try (Connection connection = dataSource.getConnection()) {
            try{
                connection.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE); // nivel mas estricto (menos concurrencia)
                connection.setAutoCommit(false);

                Course createCourse = courseDao.create(connection, course);

                connection.commit();
                return createCourse;

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

    /*
     * [FUNC-2] findCourses: buscar cursos que se celebren en una ciudad a partir de una fecha
     *                 En caso de introducir erroneamente algun dato, se lanzaria una InputValidationException
     *                 En caso de no poder establecer la conexion, se lanzaria una RuntimeException
     */
    @Override
    public List<Course> findCourses(String city, LocalDateTime startDate) throws InputValidationException {
        // Validamos que se ha introducido una ciudad correcta
        PropertyValidator.validateMandatoryString("city", city);

        try (Connection connection = dataSource.getConnection()){
            return courseDao.findCourses(connection,city,startDate);
        } catch (SQLException e){
            throw new RuntimeException(e);
        }
    }

    /*
     * [FUNC-3] findCourse: buscar un curso por su identificador
     *                 En caso de no encontrar dicho curso, se lanzaria una InstanceNotFoundException
     *                 En caso de no poder establecer la conexion, se lanzaria una RuntimeException
     */
    @Override
    public Course findCourse(Long courseId) throws InstanceNotFoundException {

        try (Connection connection = dataSource.getConnection()){
            return courseDao.findCourseById(connection, courseId);
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    /*
     * [FUNC-4] joinCourse: inscribirse en un curso
     *                 En caso de no poder encontrar dicho curso, se lanzaria una InstanceNotFoundException
     *                 En caso de introducir erroneamente algun dato, se lanzaria una InputValidationException
     *                 En caso de no poder establecer la conexion, se lanzaria una RuntimeException
     *                 En caso de que el curso ya este lleno (no hay plazas disponibles), se lanzaria una NotEnoughPlacesException
     *                 En caso de que el curso ya haya empezado, se lanzaria una CourseAlreadyStartedException
     *                 En caso de que el usuario ya se haya unido a este curso, se lanzaria una CourseAlreadyJoinedException
     */
    @Override
    public Inscription joinCourse(String userEmail, Long courseId, String creditCardNumber) throws InputValidationException,
            InstanceNotFoundException, NotEnoughPlacesException, CourseAlreadyStartedException, CourseAlreadyJoinedException {
        // Validamos los datos introducidos:
        PropertyValidator.validateCreditCard(creditCardNumber); // tarjeta de credito

        if(!isValidEmail(userEmail)) { // correo electronico
            throw new InputValidationException("User email is not valid.");
        }

        try(Connection connection = dataSource.getConnection()){
            try {
                // preparar conexion
                connection.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
                connection.setAutoCommit(false);

                // puede lanzar InstanceNotFoundException
                Course course = courseDao.findCourseById(connection,courseId);

                // si no hay plazas disponibles
                if (Short.valueOf(course.getfreePlaces()).equals((short) 0)){
                    throw new NotEnoughPlacesException(courseId);
                }

                // si el curso ya ha empezado
                if (LocalDateTime.now().isAfter(course.getStartDate())){
                    throw new CourseAlreadyStartedException(courseId);
                }

                // si el usuario ya estaba inscrito
                List<Inscription> listInscription = inscriptionDao.findByUser(connection,userEmail);
                Boolean exists = false;
                for (Inscription i: listInscription){
                    if((i.getUserEmail().equals(userEmail)) && (Objects.equals(i.getCourseId(), courseId)))
                        exists = true;
                }

                if (exists)
                    throw new CourseAlreadyJoinedException(courseId);

                // actualizamos el numero de plazas reservadas del curso
                course.setfreePlaces((short) (course.getfreePlaces() - 1));

                Inscription inscription = inscriptionDao.create(connection,new Inscription(courseId,userEmail,
                        creditCardNumber,LocalDateTime.now()));

                courseDao.update(connection,course);
                connection.commit();

                return inscription;
            }catch (InstanceNotFoundException e) {
                connection.commit();
                throw e;
            } catch (SQLException e) {
                connection.rollback();
                throw new RuntimeException(e);
            } catch (RuntimeException | Error e) {
                connection.rollback();
                throw e;
            }
        } catch (SQLException e){
            throw new RuntimeException(e);
        }
    }

    /*
     * [FUNC-5] cancelInscription: cancelar una inscripcion a un curso
     *                 En caso de no encontrar dicho curso, se lanzaria una InstanceNotFoundException
     *                 En caso de introducir algun dato erroneo, se lanzaria una InputValidationException
     *                 En caso de que la inscripcion ya se haya cancelado, se lanzaria una AlreadyCancelledException
     *                 En caso de cancelar la inscripcion fuera del plazo de cancelacion, se lanzaria una OutOfDateException
     *                 En caso de que se intente cancelar la inscripcion de otro usuario, se lanzaria una WrongUserException
     *                 En caso de no poder establecer la conexion, se lanzaria una RuntimeException
     */
    @Override
    public void cancelInscription(Long inscriptionId, String userEmail) throws InputValidationException, InstanceNotFoundException, AlreadyCancelledException, OutOfDateException, WrongUserException {
        // Validamos los datos introducidos:
        if(!isValidEmail(userEmail)) { // correo electronico
            throw new InputValidationException("User email is not valid.");
        }

        if (inscriptionId == null || inscriptionId < 0){ // identificador del curso
            throw new InputValidationException("ID" + inscriptionId + "is not valid");
        }

        try(Connection connection = dataSource.getConnection()){
            try {
                connection.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
                connection.setAutoCommit(false);
                Inscription inscription = inscriptionDao.findByUserId(connection,inscriptionId);

                // comprobamos que el usuario es el mismo que se inscribio
                if (!inscription.getUserEmail().equals(userEmail)){
                    throw new WrongUserException(inscriptionId);
                }

                // comprobamos que la inscripcion no este cancelada ya
                if (inscription.getCancelDate() != null){
                    throw new AlreadyCancelledException(inscriptionId);
                }

                // comprobamos que el plazo de cancelacion aun esta abierto
                Course course = courseDao.findCourseById(connection,inscription.getCourseId());
                LocalDateTime actualTime = LocalDateTime.now().withNano(0);
                long timeRemain = ChronoUnit.DAYS.between(actualTime, course.getStartDate());
                if (timeRemain < 7){
                    throw new OutOfDateException(course.getCourseId());
                }

                // actualizamos el numero de plazas reservadas del curso
                course.setfreePlaces((short) (course.getfreePlaces() + 1));

                // marcamos la inscripcion como cancelada
                inscription.setCancelDate(actualTime);

                // actualizamos la informacion
                courseDao.update(connection,course);
                inscriptionDao.update(connection,inscription);

                connection.commit();
            } catch (InstanceNotFoundException | AlreadyCancelledException | OutOfDateException | WrongUserException e) {
                connection.rollback();
                throw e;
            } catch (SQLException e) {
                connection.rollback();
                throw new RuntimeException(e);
            } catch (RuntimeException | Error e) {
                connection.rollback();
                throw e;
            }
        } catch (SQLException e){
            throw new RuntimeException(e);
        }
    }

     /*
     * [FUNC-6] findInscriptions: buscar todas las inscripciones de un usuario
     *                 En caso de que el correo no sea valido, se lanzaria una InputValidationException
     *                 En caso de no poder establecer la conexion, se lanzaria una RuntimeException
     */
    @Override
    public List<Inscription> findInscriptions(String userEmail) throws InputValidationException {
        // Validamos los datos introducidos:
        if(!isValidEmail(userEmail)) { // correo electronico
            throw new InputValidationException("User email is not valid.");
        }

        try (Connection connection = dataSource.getConnection()){
            return inscriptionDao.findByUser(connection,userEmail);
        } catch (SQLException e){
            throw new RuntimeException(e);
        }
    }
    
}
