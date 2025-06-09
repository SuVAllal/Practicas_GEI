package es.udc.ws.app.client.service.exceptions;

public class ClientNotEnoughPlacesException extends Exception{
    private Long courseId;
    public ClientNotEnoughPlacesException(Long courseId){
        super("Course with id=\"" + courseId + "\n is full");
        this.courseId = courseId;
    }

    public Long getCourseId() {
        return courseId;
    }

    public void setCourseId(Long courseId) {
        this.courseId = courseId;
    }
}
