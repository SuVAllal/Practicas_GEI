namespace java es.udc.ws.courses.thrift

struct ThriftCourseDto {
    1: i64 courseId
    2: string name
    3: string city
    4: string startDate
    5: double price
    6: i16 maxPlaces
    7: i16 freePlaces
}

struct ThriftInscriptionDto {
    1: i64 inscriptionId
    2: i64 courseId
    3: string userEmail
    4: string creditCardNumber
    5: string inscriptionDate
    6: string cancelDate
}

exception ThriftInputValidationException {
    1: string message
}

exception ThriftInstanceNotFoundException {
    1: string instanceId
    2: string instanceType
}

exception ThriftNotEnoughPlacesException {
    1: string message
}

exception ThriftCourseAlreadyJoinedException {
    1: string message
}

exception ThriftCourseAlreadyStartedException {
    1: string message
}

service ThriftCourseService{
    ThriftCourseDto addCourse(1: ThriftCourseDto course) throws (ThriftInputValidationException e)
    list<ThriftCourseDto> findCourses(1: string city) throws (ThriftInputValidationException e)
    ThriftCourseDto findCourse(1: i64 courseId) throws (1: ThriftInstanceNotFoundException e, 2: ThriftInputValidationException ee)
    ThriftInscriptionDto joinCourse(1: string userEmail, 2: i64 courseId, 3: string creditCardNumber) throws (1: ThriftInputValidationException e, 2: ThriftInstanceNotFoundException ee, 3: ThriftNotEnoughPlacesException eee, 4: ThriftCourseAlreadyJoinedException eeee, 5: ThriftCourseAlreadyStartedException eeeee)

}