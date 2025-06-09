package es.udc.ws.app.model.courseservice.exceptions;

public class OutOfDateException extends Exception{
    private Long courseId;

    public OutOfDateException(Long courseId) {
        super("Course with id=\"" + courseId + "\n is out of date to join");
        this.courseId = courseId;
    }

    public Long getCourseId() {
        return courseId;
    }

    public void setCourseId(Long courseId) {
        this.courseId = courseId;
    }
}
