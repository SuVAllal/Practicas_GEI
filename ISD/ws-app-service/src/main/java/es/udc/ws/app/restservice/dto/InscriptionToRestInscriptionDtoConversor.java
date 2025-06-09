package es.udc.ws.app.restservice.dto;

import es.udc.ws.app.model.inscription.Inscription;

import java.util.List;
import java.util.ArrayList;

public class InscriptionToRestInscriptionDtoConversor {
    public static List<RestInscriptionDto> toRestInscriptionDtos(List<Inscription> inscriptions){
        List<RestInscriptionDto> listInscriptionDto = new ArrayList<>(inscriptions.size());
        for (Inscription inscription : inscriptions) {
            listInscriptionDto.add(toRestInscriptionDto(inscription));
        }
        return listInscriptionDto;
    }
    public static RestInscriptionDto toRestInscriptionDto(Inscription inscription){
        //obtenemos el substring de la targeta de credito con sus ultimos 4 digitos
        String creditCardLast4 = inscription.getCreditCardNumber() != null
                ? inscription.getCreditCardNumber().substring(inscription.getCreditCardNumber().length() - 4)
                : null;

        return new RestInscriptionDto(inscription.getInscriptionId(), inscription.getCourseId(), inscription.getUserEmail(),
                creditCardLast4, inscription.getInscriptionDate(), inscription.getCancelDate());
    }
    public static Inscription toInscription(RestInscriptionDto inscription){
        return new Inscription( inscription.getInscriptionId(), inscription.getCourseId(), inscription.getUserEmail(),
                inscription.getCreditCardNumber(),inscription.getInscriptionDate(), inscription.getCancelDate());
    }
}
