package es.udc.ws.app.client.service.dto;

import java.time.LocalDateTime;

public class ClientCourseDto {
    private Long courseId;
    private String name;
    private String city;
    private LocalDateTime startDate;
    private float price;
    private short maxPlaces;
    private short reservedPlaces;

    public ClientCourseDto(Long courseId, String name, String city, LocalDateTime startDate, float price,
                           short maxPlaces, short reservedPlaces) {
        this.courseId = courseId;
        this.name = name;
        this.city = city;
        this.startDate = (startDate != null) ? startDate.withNano(0) : null;
        this.price = price;
        this.maxPlaces = maxPlaces;
        this.reservedPlaces = reservedPlaces;
    }

    public Long getCourseId() {
        return courseId;
    }

    public void setCourseId(Long courseId) {
        this.courseId = courseId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public LocalDateTime getStartDate() {
        return startDate;
    }

    public void setStartDate(LocalDateTime startDate) {
        this.startDate = (startDate != null) ? startDate.withNano(0) : null;
    }

    public float getPrice() {
        return price;
    }

    public void setPrice(float price) {
        this.price = price;
    }

    public short getMaxPlaces() {
        return maxPlaces;
    }

    public void setMaxPlaces(short maxPlaces) {
        this.maxPlaces = maxPlaces;
    }

    public short getReservedPlaces() {
        return reservedPlaces;
    }

    public void setReservedPlaces(short reservedPlaces) {
        this.reservedPlaces = reservedPlaces;
    }
}
