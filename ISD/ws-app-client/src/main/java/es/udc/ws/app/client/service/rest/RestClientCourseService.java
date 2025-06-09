package es.udc.ws.app.client.service.rest;

import com.fasterxml.jackson.core.util.DefaultPrettyPrinter;
import com.fasterxml.jackson.databind.ObjectMapper;
import es.udc.ws.app.client.service.ClientCourseService;
import es.udc.ws.app.client.service.dto.ClientCourseDto;
import es.udc.ws.app.client.service.dto.ClientInscriptionDto;
import es.udc.ws.app.client.service.exceptions.ClientAlreadyCancelledException;
import es.udc.ws.app.client.service.exceptions.ClientNotEnoughPlacesException;
import es.udc.ws.app.client.service.exceptions.ClientOutOfDateException;
import es.udc.ws.app.client.service.exceptions.ClientWrongUserException;
import es.udc.ws.app.client.service.rest.json.JsonToClientCourseDtoConversor;
import es.udc.ws.app.client.service.rest.json.JsonToClientExceptionConversor;
import es.udc.ws.app.client.service.rest.json.JsonToClientInscriptionDtoConversor;
import es.udc.ws.util.configuration.ConfigurationParametersManager;
import es.udc.ws.util.exceptions.InputValidationException;
import es.udc.ws.util.exceptions.InstanceNotFoundException;
import es.udc.ws.util.json.ObjectMapperFactory;
import org.apache.hc.client5.http.fluent.Form;
import org.apache.hc.client5.http.fluent.Request;
import org.apache.hc.core5.http.ClassicHttpResponse;
import org.apache.hc.core5.http.ContentType;
import org.apache.hc.core5.http.HttpStatus;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.Map;

public class RestClientCourseService implements ClientCourseService {
    private final static String ENDPOINT_ADDRESS_PARAMETER = "RestClientCourseService.endpointAddress";
    private String endpointAddress;
    @Override
    public Long addCourse(ClientCourseDto courseDto) throws InputValidationException {
        try{
            ClassicHttpResponse resp = (ClassicHttpResponse) Request.post(getEndpointAddress() + "courses").
                    bodyStream(toInputStream(courseDto), ContentType.create("application/json")).execute().
                    returnResponse();
            validateStatusCode(HttpStatus.SC_CREATED, resp);
            return JsonToClientCourseDtoConversor.toClientCourseDto(resp.getEntity().getContent()).getCourseId();

        } catch (InputValidationException e){
            throw e;
        } catch (Exception e){
            throw new RuntimeException(e);
        }
    }

    @Override
    public List<ClientCourseDto> findCourses(String city) throws InputValidationException {
        try {
            ClassicHttpResponse response = (ClassicHttpResponse) Request.get(getEndpointAddress() + "courses?city="
                            + URLEncoder.encode(city, "UTF-8")).
                    execute().returnResponse();
            validateStatusCode(HttpStatus.SC_OK, response);
            return JsonToClientCourseDtoConversor.toClientCourseDtos(response.getEntity()
                    .getContent());

        } catch (InputValidationException e) {
            throw e;
        } catch (Exception e){
            throw new RuntimeException(e);
        }
    }

    @Override
    public ClientCourseDto findCourse(Long courseId) throws InstanceNotFoundException, InputValidationException {
        try {
            ClassicHttpResponse resp = (ClassicHttpResponse) Request.get(getEndpointAddress() + "courses/" + courseId).
                    execute().returnResponse();

            validateStatusCode(HttpStatus.SC_OK, resp);

            return JsonToClientCourseDtoConversor.toClientCourseDto(
                    resp.getEntity().getContent());
        } catch (InstanceNotFoundException | InputValidationException e) {
            throw e;
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public Long joinCourse(String userEmail, Long courseId, String creditCardNumber) throws
            InputValidationException, InstanceNotFoundException, ClientNotEnoughPlacesException {

        try {
            String json = String.format(
                    "{\"userEmail\":\"%s\", \"courseId\":%d, \"creditCardNumber\":\"%s\"}",
                    userEmail, courseId, creditCardNumber
            );

            ClassicHttpResponse resp = (ClassicHttpResponse) Request.post(getEndpointAddress() + "inscriptions")
                    .bodyString(json, ContentType.APPLICATION_JSON)
                    .execute()
                    .returnResponse();

            validateStatusCode(HttpStatus.SC_CREATED, resp);

            return JsonToClientInscriptionDtoConversor.toClientInscriptionDto(
                    resp.getEntity().getContent()).getCourseId();
        } catch(InputValidationException | InstanceNotFoundException | ClientNotEnoughPlacesException e) {
            throw e;
        } catch(Exception e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void cancelInscription(Long inscriptionId, String userEmail) throws InputValidationException, InstanceNotFoundException, ClientAlreadyCancelledException, ClientOutOfDateException, ClientWrongUserException {
        try {
            String json = String.format(
                    "{\"userEmail\":\"%s\"}",
                    userEmail
            );

            ClassicHttpResponse resp = (ClassicHttpResponse) Request.post(getEndpointAddress() + "inscriptions/"
                            + URLEncoder.encode(inscriptionId.toString(), StandardCharsets.UTF_8) + "/cancel")
                    .bodyString(json, ContentType.APPLICATION_JSON)
                    .execute()
                    .returnResponse();
            validateStatusCode(HttpStatus.SC_NO_CONTENT, resp);

        } catch(InstanceNotFoundException | InputValidationException | ClientAlreadyCancelledException |
                ClientOutOfDateException | ClientWrongUserException e){
            throw e;
        } catch(Exception e){
            throw new RuntimeException(e);
        }
    }


    @Override
    public List<ClientInscriptionDto> findInscriptions(String userEmail) throws InputValidationException {
        try{
            ClassicHttpResponse respuesta = (ClassicHttpResponse) Request.get(getEndpointAddress() + "inscriptions?userEmail="
                            + URLEncoder.encode(userEmail,"UTF-8")).
                    execute().returnResponse();
            validateStatusCode(HttpStatus.SC_OK, respuesta);
            return JsonToClientInscriptionDtoConversor.toClientInscriptionDtos(respuesta.getEntity().getContent());

        }catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
    private synchronized String getEndpointAddress() {
        if (endpointAddress == null) {
            endpointAddress = ConfigurationParametersManager
                    .getParameter(ENDPOINT_ADDRESS_PARAMETER);
        }
        return endpointAddress;
    }
    private InputStream toInputStream(ClientCourseDto course) {

        try {

            ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
            ObjectMapper objectMapper = ObjectMapperFactory.instance();
            objectMapper.writer(new DefaultPrettyPrinter()).writeValue(outputStream,
                    JsonToClientCourseDtoConversor.toObjectNode(course));

            return new ByteArrayInputStream(outputStream.toByteArray());

        } catch (IOException e) {
            throw new RuntimeException(e);
        }

    }

    private void validateStatusCode(int successCode, ClassicHttpResponse response) throws Exception {

        try {

            int statusCode = response.getCode();

            /* Success? */
            if (statusCode == successCode) {
                return;
            }

            /* Handler error. */
            switch (statusCode) {
                case HttpStatus.SC_NOT_FOUND -> throw JsonToClientExceptionConversor.fromNotFoundErrorCode(
                        response.getEntity().getContent());
                case HttpStatus.SC_BAD_REQUEST -> throw JsonToClientExceptionConversor.fromBadRequestErrorCode(
                        response.getEntity().getContent());
                case HttpStatus.SC_FORBIDDEN -> throw JsonToClientExceptionConversor.fromForbiddenErrorCode(
                        response.getEntity().getContent());
                case HttpStatus.SC_GONE -> throw JsonToClientExceptionConversor.fromGoneErrorCode(
                        response.getEntity().getContent());
                default -> throw new RuntimeException("HTTP error; status code = "
                        + statusCode);
            }

        } catch (IOException e) {
            throw new RuntimeException(e);
        }

    }


}
