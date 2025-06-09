package es.udc.ws.app.model.course;

import es.udc.ws.util.configuration.ConfigurationParametersManager;

public class SqlCourseDaoFactory {
    private final static String CLASS_NAME_PARAMETER = "SqlCourseDaoFactory.className";
    private static SqlCourseDao dao = null;

    private SqlCourseDaoFactory(){
    }

    @SuppressWarnings("rawtypes")
    private static SqlCourseDao getInstance() {
        try {
            String daoClassName = ConfigurationParametersManager
                    .getParameter(CLASS_NAME_PARAMETER); // Obtenemos la clase del archivo de conf
            Class daoClass = Class.forName(daoClassName); // Carga la clase
            return (SqlCourseDao) daoClass.getDeclaredConstructor().newInstance();
        } catch(Exception e) {
            throw new RuntimeException(e);
        }
    }

    public synchronized static SqlCourseDao getDao() {
        if(dao == null) {
            dao = getInstance(); // Aseguramos que se instancie solo una vez
        }
        return dao;
    }
}
