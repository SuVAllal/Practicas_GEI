package es.udc.ws.app.model.courseservice;

import es.udc.ws.app.model.course.Course;
import es.udc.ws.app.model.courseservice.exceptions.*;
import es.udc.ws.app.model.inscription.Inscription;
import es.udc.ws.util.exceptions.InputValidationException;
import es.udc.ws.util.exceptions.InstanceNotFoundException;

import java.time.LocalDateTime;
import java.util.List;

public interface CourseService {
    public Course addCourse(Course course) throws InputValidationException;

    public List<Course> findCourses(String city, LocalDateTime startDate)
            throws InputValidationException;

    public Course findCourse(Long courseId) throws InstanceNotFoundException, InputValidationException;

    public Inscription joinCourse(String userEmail, Long courseId, String creditCardNumber)
            throws InputValidationException, InstanceNotFoundException, NotEnoughPlacesException, CourseAlreadyStartedException, CourseAlreadyJoinedException;

    public void cancelInscription(Long inscriptionId, String userEmail)
            throws InputValidationException, InstanceNotFoundException, AlreadyCancelledException, OutOfDateException, WrongUserException;

    public List<Inscription> findInscriptions(String userEmail)
            throws InputValidationException;
}
