package es.udc.ws.app.model.inscription;

import java.sql.*;

public class Jdbc3CcSqlInscriptionDao extends AbstractSqlInscriptionDao {
    /*
     * create: dar de alta una inscripcion
     *         En caso de no poder realizar el alta, se lanzaria una RuntimeException
     */
    @Override
    public Inscription create(Connection connection, Inscription inscription) {
        String queryString = "INSERT INTO Inscription"
                + " (courseId, userEmail, creditCardNumber, inscriptionDate, cancelDate)"
                + " VALUES (?, ?, ?, ?, ?)";

        try(PreparedStatement preparedStatement = connection.prepareStatement(
                queryString, Statement.RETURN_GENERATED_KEYS)) {

            int i = 1;
            preparedStatement.setLong(i++, inscription.getCourseId());
            preparedStatement.setString(i++, inscription.getUserEmail());
            preparedStatement.setString(i++, inscription.getCreditCardNumber());
            preparedStatement.setTimestamp(i++, Timestamp.valueOf(inscription.getInscriptionDate()));
            Timestamp date = inscription.getCancelDate() != null ? Timestamp.valueOf(inscription.getCancelDate()) : null;
            preparedStatement.setTimestamp(i++, date);

            preparedStatement.executeUpdate();

            ResultSet resultSet = preparedStatement.getGeneratedKeys();

            if(!resultSet.next()) {
                throw new SQLException(
                        "JDBC driver did not return generated key.");
            }
            Long inscriptionId = resultSet.getLong(1);

            return new Inscription(inscriptionId, inscription.getCourseId(), inscription.getUserEmail(),
                    inscription.getCreditCardNumber(), inscription.getInscriptionDate(), inscription.getCancelDate());

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }
}
