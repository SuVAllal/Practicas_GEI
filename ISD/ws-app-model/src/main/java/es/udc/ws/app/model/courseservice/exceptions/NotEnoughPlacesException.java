package es.udc.ws.app.model.courseservice.exceptions;

public class NotEnoughPlacesException extends Exception{
    private Long courseId;
    public NotEnoughPlacesException(Long courseId){
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
