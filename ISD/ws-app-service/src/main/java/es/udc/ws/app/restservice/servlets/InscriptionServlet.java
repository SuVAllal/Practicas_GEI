package es.udc.ws.app.restservice.servlets;

import es.udc.ws.app.model.courseservice.CourseServiceFactory;
import es.udc.ws.app.model.courseservice.exceptions.*;
import es.udc.ws.app.model.inscription.Inscription;
import es.udc.ws.app.restservice.dto.InscriptionToRestInscriptionDtoConversor;
import es.udc.ws.app.restservice.dto.RestInscriptionDto;
import es.udc.ws.app.restservice.json.AppExceptionToJsonConversor;
import es.udc.ws.app.restservice.json.JsonToRestInscriptionDtoConversor;
import es.udc.ws.util.exceptions.InputValidationException;
import es.udc.ws.util.exceptions.InstanceNotFoundException;
import es.udc.ws.util.servlet.RestHttpServletTemplate;
import es.udc.ws.util.servlet.ServletUtils;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;


import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class InscriptionServlet extends RestHttpServletTemplate {
    @Override
    protected void processGet(HttpServletRequest req, HttpServletResponse resp) throws IOException, InstanceNotFoundException, InputValidationException {

        String userEmail = req.getParameter("userEmail");
        List<Inscription> inscriptionList = CourseServiceFactory.getService().findInscriptions(userEmail);
        List<RestInscriptionDto> listRestinscription = InscriptionToRestInscriptionDtoConversor.toRestInscriptionDtos(inscriptionList);
        ServletUtils.writeServiceResponse(resp, HttpServletResponse.SC_OK, JsonToRestInscriptionDtoConversor.toArrayNode(listRestinscription), null);

    }

    @Override
    protected void processPost(HttpServletRequest req, HttpServletResponse resp) throws IOException, InstanceNotFoundException, InputValidationException {

        String pathInfo = req.getPathInfo();

        if (req.getPathInfo() == null || req.getPathInfo().equals("/")) {
            RestInscriptionDto inscriptionDtoIn = JsonToRestInscriptionDtoConversor.toRestInscriptionDto(req.getInputStream());

            Inscription inscription;

            try {
                inscription = CourseServiceFactory.getService().joinCourse(inscriptionDtoIn.getUserEmail(),
                        inscriptionDtoIn.getCourseId(), inscriptionDtoIn.getCreditCardNumber());

                RestInscriptionDto inscriptionDto = InscriptionToRestInscriptionDtoConversor.toRestInscriptionDto(inscription);
                String inscriptionURL = ServletUtils.normalizePath(req.getRequestURL().toString()) + "/" + inscription.getInscriptionId().toString();
                Map<String, String> headers = new HashMap<>(1);
                headers.put("Location", inscriptionURL);
                ServletUtils.writeServiceResponse(resp, HttpServletResponse.SC_CREATED,
                        JsonToRestInscriptionDtoConversor.toObjectNode(inscriptionDto), headers);

            } catch (NotEnoughPlacesException e) {
                ServletUtils.writeServiceResponse(resp, HttpServletResponse.SC_FORBIDDEN,
                        AppExceptionToJsonConversor.toNotEnoughPlacesException(e), null);
            } catch (CourseAlreadyStartedException e) {
                ServletUtils.writeServiceResponse(resp, HttpServletResponse.SC_FORBIDDEN,
                        AppExceptionToJsonConversor.toCourseAlreadyStartedException(e), null);
            } catch (CourseAlreadyJoinedException e) {
                ServletUtils.writeServiceResponse(resp, HttpServletResponse.SC_FORBIDDEN,
                        AppExceptionToJsonConversor.toCourseAlreadyJoinedException(e), null);
            }

        } else {
            RestInscriptionDto inscriptionDtoIn = JsonToRestInscriptionDtoConversor.toRestInscriptionDto(req.getInputStream());
            String[] partes = pathInfo.split("/");
            Long inscriptionId = Long.parseLong(partes[1]);

            try {
                CourseServiceFactory.getService().cancelInscription(inscriptionId, inscriptionDtoIn.getUserEmail());
                resp.setStatus(HttpServletResponse.SC_NO_CONTENT);
            } catch (AlreadyCancelledException e) {
                ServletUtils.writeServiceResponse(resp, HttpServletResponse.SC_FORBIDDEN, AppExceptionToJsonConversor.toAlreadyCancelledException(e), null);
            } catch (WrongUserException e) {
                ServletUtils.writeServiceResponse(resp, HttpServletResponse.SC_FORBIDDEN, AppExceptionToJsonConversor.toWrongUserException(e), null);

            } catch (OutOfDateException e) {
                ServletUtils.writeServiceResponse(resp, HttpServletResponse.SC_FORBIDDEN, AppExceptionToJsonConversor.toOutOfDateException(e), null);

            }

        }
    }
}
