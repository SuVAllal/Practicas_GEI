package es.udc.ws.app.client.service.dto;

import java.time.LocalDateTime;

public class ClientInscriptionDto {
    private Long inscriptionId;
    private Long courseId;
    private String userEmail;
    private String creditCardNumber;
    private LocalDateTime inscriptionDate;
    private LocalDateTime cancelDate;

    public ClientInscriptionDto(Long inscriptionId, Long courseId, String userEmail, String creditCardNumber,
                              LocalDateTime inscriptionDate, LocalDateTime cancelDate) {
        this.inscriptionId = inscriptionId;
        this.courseId = courseId;
        this.userEmail = userEmail;
        this.creditCardNumber = creditCardNumber;
        this.inscriptionDate = inscriptionDate;
        this.cancelDate = cancelDate;
    }

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
        return inscriptionDate;
    }

    public void setInscriptionDate(LocalDateTime inscriptionDate) {
        this.inscriptionDate = inscriptionDate;
    }

    public LocalDateTime getCancelDate() {
        return cancelDate;
    }

    public void setCancelDate(LocalDateTime cancelDate) {
        this.cancelDate = cancelDate;
    }
}
