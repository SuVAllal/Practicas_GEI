package es.udc.ws.app.client.service.thrift;

import es.udc.ws.app.client.service.dto.ClientCourseDto;
import es.udc.ws.courses.thrift.ThriftCourseDto;


import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

public class ClientCourseDtoToThriftCourseDtoConversor {
    public static ThriftCourseDto toThriftCourseDto(ClientCourseDto clientCourseDto){
        Long id = clientCourseDto.getCourseId();
        return new ThriftCourseDto(id == null ? -1 : id, clientCourseDto.getName(), clientCourseDto.getCity(),
                clientCourseDto.getStartDate().toString(), Float.valueOf(clientCourseDto.getPrice()).doubleValue(),
                clientCourseDto.getMaxPlaces(), (short) (clientCourseDto.getMaxPlaces() - clientCourseDto.getReservedPlaces()));
    }
    public static ClientCourseDto toClientCourseDto(ThriftCourseDto thriftCourseDto){
        return new ClientCourseDto(thriftCourseDto.getCourseId(), thriftCourseDto.getName(), thriftCourseDto.getCity(),
                LocalDateTime.parse(thriftCourseDto.getStartDate()),Double.valueOf(thriftCourseDto.getPrice()).floatValue()
        , thriftCourseDto.getMaxPlaces(), (short) (thriftCourseDto.getMaxPlaces() - thriftCourseDto.getFreePlaces()));
    }
    public static List<ClientCourseDto> toClientCourseDtos(List<ThriftCourseDto> listThriftCourse){
        List<ClientCourseDto> listClientCourse = new ArrayList<>(listThriftCourse.size());
        for (ThriftCourseDto thriftCourseDto : listThriftCourse){
            listClientCourse.add(ClientCourseDtoToThriftCourseDtoConversor.toClientCourseDto(thriftCourseDto));
        }
        return listClientCourse;
    }
}
