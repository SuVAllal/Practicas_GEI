package es.udc.ws.app.client.service.rest.json;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import com.fasterxml.jackson.databind.node.JsonNodeType;
import com.fasterxml.jackson.databind.node.ObjectNode;
import es.udc.ws.app.client.service.dto.ClientInscriptionDto;
import es.udc.ws.util.json.ObjectMapperFactory;
import es.udc.ws.util.json.exceptions.ParsingException;

import java.io.InputStream;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

public class JsonToClientInscriptionDtoConversor {
    public static ClientInscriptionDto toClientInscriptionDto(InputStream jsonInscription) throws ParsingException {
        try {
            ObjectMapper objectMapper = ObjectMapperFactory.instance();
            JsonNode rootNode = objectMapper.readTree(jsonInscription);

            if(rootNode.getNodeType() != JsonNodeType.OBJECT) {
                throw new ParsingException("Unrecognized JSON (object expected)");
            } else {
                return toClientInscriptionDto(rootNode);
            }
        } catch(ParsingException ex) {
            throw ex;
        } catch (Exception e) {
            throw new ParsingException(e);
        }
    }

    public static List<ClientInscriptionDto> toClientInscriptionDtos(InputStream jsonInscriptions) throws ParsingException {
        try {
            ObjectMapper objectMapper = ObjectMapperFactory.instance();
            JsonNode rootNode = objectMapper.readTree(jsonInscriptions);

            if(rootNode.getNodeType() != JsonNodeType.ARRAY) {
                throw new ParsingException("Unrecognized JSON (array expected)");
            } else {
                ArrayNode inscriptionsArray = (ArrayNode) rootNode;
                List<ClientInscriptionDto> inscriptionDtos = new ArrayList<>(inscriptionsArray.size());

                for(JsonNode inscriptionNode : inscriptionsArray) {
                    inscriptionDtos.add(toClientInscriptionDto(inscriptionNode));
                }
                return inscriptionDtos;
            }
        } catch (ParsingException exception) {
            throw exception;
        } catch (Exception ex) {
            throw new ParsingException(ex);
        }
    }


    private static ClientInscriptionDto toClientInscriptionDto(JsonNode inscriptionNode) throws ParsingException {
        if(inscriptionNode.getNodeType() != JsonNodeType.OBJECT) {
            throw new ParsingException("Unrecognized JSON (object expected).");
        } else {
            ObjectNode inscriptionObject = (ObjectNode) inscriptionNode;

            // Verificar si el nodo inscriptionId existe y no es nulo
            JsonNode inscriptionIdNode = inscriptionObject.get("inscriptionId");
            Long inscriptionId = (inscriptionIdNode != null) ? inscriptionNode.get("inscriptionId").longValue() : null;

            // Verificar si el nodo courseId existe y no es nulo
            Long courseId = inscriptionNode.get("courseId").longValue();


            // Verificar si el nodo userEmail existe y no es nulo
            String userEmail = inscriptionNode.get("userEmail").textValue().trim();

            // Verificar si el nodo creditCardNumber existe y no es nulo
            String creditCardNumber = (inscriptionNode.has("creditCardNumber") && !inscriptionNode.get("creditCardNumber").isNull())
                    ? inscriptionNode.get("creditCardNumber").textValue().trim()
                    : null;

            LocalDateTime inscriptionDate = (inscriptionObject.has("inscriptionDate") && !inscriptionObject.get("inscriptionDate").isNull())
                    ? LocalDateTime.parse(inscriptionObject.get("inscriptionDate").textValue().trim())
                    : null;

            LocalDateTime cancelDate = (inscriptionObject.has("cancelDate") && !inscriptionObject.get("cancelDate").isNull())
                    ? LocalDateTime.parse(inscriptionObject.get("cancelDate").textValue().trim())
                    : null;


            // Crear y devolver el objeto ClientInscriptionDto
            return new ClientInscriptionDto(inscriptionId, courseId, userEmail, creditCardNumber, inscriptionDate, cancelDate);
        }
    }
}
