package es.udc.ws.app.client.service.thrift;

import es.udc.ws.app.client.service.ClientCourseService;
import es.udc.ws.app.client.service.dto.ClientCourseDto;
import es.udc.ws.app.client.service.dto.ClientInscriptionDto;
import es.udc.ws.app.client.service.exceptions.*;
import es.udc.ws.courses.thrift.*;
import es.udc.ws.util.configuration.ConfigurationParametersManager;
import es.udc.ws.util.exceptions.InputValidationException;
import es.udc.ws.util.exceptions.InstanceNotFoundException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.THttpClient;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import java.util.List;

public class ThriftClientCourseService implements ClientCourseService {
    private final static String ENDPOINT_ADDRESS_PARAMETER = "ThriftClientCourseService.endpointAddress";
    private final static String endpointAddress = ConfigurationParametersManager.getParameter(ENDPOINT_ADDRESS_PARAMETER);
    private ThriftCourseService.Client getClient() {
        try {

            TTransport transport = new THttpClient(endpointAddress);
            TProtocol protocol = new TBinaryProtocol(transport);

            return new ThriftCourseService.Client(protocol);

        } catch (TTransportException e) {
            throw new RuntimeException(e);
        }
    }
    @Override
    public Long addCourse(ClientCourseDto courseDto) throws InputValidationException {
        ThriftCourseService.Client client = getClient();
        try (TTransport transport = client.getInputProtocol().getTransport()) {
            transport.open();
            return client.addCourse(ClientCourseDtoToThriftCourseDtoConversor.toThriftCourseDto(courseDto)).getCourseId();
        } catch (ThriftInputValidationException e) {
            throw new InputValidationException(e.getMessage());
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public List<ClientCourseDto> findCourses(String city) throws InputValidationException {
        ThriftCourseService.Client client = getClient();
        try (TTransport transport = client.getInputProtocol().getTransport()) {
            transport.open();
            return ClientCourseDtoToThriftCourseDtoConversor.toClientCourseDtos(client.findCourses(city));
        } catch (ThriftInputValidationException e) {
            throw new InputValidationException(e.getMessage());
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public ClientCourseDto findCourse(Long courseId) throws InstanceNotFoundException, InputValidationException {
        ThriftCourseService.Client client = getClient();

        try(TTransport transport = client.getInputProtocol().getTransport()) {
            transport.open();
            ThriftCourseDto course = client.findCourse(courseId);
            return ClientCourseDtoToThriftCourseDtoConversor.toClientCourseDto(course);
        } catch (ThriftInstanceNotFoundException e) {
            throw new InstanceNotFoundException(e.getInstanceId(), e.getInstanceType());
        } catch (ThriftInputValidationException e) {
            throw new InputValidationException(e.getMessage());
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public Long joinCourse(String userEmail, Long courseId, String creditCardNumber) throws InputValidationException, InstanceNotFoundException, ClientNotEnoughPlacesException {
        ThriftCourseService.Client client = getClient();

        try(TTransport transport = client.getInputProtocol().getTransport()) {

            transport.open();
            return client.joinCourse(userEmail, courseId, creditCardNumber).getInscriptionId();
        }catch (ThriftInputValidationException e) {
            throw new InputValidationException(e.getMessage());
        } catch (ThriftInstanceNotFoundException e) {
            throw new InstanceNotFoundException(e.getInstanceId(), e.getInstanceType());
        } catch (ThriftNotEnoughPlacesException e) {
            throw new ClientNotEnoughPlacesException(Long.parseLong(e.getMessage()));
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void cancelInscription(Long inscriptionId, String userEmail) throws InputValidationException, InstanceNotFoundException, ClientAlreadyCancelledException, ClientOutOfDateException, ClientWrongUserException {

    }

    @Override
    public List<ClientInscriptionDto> findInscriptions(String userEmail) throws InputValidationException {
        return null;
    }
}
