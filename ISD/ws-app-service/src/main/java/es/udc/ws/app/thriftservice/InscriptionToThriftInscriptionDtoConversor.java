package es.udc.ws.app.thriftservice;

import es.udc.ws.app.model.inscription.Inscription;
import es.udc.ws.courses.thrift.ThriftInscriptionDto;

public class InscriptionToThriftInscriptionDtoConversor {
    public static ThriftInscriptionDto toThriftInscriptionDto(Inscription inscription) {

        return new ThriftInscriptionDto(inscription.getInscriptionId(), inscription.getCourseId(),
                inscription.getUserEmail(), inscription.getCreditCardNumber(),
                (inscription.getInscriptionDate() != null) ? inscription.getInscriptionDate().toString() : "No establecida",
                (inscription.getCancelDate() != null) ? inscription.getCancelDate().toString() : "No establecida");
    }

}
