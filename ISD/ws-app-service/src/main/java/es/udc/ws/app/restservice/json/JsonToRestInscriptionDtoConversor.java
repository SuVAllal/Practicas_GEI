package es.udc.ws.app.restservice.json;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import com.fasterxml.jackson.databind.node.JsonNodeFactory;
import com.fasterxml.jackson.databind.node.JsonNodeType;
import com.fasterxml.jackson.databind.node.ObjectNode;
import es.udc.ws.app.restservice.dto.RestInscriptionDto;
import es.udc.ws.util.json.ObjectMapperFactory;
import es.udc.ws.util.json.exceptions.ParsingException;


import java.io.InputStream;
import java.time.LocalDateTime;
import java.util.List;

public class JsonToRestInscriptionDtoConversor {
    public static ObjectNode toObjectNode(RestInscriptionDto inscription){

        ObjectNode inscriptionObjectNode = JsonNodeFactory.instance.objectNode();
        inscriptionObjectNode.put("inscriptionId", inscription.getInscriptionId())
                .put("courseId", inscription.getCourseId())
                .put("userEmail", inscription.getUserEmail());

        // Solo incluimos los últimos 4 dígitos de la tarjeta de crédito
        String creditCardLast4 = inscription.getCreditCardNumber() != null
                ? inscription.getCreditCardNumber().substring(inscription.getCreditCardNumber().length() - 4)
                : null;
        inscriptionObjectNode.put("creditCardNumber", creditCardLast4);
        inscriptionObjectNode.put("inscriptionDate", inscription.getInscriptionDate().toString());
        inscriptionObjectNode.put("cancelDate", inscription.getCancelDate() != null ? inscription.getCancelDate().toString() : null);

        return inscriptionObjectNode;
    }



    public static ArrayNode toArrayNode(List<RestInscriptionDto> listInscription){
        ArrayNode inscriptionNode = JsonNodeFactory.instance.arrayNode();
        for (RestInscriptionDto inscriptionDto : listInscription){
            ObjectNode inscriptionObject = toObjectNode(inscriptionDto);
            inscriptionNode.add(inscriptionObject);
        }
        return inscriptionNode;
    }


    public static RestInscriptionDto toRestInscriptionDto(InputStream jsonInscription) throws ParsingException {
        try {
            ObjectMapper objectMapper = ObjectMapperFactory.instance();
            JsonNode jsonNode = objectMapper.readTree(jsonInscription);
            if (jsonNode.getNodeType() != JsonNodeType.OBJECT) {
                throw new ParsingException("Object expected");
            } else {
                ObjectNode inscriptionObjectNode = (ObjectNode) jsonNode;

                Long inscriptionId = (inscriptionObjectNode.has("inscriptionId") && !inscriptionObjectNode.get("inscriptionId").isNull())
                        ? inscriptionObjectNode.get("inscriptionId").longValue()
                        : null;

                Long courseId = (inscriptionObjectNode.has("courseId") && !inscriptionObjectNode.get("courseId").isNull())
                        ? inscriptionObjectNode.get("courseId").longValue()
                        : null;

                String userEmail = (inscriptionObjectNode.has("userEmail") && !inscriptionObjectNode.get("userEmail").isNull())
                        ? inscriptionObjectNode.get("userEmail").textValue().trim()
                        : null;

                String creditCardNumber = (inscriptionObjectNode.has("creditCardNumber") && !inscriptionObjectNode.get("creditCardNumber").isNull())
                        ? inscriptionObjectNode.get("creditCardNumber").textValue().trim()
                        : null;

                JsonNode inscriptionDateNode = (inscriptionObjectNode.has("inscriptionDate") && !inscriptionObjectNode.get("inscriptionDate").isNull())
                        ? inscriptionObjectNode.get("inscriptionDate")
                        : null;
                
                LocalDateTime inscriptionDate = (inscriptionDateNode != null && !inscriptionDateNode.isNull())
                        ? LocalDateTime.parse(inscriptionDateNode.textValue().trim())
                        : null;

                //comprobamos que no sea nula la fecha de cancelacion
                JsonNode cancelDateNode = inscriptionObjectNode.get("cancelDate");
                LocalDateTime cancelDate = (cancelDateNode != null && !cancelDateNode.isNull())
                        ? LocalDateTime.parse(cancelDateNode.textValue().trim())
                        : null;

                return new RestInscriptionDto(inscriptionId, courseId, userEmail, creditCardNumber, inscriptionDate, cancelDate);
            }
        } catch (ParsingException e) {
            throw e;
        } catch (Exception e) {
            throw new ParsingException(e);
        }    }
}
