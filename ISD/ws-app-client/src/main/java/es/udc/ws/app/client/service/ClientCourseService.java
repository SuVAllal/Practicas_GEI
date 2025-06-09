package es.udc.ws.app.client.service;

import es.udc.ws.app.client.service.dto.ClientCourseDto;
import es.udc.ws.app.client.service.dto.ClientInscriptionDto;
import es.udc.ws.app.client.service.exceptions.ClientAlreadyCancelledException;
import es.udc.ws.app.client.service.exceptions.ClientNotEnoughPlacesException;
import es.udc.ws.app.client.service.exceptions.ClientOutOfDateException;
import es.udc.ws.app.client.service.exceptions.ClientWrongUserException;
import es.udc.ws.util.exceptions.InputValidationException;
import es.udc.ws.util.exceptions.InstanceNotFoundException;

import java.time.LocalDateTime;
import java.util.List;

public interface ClientCourseService {
    public Long addCourse(ClientCourseDto courseDto) throws InputValidationException;
    public List<ClientCourseDto> findCourses(String city) throws InputValidationException;
    public ClientCourseDto findCourse(Long courseId) throws InstanceNotFoundException, InputValidationException;
    public Long joinCourse(String userEmail, Long CourseId, String creditCardNumber)
        throws InputValidationException, InstanceNotFoundException, ClientNotEnoughPlacesException;
    public void cancelInscription(Long inscriptionId, String userEmail) throws InputValidationException,
            InstanceNotFoundException, ClientAlreadyCancelledException, ClientOutOfDateException, ClientWrongUserException;
    public List<ClientInscriptionDto> findInscriptions(String userEmail) throws InputValidationException;
}
