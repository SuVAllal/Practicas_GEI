package es.udc.ws.app.client.service;

import es.udc.ws.util.configuration.ConfigurationParametersManager;

public class ClientCourseServiceFactory {
    private final static String CLASS_NAME_PARAMETER = "ClientCourseServiceFactory.className";
    private static ClientCourseService service = null;

    private ClientCourseServiceFactory() {

    }

    @SuppressWarnings("rawtypes")
    private static ClientCourseService getInstance() {
        try {
            String serviceClassName = ConfigurationParametersManager
                    .getParameter(CLASS_NAME_PARAMETER); // Obtenemos la clase del archivo de conf
            Class serviceClass = Class.forName(serviceClassName); // Carga la clase
            return (ClientCourseService) serviceClass.getDeclaredConstructor().newInstance();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public synchronized static ClientCourseService getService() {
        if(service == null) {
            service = getInstance(); // Aseguramos que se instancie solo una vez
        }
        return service;
    }
}
