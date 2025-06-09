package es.udc.ws.app.model.course;

import es.udc.ws.util.exceptions.InstanceNotFoundException;

import java.sql.*;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

public abstract class AbstractSqlCourseDao implements SqlCourseDao {
    protected AbstractSqlCourseDao() {

    }

    /*
     * findCourseById: buscar cursos por su identificador
     *                 En caso de no encontrar dicho curso, se lanzaria una InstanceNotFoundException
     *                 En caso de no poder realizar la busqueda, se lanzaria una RuntimeException
     */
    @Override
    public Course findCourseById(Connection connection, Long courseId)
            throws InstanceNotFoundException {

        String queryString = "SELECT name, city, startDate, price, maxPlaces, freePlaces, creationDate "
                + "FROM Course WHERE courseId = ?";

        try (PreparedStatement preparedStatement = connection.prepareStatement(queryString)) {
            int i = 1;
            preparedStatement.setLong(i++, courseId.longValue());

            ResultSet resultSet = preparedStatement.executeQuery();

            // No se ha encontrado un Curso con ese identificador
            if (!resultSet.next()) {
                throw new InstanceNotFoundException(courseId,
                        Course.class.getName());
            }

            // Recogemos toda la informacion resultante de la busqueda
            i = 1;
            String name = resultSet.getString(i++);
            String city = resultSet.getString(i++);
            Timestamp startDateAsTimestamp = resultSet.getTimestamp(i++);
            LocalDateTime startDate = startDateAsTimestamp.toLocalDateTime();
            float price = resultSet.getFloat(i++);
            short maxPlaces = resultSet.getShort(i++);
            short freePlaces = resultSet.getShort(i++);
            Timestamp creationDateAsTimestamp = resultSet.getTimestamp(i++);
            LocalDateTime creationDate = creationDateAsTimestamp.toLocalDateTime();

            // Devolvemos un nuevo Curso que representa al Curso que se ha encontrado
            return new Course(courseId, name, city, startDate, price, maxPlaces, creationDate, freePlaces);
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    /*
     * findCourses: buscar cursos que se celebren en una ciudad a partir de una fecha
     *                       En caso de no poder realizar la busqueda, se lanzaria una RuntimeException
     */
    @Override
    public List<Course> findCourses(Connection connection, String city, LocalDateTime startDate) {
        String queryString = "SELECT * FROM Course WHERE city = ? AND startDate >= ? ORDER BY startDate";

        try (PreparedStatement preparedStatement = connection.prepareStatement(queryString)){
            preparedStatement.setString(1, city);
            preparedStatement.setTimestamp(2, Timestamp.valueOf(startDate));

            ResultSet resultSet = preparedStatement.executeQuery();

            List<Course> courseList = new ArrayList<>();

            while (resultSet.next()){
                Long courseIdEntry = resultSet.getLong("courseId");
                String nameEntry = resultSet.getString("name");
                String cityEntry = resultSet.getString("city");
                LocalDateTime startDateEntry = resultSet.getTimestamp("startDate").toLocalDateTime();
                float priceEntry = resultSet.getFloat("price");
                short maxPlacesEntry = resultSet.getShort("maxPlaces");
                short freePlacesEntry = resultSet.getShort("freePlaces");
                LocalDateTime creationDateEntry = resultSet.getTimestamp("creationDate").toLocalDateTime();
                Course courseEntry = new Course(courseIdEntry,nameEntry,cityEntry,startDateEntry,priceEntry,maxPlacesEntry,creationDateEntry,freePlacesEntry);
                courseList.add(courseEntry);
            }
            return courseList;
        }catch (SQLException e){
            throw new RuntimeException();
        }
    }


    /*
     * update: modificar los parametros de un curso ya creado (actualizar su informacion)
     *         En caso de no encontrar dicho curso, se lanzaria una InstanceNotFoundException
     *         En caso de no poder realizar la busqueda, se lanzaria una RuntimeException
     */
    @Override
    public void update(Connection connection, Course course) throws InstanceNotFoundException {
        // Create queryString
        String query = "UPDATE Course SET name = ?, city = ?, startDate = ?, " +
                "price = ?, maxPlaces = ?, freePlaces = ?, creationDate = ? " +
                "WHERE courseId = ?";

        try(PreparedStatement preparedStatement = connection.prepareStatement(query)){
            // Fill preparedStatement
            int i = 1;
            preparedStatement.setString(i++, course.getName());
            preparedStatement.setString(i++, course.getCity());
            preparedStatement.setTimestamp(i++, Timestamp.valueOf(course.getStartDate()));
            preparedStatement.setFloat(i++, course.getPrice());
            preparedStatement.setShort(i++, course.getMaxPlaces());
            preparedStatement.setShort(i++, course.getfreePlaces());
            preparedStatement.setTimestamp(i++, Timestamp.valueOf(course.getCreationDate()));
            preparedStatement.setLong(i, course.getCourseId());

            // Execute query
            int updatedRows = preparedStatement.executeUpdate();

            if (updatedRows == 0){
                throw new InstanceNotFoundException(course.getCourseId(),
                        Course.class.getName());
            }

        } catch (SQLException e){
            throw new RuntimeException(e);
        }

    }
    /*
     * remove: eliminar un curso de la Base de Datos. Necesario para los tests
     *         En caso de no poder realizar la busqueda, se lanzaria una RuntimeException
     */
    @Override
    public void remove(Connection connection, Long courseId)
            throws InstanceNotFoundException {

        // Create queryString
        String queryString = "DELETE FROM Course WHERE courseId = ?";

        try (PreparedStatement preparedStatement = connection.prepareStatement(queryString)) {
            // Fill preparedStatement
            int i = 1;
            preparedStatement.setLong(i++, courseId);

            // Execute query
            int removedRows = preparedStatement.executeUpdate();

            if(removedRows == 0) {
                throw new InstanceNotFoundException(courseId,
                        Course.class.getName());
            }

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }

    }
}
