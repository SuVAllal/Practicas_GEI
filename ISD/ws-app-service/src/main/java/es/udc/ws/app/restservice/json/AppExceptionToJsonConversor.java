package es.udc.ws.app.restservice.json;

import com.fasterxml.jackson.databind.node.JsonNodeFactory;
import com.fasterxml.jackson.databind.node.ObjectNode;
import es.udc.ws.app.model.courseservice.exceptions.*;
import es.udc.ws.util.exceptions.InputValidationException;

public class AppExceptionToJsonConversor {
    public static ObjectNode toAlreadyCancelledException(AlreadyCancelledException ex) {

        ObjectNode exceptionObject = JsonNodeFactory.instance.objectNode();

        exceptionObject.put("errorType", "AlreadyCancelled");
        exceptionObject.put("inscriptionId", ex.getInscriptionId());

        return exceptionObject;
    }


    public static ObjectNode toCourseAlreadyJoinedException(CourseAlreadyJoinedException ex) {
        ObjectNode exceptionObject = JsonNodeFactory.instance.objectNode();

        exceptionObject.put("errorType", "CourseAlreadyJoined");
        exceptionObject.put("courseId", ex.getCourseId());

        return exceptionObject;
    }


    public static ObjectNode toCourseAlreadyStartedException(CourseAlreadyStartedException ex) {

        ObjectNode exceptionObject = JsonNodeFactory.instance.objectNode();

        exceptionObject.put("errorType", "CourseAlreadyStarted");
        exceptionObject.put("courseId", ex.getCourseId());

        return exceptionObject;
    }


    public static ObjectNode toNotEnoughPlacesException(NotEnoughPlacesException ex) {
        ObjectNode exceptionObject = JsonNodeFactory.instance.objectNode();

        exceptionObject.put("errorType", "NotEnoughPlaces");
        exceptionObject.put("courseId", ex.getCourseId());

        return exceptionObject;
    }
    public static ObjectNode toOutOfDateException(OutOfDateException ex) {

        ObjectNode exceptionObject = JsonNodeFactory.instance.objectNode();

        exceptionObject.put("errorType", "OutOfDate");
        exceptionObject.put("courseId", ex.getCourseId());

        return exceptionObject;

    }
    public static ObjectNode toWrongUserException(WrongUserException ex) {

        ObjectNode exceptionObject = JsonNodeFactory.instance.objectNode();

        exceptionObject.put("errorType", "WrongUser");
        exceptionObject.put("inscriptionId", ex.getInscriptionId());

        return exceptionObject;
    }
}
