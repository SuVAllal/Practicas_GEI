package es.udc.ws.app.model.courseservice.exceptions;

public class AlreadyCancelledException extends Exception {
    private Long inscriptionId;
    public AlreadyCancelledException(Long inscriptionId){
        super("Inscription with id=\"" + inscriptionId + "\n is already cancelled");
        this.inscriptionId = inscriptionId;
    }

    public Long getInscriptionId() {
        return inscriptionId;
    }

    public void setInscriptionId(Long inscriptionId) {
        this.inscriptionId = inscriptionId;
    }
}
