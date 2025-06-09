package es.udc.ws.app.client.service.exceptions;

public class ClientAlreadyCancelledException extends Exception {
    private Long inscriptionId;
    public ClientAlreadyCancelledException(Long inscriptionId){
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
