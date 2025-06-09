package es.udc.ws.app.model.inscription;

import es.udc.ws.util.exceptions.InstanceNotFoundException;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

public abstract class AbstractSqlInscriptionDao implements SqlInscriptionDao {
    protected AbstractSqlInscriptionDao(){}

    /*
     * findByUser: buscar las inscripciones de un usuario por su email
     *                 En caso de no poder realizar la busqueda, se lanzaria una RuntimeException
     */
    @Override
    public List<Inscription> findByUser(Connection connection, String userEmail) {
        String queryString = "SELECT * FROM Inscription WHERE userEmail = ? ORDER BY inscriptionDate DESC";

        try (PreparedStatement preparedStatement = connection.prepareStatement(queryString)){
            preparedStatement.setString(1, userEmail);

            ResultSet resultSet = preparedStatement.executeQuery();

            List<Inscription> inscriptionList = new ArrayList<>();
            while (resultSet.next()){
                Long inscriptionIdEntry = resultSet.getLong("inscriptionId");
                Long courseIdEntry = resultSet.getLong("courseId");
                String userEmailEntry = resultSet.getString("userEmail");
                String creditCardNumberEntry = resultSet.getString("creditCardNumber");
                LocalDateTime inscriptionDateEntry = resultSet.getTimestamp("inscriptionDate").toLocalDateTime();
                LocalDateTime cancelDateEntry = resultSet.getTimestamp("cancelDate") != null
                        ? resultSet.getTimestamp("cancelDate").toLocalDateTime()
                        : null;
                Inscription inscription = new Inscription(inscriptionIdEntry,courseIdEntry,userEmailEntry,creditCardNumberEntry,
                        inscriptionDateEntry,cancelDateEntry);
                inscriptionList.add(inscription);
            }
            return inscriptionList;

        } catch (SQLException e){
            throw new RuntimeException(e);
        }

    }

    /*
     * findByUserId: buscar una inscripcion por su identificador y el email del usuario
     *                 En caso de no poder realizar la busqueda, se lanzaria una RuntimeException
     */
    @Override
    public Inscription findByUserId(Connection connection, Long inscriptionId) throws InstanceNotFoundException {
        String queryString = "SELECT * FROM Inscription WHERE inscriptionId = ?";

        try(PreparedStatement preparedStatement = connection.prepareStatement(queryString)) {
            preparedStatement.setLong(1,inscriptionId);

            ResultSet resultSet = preparedStatement.executeQuery();
            Inscription inscription = null;
            if (resultSet.next()){
                Long inscriptionIdEntry = resultSet.getLong("inscriptionId");
                Long courseIdEntry = resultSet.getLong("courseId");
                String userEmailEntry = resultSet.getString("userEmail");
                String creditCardNumberEntry = resultSet.getString("creditCardNumber");
                LocalDateTime inscriptionDateEntry = resultSet.getTimestamp("inscriptionDate").toLocalDateTime();
                LocalDateTime cancelDateEntry = resultSet.getTimestamp("cancelDate") != null
                        ? resultSet.getTimestamp("cancelDate").toLocalDateTime()
                        : null;
                inscription = new Inscription(inscriptionIdEntry,courseIdEntry,userEmailEntry,creditCardNumberEntry,inscriptionDateEntry,cancelDateEntry);
            }
            if (inscription == null){
                throw new InstanceNotFoundException(inscriptionId,Inscription.class.getName());
            } else {
                return inscription;
            }
        }catch (SQLException e){
            throw new RuntimeException(e);
        }
    }

    /*
     * update: modificar los parametros de una inscripcion ya creada (actualizar su informacion)
     *         En caso de no encontrar dicha inscripcion, se lanzaria una InstanceNotFoundException
     *         En caso de no poder realizar la busqueda, se lanzaria una RuntimeException
     */
     @Override
    public void update(Connection connection, Inscription inscription) throws InstanceNotFoundException {
        String queryString = "UPDATE Inscription SET courseId = ?, userEmail = ?," +
                " creditCardNumber = ?, inscriptionDate = ?, cancelDate = ? WHERE inscriptionId = ?";

        try(PreparedStatement preparedStatement = connection.prepareStatement(queryString)){
            preparedStatement.setLong(1, inscription.getCourseId());
            preparedStatement.setString(2, inscription.getUserEmail());
            preparedStatement.setString(3, inscription.getCreditCardNumber());
            preparedStatement.setTimestamp(4, Timestamp.valueOf(inscription.getInscriptionDate()));
            Timestamp date = inscription.getCancelDate() != null ? Timestamp.valueOf(inscription.getCancelDate()) : null;
            preparedStatement.setTimestamp(5, date);
            preparedStatement.setLong(6,inscription.getInscriptionId());

            int updated = preparedStatement.executeUpdate();

            if (updated == 0){
                throw new InstanceNotFoundException(inscription.getInscriptionId(),Inscription.class.getName());
            }

        } catch (SQLException e){
            throw new RuntimeException(e);
        }
    }

    /*
     * remove: eliminar una inscripcion de la Base de Datos. Necesario para los tests
     *         En caso de no encontrar dicha inscripcion, se lanzaria una InstanceNotFoundException
     *         En caso de no poder realizar la busqueda, se lanzaria una RuntimeException
     */
    @Override
    public void remove(Connection connection, Long inscriptionId) throws InstanceNotFoundException {
        String queryString = "DELETE FROM Inscription WHERE" + " inscriptionId = ?";

        try (PreparedStatement preparedStatement = connection.prepareStatement(queryString)) {
            int i = 1;
            preparedStatement.setLong(i++, inscriptionId);

            int removedRows = preparedStatement.executeUpdate();

            if(removedRows == 0) {
                throw new InstanceNotFoundException(inscriptionId,
                        Inscription.class.getName());
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }

    }
}
