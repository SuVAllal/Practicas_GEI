package es.udc.ws.app.model.courseservice.exceptions;

public class WrongUserException extends Exception{
    private Long inscriptionId;

    public WrongUserException(Long inscriptionId) {
        super("Inscription with id=\"" + inscriptionId + "\n is done by another user");
        this.inscriptionId = inscriptionId;
    }

    public Long getInscriptionId() {
        return inscriptionId;
    }

    public void setInscriptionId(Long inscriptionId) {
        this.inscriptionId = inscriptionId;
    }
}
