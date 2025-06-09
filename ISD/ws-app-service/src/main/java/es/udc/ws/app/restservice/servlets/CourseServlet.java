package es.udc.ws.app.restservice.servlets;

import es.udc.ws.app.model.course.Course;
import es.udc.ws.app.model.courseservice.CourseServiceFactory;
import es.udc.ws.app.restservice.dto.CourseToRestCourseDtoConversor;
import es.udc.ws.app.restservice.dto.RestCourseDto;
import es.udc.ws.app.restservice.json.JsonToRestCourseDtoConversor;
import es.udc.ws.util.exceptions.InputValidationException;
import es.udc.ws.util.exceptions.InstanceNotFoundException;
import es.udc.ws.util.servlet.RestHttpServletTemplate;
import es.udc.ws.util.servlet.ServletUtils;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

import java.io.IOException;
import java.time.LocalDateTime;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class CourseServlet extends RestHttpServletTemplate {

    @Override
    protected void processGet(HttpServletRequest req, HttpServletResponse resp) throws IOException, InstanceNotFoundException, InputValidationException {
        if (req.getPathInfo() == null || req.getPathInfo().equals("/")){
            String city = req.getParameter("city");
            List<Course> listCourses = CourseServiceFactory.getService().findCourses(city, LocalDateTime.now());
            List<RestCourseDto> listRestCourses = CourseToRestCourseDtoConversor.toRestCourseDtos(listCourses);
            ServletUtils.writeServiceResponse(resp, HttpServletResponse.SC_OK, JsonToRestCourseDtoConversor.toArrayNode(listRestCourses),null);
        } else {
            Long courseId = ServletUtils.getIdFromPath(req, "courseId");
            Course course = CourseServiceFactory.getService().findCourse(courseId);
            RestCourseDto courseDto = CourseToRestCourseDtoConversor.toRestCourseDto(course);
            ServletUtils.writeServiceResponse(resp, HttpServletResponse.SC_OK,
                    JsonToRestCourseDtoConversor.toObjectNode(courseDto), null);
        }
    }

    @Override
    protected void processPost(HttpServletRequest req, HttpServletResponse resp) throws IOException, InstanceNotFoundException, InputValidationException {
        ServletUtils.checkEmptyPath(req);

        // conversion de json -> dto -> objeto
        RestCourseDto courseDto = JsonToRestCourseDtoConversor.toRestCourseDto(req.getInputStream());
        Course course = CourseToRestCourseDtoConversor.toCourse(courseDto);

        course = CourseServiceFactory.getService().addCourse(course); // invocamos la capa modelo

        // creacion de la respuesta
        courseDto = CourseToRestCourseDtoConversor.toRestCourseDto(course);
        String courseURL = ServletUtils.normalizePath(req.getRequestURL().toString() + "/" + course.getCourseId());
        Map<String, String> headers = new HashMap<>(1);
        headers.put("Location", courseURL);
        ServletUtils.writeServiceResponse(resp,HttpServletResponse.SC_CREATED,
                JsonToRestCourseDtoConversor.toObjectNode(courseDto),headers);
    }
}
