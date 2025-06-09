package es.udc.ws.app.model.courseservice.exceptions;

public class CourseAlreadyStartedException extends Exception{
    Long courseId;
    public CourseAlreadyStartedException(Long courseId) {
        super("Course with id=\"" + courseId + "\n already started");
        this.courseId = courseId;
    }

    public Long getCourseId() {
        return courseId;
    }

    public void setCourseId(Long courseId) {
        this.courseId = courseId;
    }
}
