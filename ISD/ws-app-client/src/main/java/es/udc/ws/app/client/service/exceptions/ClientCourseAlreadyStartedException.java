package es.udc.ws.app.client.service.exceptions;

public class ClientCourseAlreadyStartedException extends Exception{
    Long courseId;
    public ClientCourseAlreadyStartedException(Long courseId) {
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
