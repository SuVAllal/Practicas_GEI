package es.udc.ws.app.model.inscription;

import es.udc.ws.util.exceptions.InstanceNotFoundException;

import java.sql.Connection;
import java.util.List;

public interface SqlInscriptionDao {
    public Inscription create(Connection connection, Inscription inscription);
    public List<Inscription> findByUser(Connection connection, String userEmail);
    public Inscription findByUserId(Connection connection, Long inscriptionId) throws InstanceNotFoundException;
    public void update(Connection connection, Inscription inscription) throws InstanceNotFoundException;
    public void remove(Connection connection, Long inscriptionId) throws InstanceNotFoundException;
}
