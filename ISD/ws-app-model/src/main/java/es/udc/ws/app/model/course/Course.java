package es.udc.ws.app.model.course;

import java.time.LocalDateTime;
import java.util.Objects;

public class Course {
    private Long courseId; // id del curso
    private String name; // nombre del curso
    private String city; // ciudad donde se celebra el curso
    private LocalDateTime creationDate; // fecha y hora a la que se dio de alta el curso
    private LocalDateTime startDate; // fecha y hora de inicio del curso
    private float price; // precio del curso
    private short maxPlaces; // numero maximo de personas que pueden inscribirse al curso
    private short freePlaces; // numero de plazas reservadas para inscribirse al curso

    // Constructores de Course
    public Course(String name, String city, LocalDateTime startDate, float price, short maxPlaces) {
        this.name = name;
        this.city = city;
        this.startDate = (startDate != null) ? startDate.withNano(0) : null;
        this.price = price;
        this.maxPlaces = maxPlaces;
        this.freePlaces = maxPlaces;
    }

    public Course(Long courseId, String name, String city, LocalDateTime startDate, float price, short maxPlaces) {
        this(name, city, startDate, price, maxPlaces);
        this.courseId = courseId;
    }

    public Course(Long courseId, String name, String city, LocalDateTime startDate, float price, short maxPlaces, LocalDateTime creationDate, short freePlaces) {
        this(courseId, name, city, startDate, price, maxPlaces);
        this.freePlaces = freePlaces;
        this.creationDate = (creationDate != null) ? creationDate.withNano(0) : null;
    }

    // Getter y Setter de Course
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

    public LocalDateTime getCreationDate() {
        return (creationDate != null) ? creationDate.withNano(0) : null;
    }

    public void setCreationDate(LocalDateTime creationDate) {
        this.creationDate = (creationDate != null) ? creationDate.withNano(0) : null;
    }

    public LocalDateTime getStartDate() {
        return (startDate != null) ? startDate.withNano(0) : null;
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

    public short getfreePlaces() {
        return freePlaces;
    }

    public void setfreePlaces(short freePlaces) {
        this.freePlaces = freePlaces;
    }

    // Equals de Course
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Course course = (Course) o;
        return Objects.equals(courseId, course.courseId) && Objects.equals(name, course.name) &&
                Objects.equals(city, course.city) && Objects.equals(creationDate, course.creationDate) &&
                Objects.equals(startDate, course.startDate) && Objects.equals(price, course.price) &&
                Objects.equals(maxPlaces, course.maxPlaces) && Objects.equals(freePlaces, course.freePlaces);
    }

    // Hashcode de Course
    @Override
    public int hashCode() {
        return Objects.hash(courseId, name, city, creationDate, startDate, price, maxPlaces, freePlaces);
    }
}
