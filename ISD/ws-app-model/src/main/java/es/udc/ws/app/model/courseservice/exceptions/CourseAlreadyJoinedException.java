package es.udc.ws.app.model.courseservice.exceptions;

public class CourseAlreadyJoinedException extends Exception{
    Long courseId;
    public CourseAlreadyJoinedException(Long courseId) {
        super("Course with id=\"" + courseId + "\n already joined");
        this.courseId = courseId;
    }

    public Long getCourseId() {
        return courseId;
    }

    public void setCourseId(Long courseId) {
        this.courseId = courseId;
    }
}
