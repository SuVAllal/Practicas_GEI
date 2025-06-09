package es.udc.ws.app.restservice.json;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import com.fasterxml.jackson.databind.node.JsonNodeFactory;
import com.fasterxml.jackson.databind.node.JsonNodeType;
import com.fasterxml.jackson.databind.node.ObjectNode;
import es.udc.ws.app.restservice.dto.RestCourseDto;
import es.udc.ws.util.json.ObjectMapperFactory;
import es.udc.ws.util.json.exceptions.ParsingException;

import java.io.InputStream;
import java.time.LocalDateTime;
import java.util.List;

public class JsonToRestCourseDtoConversor {
    public static ObjectNode toObjectNode(RestCourseDto course){
        ObjectNode courseObjectNode = JsonNodeFactory.instance.objectNode();
        courseObjectNode.put("courseId", course.getCourseId()).put("name", course.getName()).
                put("city", course.getCity()).put("startDate", course.getStartDate().toString()).
                put("price", course.getPrice()).put("maxPlaces", course.getMaxPlaces()).
                put("freePlaces",course.getFreePlaces());
        return courseObjectNode;
    }
    public static ArrayNode toArrayNode(List<RestCourseDto> listCourse){
        ArrayNode courseNode = JsonNodeFactory.instance.arrayNode();
        for (RestCourseDto courseDto : listCourse){
            ObjectNode courseObject = toObjectNode(courseDto);
            courseNode.add(courseObject);
        }
        return courseNode;
    }
    public static RestCourseDto toRestCourseDto(InputStream jsonCourse) throws ParsingException{
        try {
            ObjectMapper objectMapper = ObjectMapperFactory.instance();
            JsonNode jsonNode = objectMapper.readTree(jsonCourse);
            if (jsonNode.getNodeType() != JsonNodeType.OBJECT){
                throw new ParsingException("Object expected");
            } else {
                ObjectNode courseObjectNode = (ObjectNode) jsonNode;
                JsonNode courseNode = courseObjectNode.get("courseId");

                Long courseId = (courseNode != null) ? courseNode.longValue() : null;
                String name = courseObjectNode.get("name").textValue().trim();
                String city = courseObjectNode.get("city").textValue().trim();
                LocalDateTime startDate = LocalDateTime.parse(courseObjectNode.get("startDate").textValue().trim());
                float price = courseObjectNode.get("price").floatValue();
                short maxPlaces = courseObjectNode.get("maxPlaces").shortValue();
                short freePlaces = courseObjectNode.get("freePlaces").shortValue();
                return new RestCourseDto(courseId,name,city,startDate,price,maxPlaces,freePlaces);
            }
        } catch (ParsingException e){
            throw e;
        } catch (Exception e){
            throw new ParsingException(e);
        }
    }
}
