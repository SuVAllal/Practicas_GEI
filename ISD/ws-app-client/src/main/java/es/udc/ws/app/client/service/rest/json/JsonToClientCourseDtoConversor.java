package es.udc.ws.app.client.service.rest.json;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import com.fasterxml.jackson.databind.node.JsonNodeFactory;
import com.fasterxml.jackson.databind.node.JsonNodeType;
import com.fasterxml.jackson.databind.node.ObjectNode;
import es.udc.ws.app.client.service.dto.ClientCourseDto;
import es.udc.ws.util.json.ObjectMapperFactory;
import es.udc.ws.util.json.exceptions.ParsingException;

import java.io.IOException;
import java.io.InputStream;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

public class JsonToClientCourseDtoConversor {
    public static ObjectNode toObjectNode(ClientCourseDto course) throws IOException {
        ObjectNode courseObject = JsonNodeFactory.instance.objectNode();

        if(course.getCourseId() != null) {
            courseObject.put("courseId", course.getCourseId());
        }

        courseObject.put("name", course.getName()).
                put("city", course.getCity()).
                put("startDate", course.getStartDate().toString()).
                put("price", course.getPrice()).
                put("maxPlaces", course.getMaxPlaces()).
                put("freePlaces", course.getMaxPlaces() - course.getReservedPlaces());

        return courseObject;
    }

    public static ClientCourseDto toClientCourseDto(InputStream jsonCourse) throws ParsingException {
        try {
            ObjectMapper objectMapper = ObjectMapperFactory.instance();
            JsonNode rootNode = objectMapper.readTree(jsonCourse);

            if(rootNode.getNodeType() != JsonNodeType.OBJECT) {
                throw new ParsingException("Unrecognized JSON (object expected)");
            } else {
                return toClientCourseDto(rootNode);
            }
        } catch (ParsingException ex) {
            throw ex;
        } catch (Exception e) {
            throw new ParsingException(e);
        }
    }


    public static List<ClientCourseDto> toClientCourseDtos(InputStream jsonCourses) throws ParsingException {
        try {
            ObjectMapper objectMapper = ObjectMapperFactory.instance();
            JsonNode rootNode = objectMapper.readTree(jsonCourses);

            if(rootNode.getNodeType() != JsonNodeType.ARRAY) {
                throw new ParsingException("Unrecognized JSON (array expected)");
            } else {
                ArrayNode coursesArray = (ArrayNode) rootNode;
                List<ClientCourseDto> courseDtos = new ArrayList<>(coursesArray.size());
                for(JsonNode courseNode : coursesArray) {
                    courseDtos.add(toClientCourseDto(courseNode));
                }

                return courseDtos;
            }
        } catch(ParsingException ex) {
            throw ex;
        } catch (Exception e) {
            throw new ParsingException(e);
        }
    }


    private static ClientCourseDto toClientCourseDto(JsonNode courseNode) throws ParsingException {
        if(courseNode.getNodeType() != JsonNodeType.OBJECT) {
            throw new ParsingException("Unrecognized JSON (object expected)");
        } else {
            ObjectNode courseObject = (ObjectNode) courseNode;

            // Verificar si el nodo courseId existe y no es nulo
            JsonNode courseIdNode = courseObject.get("courseId");
            Long courseId = (courseIdNode != null) ? courseIdNode.longValue() : null;

            // Verificar si el nodo name existe y no es nulo
            String name = (courseObject.has("name") && !courseObject.get("name").isNull())
                    ? courseObject.get("name").textValue().trim()
                    : null;

            // Verificar si el nodo city existe y no es nulo
            String city = (courseObject.has("city") && !courseObject.get("city").isNull())
                    ? courseObject.get("city").textValue().trim()
                    : null;

            // Verificar si el nodo startDate existe y no es nulo
            LocalDateTime startDate = LocalDateTime.parse(courseObject.get("startDate").textValue());

            // Verificar si el nodo price existe y no es nulo
            float price = courseObject.get("price").floatValue();

            // Verificar si el nodo maxPlaces existe y no es nulo
            short maxPlaces = courseObject.get("maxPlaces").shortValue();

            // Verificar si el nodo freePlaces existe y no es nulo
            short freePlaces = courseObject.get("freePlaces").shortValue();
            short reservedPlaces = (short) (maxPlaces - freePlaces);

            // Crear y devolver el objeto ClientCourseDto
            return new ClientCourseDto(courseId, name, city, startDate, price, maxPlaces, reservedPlaces);
        }
    }
}


