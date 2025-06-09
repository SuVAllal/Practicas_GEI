package es.udc.ws.app.model.courseservice;

import es.udc.ws.util.configuration.ConfigurationParametersManager;

public class CourseServiceFactory {
    private final static String CLASS_NAME_PARAMETER = "CourseServiceFactory.className";
    private static CourseService service = null;

    private CourseServiceFactory() {

    }

    @SuppressWarnings("rawtypes")
    private static CourseService getInstance() {
        try {
            String serviceClassName = ConfigurationParametersManager
                    .getParameter(CLASS_NAME_PARAMETER); // Obtenemos la clase del archivo de conf
            Class serviceClass = Class.forName(serviceClassName); // Carga la clase
            return (CourseService) serviceClass.getDeclaredConstructor().newInstance();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public synchronized static CourseService getService() {
        if(service == null) {
            service = getInstance(); // Aseguramos que se instancie solo una vez
        }
        return service;
    }
}
