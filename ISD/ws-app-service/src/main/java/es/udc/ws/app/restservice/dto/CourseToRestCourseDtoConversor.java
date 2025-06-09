package es.udc.ws.app.restservice.dto;

import es.udc.ws.app.model.course.Course;

import java.util.ArrayList;
import java.util.List;

public class CourseToRestCourseDtoConversor {
    public static List<RestCourseDto> toRestCourseDtos(List<Course> listCourse){
        List<RestCourseDto> listCourseDto = new ArrayList<>(listCourse.size());
        for (Course course : listCourse){
            listCourseDto.add(toRestCourseDto(course));
        }
        return listCourseDto;
    }
    public static RestCourseDto toRestCourseDto(Course course){
        return new RestCourseDto(course.getCourseId(), course.getName(), course.getCity(), course.getStartDate(),
                course.getPrice(), course.getMaxPlaces(), course.getfreePlaces());
    }
    public static Course toCourse(RestCourseDto course){
        return new Course(course.getCourseId(),course.getName(),course.getCity(),course.getStartDate(),course.getPrice(),
                course.getMaxPlaces(),null, course.getFreePlaces());
    }
}
