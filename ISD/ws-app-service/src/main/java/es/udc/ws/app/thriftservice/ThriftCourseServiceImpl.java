package es.udc.ws.app.thriftservice;

import es.udc.ws.app.model.course.Course;
import es.udc.ws.app.model.courseservice.CourseServiceFactory;
import es.udc.ws.app.model.courseservice.exceptions.CourseAlreadyJoinedException;
import es.udc.ws.app.model.courseservice.exceptions.CourseAlreadyStartedException;
import es.udc.ws.app.model.courseservice.exceptions.NotEnoughPlacesException;
import es.udc.ws.app.model.inscription.Inscription;
import es.udc.ws.courses.thrift.*;
import es.udc.ws.util.exceptions.InputValidationException;
import es.udc.ws.util.exceptions.InstanceNotFoundException;
import org.apache.thrift.TException;

import java.time.LocalDateTime;
import java.util.List;

public class ThriftCourseServiceImpl implements ThriftCourseService.Iface {
    @Override
    public ThriftCourseDto addCourse(ThriftCourseDto course) throws TException {
        Course courseConverted = CourseToThriftCourseConversor.toCourse(course);
        try {
            Course addedCourse = CourseServiceFactory.getService().addCourse(courseConverted);
            return CourseToThriftCourseConversor.toThriftCourseDto(addedCourse);
        } catch (InputValidationException e){
            throw new ThriftInputValidationException(e.getMessage());
        }
    }

    @Override
    public List<ThriftCourseDto> findCourses(String city) throws TException {
        try {
            List<Course> listCourse = CourseServiceFactory.getService().findCourses(city, LocalDateTime.now());
            return CourseToThriftCourseConversor.toThriftCourseDtos(listCourse);
        } catch (InputValidationException e){
            throw new ThriftInputValidationException(e.getMessage());
        }
    }

    @Override
    public ThriftCourseDto findCourse(long courseId) throws ThriftInstanceNotFoundException, ThriftInputValidationException {
        try {
            Course course = CourseServiceFactory.getService().findCourse(courseId);
            return CourseToThriftCourseConversor.toThriftCourseDto(course);
        } catch (InstanceNotFoundException e) {
            throw new ThriftInstanceNotFoundException(e.getInstanceId().toString(),
                    e.getInstanceType().substring(e.getInstanceType().lastIndexOf('.') + 1));
        } catch (InputValidationException e) {
            throw new ThriftInputValidationException(e.getMessage());
        }
    }

    @Override
    public ThriftInscriptionDto joinCourse(String userEmail, long courseId, String creditCardNumber) throws ThriftInputValidationException, ThriftInstanceNotFoundException, ThriftNotEnoughPlacesException, ThriftCourseAlreadyJoinedException, ThriftCourseAlreadyStartedException {
        try {
            Inscription inscription = CourseServiceFactory.getService().joinCourse(userEmail, courseId, creditCardNumber);
            return InscriptionToThriftInscriptionDtoConversor.toThriftInscriptionDto(inscription);
        } catch (NotEnoughPlacesException e) {
            throw new ThriftNotEnoughPlacesException(e.getMessage());
        } catch (InstanceNotFoundException e) {
            throw new ThriftInstanceNotFoundException(e.getInstanceId().toString(),
                    e.getInstanceType().substring(e.getInstanceType().lastIndexOf('.') + 1));
        } catch (InputValidationException e) {
            throw new ThriftInputValidationException(e.getMessage());
        } catch (CourseAlreadyJoinedException e) {
            throw new ThriftCourseAlreadyJoinedException(e.getMessage());
        } catch (CourseAlreadyStartedException e) {
            throw new ThriftCourseAlreadyStartedException(e.getMessage());
        }
    }
}
