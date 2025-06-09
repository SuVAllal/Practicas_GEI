package es.udc.ws.app.model.course;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Timestamp;

public class Jdbc3CcSqlCourseDao extends AbstractSqlCourseDao {

    /*
     * create: dar de alta un curso
     *                  En caso de no poder realizar el alta, se lanzaria una RuntimeException
     */
    @Override
    public Course create(Connection connection, Course course) {
        String queryString = "INSERT INTO Course"
                + " (name, city, startDate, price, maxPlaces, freePlaces, creationDate)"
                + " VALUES (?, ?, ?, ?, ?, ?, ?)";

        try (PreparedStatement preparedStatement = connection.prepareStatement(
                queryString, Statement.RETURN_GENERATED_KEYS)) {

            int i = 1;
            preparedStatement.setString(i++, course.getName());
            preparedStatement.setString(i++, course.getCity());
            preparedStatement.setTimestamp(i++, Timestamp.valueOf(course.getStartDate()));
            preparedStatement.setFloat(i++, course.getPrice());
            preparedStatement.setShort(i++, course.getMaxPlaces());
            preparedStatement.setShort(i++, course.getfreePlaces());
            preparedStatement.setTimestamp(i++, Timestamp.valueOf(course.getCreationDate()));

            preparedStatement.executeUpdate();

            ResultSet resultSet = preparedStatement.getGeneratedKeys();

            if(!resultSet.next()){
                throw new SQLException(
                        "JDBC driver did not return generated key.");
            }
            Long courseId = resultSet.getLong(1);

            return new Course(courseId, course.getName(), course.getCity(),
                    course.getStartDate(), course.getPrice(),
                    course.getMaxPlaces(), course.getCreationDate(), course.getfreePlaces());

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }

    }
}
