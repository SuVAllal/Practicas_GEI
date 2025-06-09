package es.udc.ws.app.client.service.rest.json;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.JsonNodeType;
import es.udc.ws.app.client.service.exceptions.*;
import es.udc.ws.util.exceptions.InputValidationException;
import es.udc.ws.util.exceptions.InstanceNotFoundException;
import es.udc.ws.util.json.ObjectMapperFactory;
import es.udc.ws.util.json.exceptions.ParsingException;

import java.io.InputStream;

public class JsonToClientExceptionConversor {
    public static Exception fromBadRequestErrorCode(InputStream ex) throws ParsingException{
        try{
            ObjectMapper objectMapper = ObjectMapperFactory.instance();
            JsonNode rootNode = objectMapper.readTree(ex);
            if (rootNode.getNodeType() != JsonNodeType.OBJECT){
                throw new ParsingException("Unrecognized JSON (object expected)");
            } else {
                String errorType = rootNode.get("errorType").textValue();
                if (errorType.equals("InputValidation")) {
                    return toInputValidationException(rootNode);
                } else {
                    throw new ParsingException("Unrecognized error type: " + errorType);
                }
            }
        } catch (ParsingException e){
            throw e;
        } catch (Exception e){
            throw new ParsingException(e);
        }
    }
    public static Exception fromNotFoundErrorCode(InputStream ex) throws ParsingException {
        try {
            ObjectMapper objectMapper = ObjectMapperFactory.instance();
            JsonNode rootNode = objectMapper.readTree(ex);
            if (rootNode.getNodeType() != JsonNodeType.OBJECT) {
                throw new ParsingException("Unrecognized JSON (object expected)");
            } else {
                String errorType = rootNode.get("errorType").textValue();
                if (errorType.equals("InstanceNotFound")) {
                    return toInstanceNotFoundException(rootNode);
                } else {
                    throw new ParsingException("Unrecognized error type: " + errorType);
                }
            }
        } catch (ParsingException e) {
            throw e;
        } catch (Exception e) {
            throw new ParsingException(e);
        }
    }
    public static Exception fromForbiddenErrorCode(InputStream ex) throws ParsingException{
        try {
            ObjectMapper objectMapper = ObjectMapperFactory.instance();
            JsonNode rootNode = objectMapper.readTree(ex);

            if(rootNode.getNodeType() != JsonNodeType.OBJECT) {
                throw new ParsingException("Unrecognized JSON (object expected)");
            } else {
                String errorType = rootNode.get("errorType").textValue();

                if(errorType.equals("AlreadyCancelled")) {
                    return toAlreadyCancelledException(rootNode);
                } else if(errorType.equals("CourseAlreadyJoined")) {
                    return toCourseAlreadyJoinedException(rootNode);
                } else if(errorType.equals("CourseAlreadyStarted")) {
                    return toCourseAlreadyStartedException(rootNode);
                } else if(errorType.equals("NotEnoughPlaces")) {
                    return toNotEnoughPlacesException(rootNode);
                } else if(errorType.equals("OutOfDate")) {
                    return toOutOfDateException(rootNode);
                } else if(errorType.equals("WrongUser")) {
                    return toWrongUserException(rootNode);
                } else {
                    throw new ParsingException("Unrecognized error type: " + errorType);
                }
            }
        } catch (ParsingException e) {
            throw e;
        } catch (Exception e) {
            throw new ParsingException(e);
        }
    }
    public static Exception fromGoneErrorCode(InputStream ex) throws ParsingException{
        try {
            ObjectMapper objectMapper = ObjectMapperFactory.instance();
            JsonNode rootNode = objectMapper.readTree(ex);

            if(rootNode.getNodeType() != JsonNodeType.OBJECT) {
                throw new ParsingException("Unrecognized JSON (object expected)");
            } else {
                String errorType = rootNode.get("errorType").textValue();
                throw new ParsingException("Unrecognized error type: " + errorType);
            }
        } catch (ParsingException e) {
            throw e;
        } catch (Exception e) {
            throw new ParsingException(e);
        }
    }


    private static InputValidationException toInputValidationException(JsonNode rootNode) {
        String message = rootNode.get("message").textValue();
        return new InputValidationException(message);
    }

    private static InstanceNotFoundException toInstanceNotFoundException(JsonNode rootNode) {
        String instanceId = rootNode.get("instanceId").textValue();
        String instanceType = rootNode.get("instanceType").textValue();
        return new InstanceNotFoundException(instanceId, instanceType);
    }
    private static ClientAlreadyCancelledException toAlreadyCancelledException(JsonNode rootNode){
        Long inscriptionId = rootNode.get("inscriptionId").longValue();
        return new ClientAlreadyCancelledException(inscriptionId);
    }

    private static ClientCourseAlreadyJoinedException toCourseAlreadyJoinedException(JsonNode rootNode) {
        Long courseId = rootNode.get("courseId").longValue();
        return new ClientCourseAlreadyJoinedException(courseId);
    }

    private static ClientCourseAlreadyStartedException toCourseAlreadyStartedException(JsonNode rootNode) {
        Long courseId = rootNode.get("courseId").longValue();
        return new ClientCourseAlreadyStartedException(courseId);
    }

    private static ClientNotEnoughPlacesException toNotEnoughPlacesException(JsonNode rootNode) {
        Long courseId = rootNode.get("courseId").longValue();
        return new ClientNotEnoughPlacesException(courseId);
    }

    private static ClientOutOfDateException toOutOfDateException(JsonNode rootNode) {
        Long courseId = rootNode.get("courseId").longValue();
        return new ClientOutOfDateException(courseId);
    }

    private static ClientWrongUserException toWrongUserException(JsonNode rootNode) {
        Long inscriptionId = rootNode.get("inscriptionId").longValue();
        return new ClientWrongUserException(inscriptionId);
    }
}
