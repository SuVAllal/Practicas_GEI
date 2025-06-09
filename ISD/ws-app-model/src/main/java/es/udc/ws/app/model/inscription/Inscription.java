package es.udc.ws.app.model.inscription;

import java.time.LocalDateTime;
import java.util.Objects;

public class Inscription {
    private Long inscriptionId; // identificador de la inscripcion
    private Long courseId; // identificador del curso
    private String userEmail; // email del usuario
    private String creditCardNumber; // tarjeta de credito
    private LocalDateTime inscriptionDate; // fecha de la inscripcion
    private LocalDateTime cancelDate; // fecha de cancelacion de la inscripcion

    // Constructores de la clase Inscription:
    public Inscription(Long courseId, String userEmail, String creditCardNumber, LocalDateTime inscriptionDate) {
        this.courseId = courseId;
        this.userEmail = userEmail;
        this.creditCardNumber = creditCardNumber;
        this.inscriptionDate = (inscriptionDate != null) ? inscriptionDate.withNano(0) : null;
        this.cancelDate = null;
    }

    public Inscription(Long inscriptionId, Long courseId, String userEmail, String creditCardNumber, LocalDateTime inscriptionDate) {
        this(courseId, userEmail, creditCardNumber, inscriptionDate);
        this.inscriptionId = inscriptionId;
    }

    public Inscription(Long inscriptionId, Long courseId, String userEmail, String creditCardNumber, LocalDateTime inscriptionDate, LocalDateTime cancelDate) {
        this(inscriptionId, courseId, userEmail, creditCardNumber, inscriptionDate);
        this.cancelDate = (cancelDate != null) ? cancelDate.withNano(0) : null;
    }

    // Getters y Setters de la clase Inscription
    public Long getInscriptionId() {
        return inscriptionId;
    }

    public void setInscriptionId(Long inscriptionId) {
        this.inscriptionId = inscriptionId;
    }

    public Long getCourseId() {
        return courseId;
    }

    public void setCourseId(Long courseId) {
        this.courseId = courseId;
    }

    public String getUserEmail() {
        return userEmail;
    }

    public void setUserEmail(String userEmail) {
        this.userEmail = userEmail;
    }

    public String getCreditCardNumber() {
        return creditCardNumber;
    }

    public void setCreditCardNumber(String creditCardNumber) {
        this.creditCardNumber = creditCardNumber;
    }

    public LocalDateTime getInscriptionDate() {
        return (inscriptionDate != null) ? inscriptionDate.withNano(0) : null;
    }

    public void setInscriptionDate(LocalDateTime inscriptionDate) {
        this.inscriptionDate = (inscriptionDate != null) ? inscriptionDate.withNano(0) : null;
    }

    public LocalDateTime getCancelDate() {
        return (cancelDate != null) ? cancelDate.withNano(0) : null;
    }

    public void setCancelDate(LocalDateTime cancelDate) {
        this.cancelDate = (cancelDate != null) ? cancelDate.withNano(0) : null;
    }

    // Equals de Inscription
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Inscription that = (Inscription) o;
        return Objects.equals(inscriptionId, that.inscriptionId) && Objects.equals(courseId, that.courseId) &&
                Objects.equals(userEmail, that.userEmail) && Objects.equals(creditCardNumber, that.creditCardNumber) &&
                Objects.equals(inscriptionDate, that.inscriptionDate) && Objects.equals(cancelDate, that.cancelDate);
    }

    // Hashcode de Inscription
    @Override
    public int hashCode() {
        return Objects.hash(inscriptionId, courseId, userEmail, creditCardNumber, inscriptionDate, cancelDate);
    }
}
