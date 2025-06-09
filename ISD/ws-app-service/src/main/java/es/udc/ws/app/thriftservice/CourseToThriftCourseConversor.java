package es.udc.ws.app.thriftservice;

import es.udc.ws.app.model.course.Course;
import es.udc.ws.courses.thrift.ThriftCourseDto;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

public class CourseToThriftCourseConversor {
    public static Course toCourse(ThriftCourseDto course){
        return new Course(course.getCourseId(), course.getName(), course.getCity(),
                LocalDateTime.parse(course.getStartDate()), Double.valueOf(course.getPrice()).floatValue(), course.getMaxPlaces());
    }
    public static ThriftCourseDto toThriftCourseDto(Course course){
        return new ThriftCourseDto(course.getCourseId(), course.getName(), course.getCity(),
                course.getStartDate().toString(), Float.valueOf(course.getPrice()).doubleValue(), course.getMaxPlaces(),
                course.getfreePlaces());
    }
    public static List<ThriftCourseDto> toThriftCourseDtos(List<Course> listCourse){
        List<ThriftCourseDto> listThriftCourse = new ArrayList<>(listCourse.size());
        for (Course course : listCourse){
            listThriftCourse.add(toThriftCourseDto(course));
        }
        return listThriftCourse;
    }
}
