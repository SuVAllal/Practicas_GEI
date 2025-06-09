package es.udc.ws.app.restservice.dto;

import java.time.LocalDateTime;

public class RestCourseDto {
    private Long courseId;
    private String name;
    private String city;
    private LocalDateTime startDate;
    private float price;
    private short maxPlaces;
    private short freePlaces;

    public RestCourseDto(Long courseId, String name, String city, LocalDateTime startDate, float price, short maxPlaces, short freePlaces) {
        this.courseId = courseId;
        this.name = name;
        this.city = city;
        this.startDate = startDate;
        this.price = price;
        this.maxPlaces = maxPlaces;
        this.freePlaces = freePlaces;
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
        this.startDate = startDate;
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

    public short getFreePlaces() {
        return freePlaces;
    }

    public void setFreePlaces(short freePlaces) {
        this.freePlaces = freePlaces;
    }

    @Override
    public String toString() {
        return "RestCourseDto [" +
                "courseId=" + courseId +
                ", name='" + name + '\'' +
                ", city='" + city + '\'' +
                ", startDate=" + startDate +
                ", price=" + price +
                ", maxPlaces=" + maxPlaces +
                ", freePlaces=" + freePlaces +
                ']';
    }
}
