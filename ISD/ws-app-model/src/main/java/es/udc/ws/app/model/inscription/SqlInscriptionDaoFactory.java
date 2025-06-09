package es.udc.ws.app.model.inscription;

import es.udc.ws.util.configuration.ConfigurationParametersManager;

public class SqlInscriptionDaoFactory {
    private final static String CLASS_NAME_PARAMETER = "SqlInscriptionDaoFactory.className";
    private static SqlInscriptionDao dao = null;

    private SqlInscriptionDaoFactory() {
    }

    @SuppressWarnings("rawtypes")
    private static SqlInscriptionDao getInstance() {
        try {
            String daoClassName = ConfigurationParametersManager
                    .getParameter(CLASS_NAME_PARAMETER); // Obtenemos la clase del archivo de conf
            Class daoClass = Class.forName(daoClassName); // Carga la clase
            return (SqlInscriptionDao) daoClass.getDeclaredConstructor().newInstance();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

    }

    public synchronized static SqlInscriptionDao getDao() {

        if (dao == null) {
            dao = getInstance(); // Aseguramos que se instancie solo una vez
        }
        return dao;

    }
}
