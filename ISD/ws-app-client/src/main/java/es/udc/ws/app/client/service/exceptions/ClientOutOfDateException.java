package es.udc.ws.app.client.service.exceptions;

public class ClientOutOfDateException extends Exception{
    private Long courseId;

    public ClientOutOfDateException(Long courseId) {
        super("Course with id=\"" + courseId + "\n is out of date to cancel");
        this.courseId = courseId;
    }

    public Long getCourseId() {
        return courseId;
    }

    public void setCourseId(Long courseId) {
        this.courseId = courseId;
    }
}
