package es.udc.ws.app.client.service.exceptions;

public class ClientCourseAlreadyJoinedException extends Exception{
    Long courseId;
    public ClientCourseAlreadyJoinedException(Long courseId) {
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
