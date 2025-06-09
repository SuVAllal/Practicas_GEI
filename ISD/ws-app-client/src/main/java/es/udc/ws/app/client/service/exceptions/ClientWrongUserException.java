package es.udc.ws.app.client.service.exceptions;

public class ClientWrongUserException extends Exception{
    private Long inscriptionId;

    public ClientWrongUserException(Long inscriptionId) {
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
