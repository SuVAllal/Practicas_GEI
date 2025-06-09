package es.udc.ws.app.model.course;

import es.udc.ws.util.exceptions.InstanceNotFoundException;

import java.sql.Connection;
import java.time.LocalDateTime;
import java.util.List;

public interface SqlCourseDao {

    public Course create(Connection connection, Course course);

    public Course findCourseById(Connection connection, Long courseId)
            throws InstanceNotFoundException;

    public List<Course> findCourses(Connection connection, String city,
                                    LocalDateTime startDate);

    public void update(Connection connection, Course course)
            throws InstanceNotFoundException;

    public void remove(Connection connection, Long courseId)
            throws InstanceNotFoundException;
}
