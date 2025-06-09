package es.udc.ws.app.client.ui;

import es.udc.ws.app.client.service.ClientCourseService;
import es.udc.ws.app.client.service.ClientCourseServiceFactory;
import es.udc.ws.app.client.service.dto.ClientCourseDto;
import es.udc.ws.app.client.service.dto.ClientInscriptionDto;
import es.udc.ws.app.client.service.exceptions.ClientAlreadyCancelledException;
import es.udc.ws.app.client.service.exceptions.ClientNotEnoughPlacesException;
import es.udc.ws.app.client.service.exceptions.ClientOutOfDateException;
import es.udc.ws.app.client.service.exceptions.ClientWrongUserException;
import es.udc.ws.util.exceptions.InputValidationException;
import es.udc.ws.util.exceptions.InstanceNotFoundException;

import java.time.LocalDateTime;
import java.util.List;

public class AppServiceClient {
    public static void main(String[] args) {
        if (args.length == 0){
            printUsageAndExit();
        }

        ClientCourseService clientCourseService =
                ClientCourseServiceFactory.getService();

        if ("-addCourse".equalsIgnoreCase(args[0])){
            validateArgs(args,6, new int[] {4, 5});

            // [add] CourseServiceClient -addCourse <name> <city> <startDate> <price> <maxPlaces>

            try {
                Long courseId = clientCourseService.addCourse(new ClientCourseDto(null, args[1], args[2],
                        LocalDateTime.parse(args[3]), Float.parseFloat(args[4]),Short.parseShort(args[5]), (short) 0));
                System.out.println("Course " + courseId + " created succesfully");
            } catch (Exception ex){
                ex.printStackTrace(System.err);
            }
        } else if ("-findCourses".equalsIgnoreCase(args[0])) {
            validateArgs(args,2, new int[] {});

            // [findByCity] CourseServiceClient -findCourses <city>

            try {
                List<ClientCourseDto> listCourses = clientCourseService.findCourses(args[1]);
                for (ClientCourseDto course : listCourses){
                    System.out.println("Course ID: " + course.getCourseId() + " Name: " + course.getName() +
                            " City: " + course.getCity() + " Start Date: " + course.getStartDate() +
                            " Price: " + course.getPrice() + " Max Places: " + course.getMaxPlaces() +
                            " Reserved Places: " + course.getReservedPlaces() + "\n");
                }
            } catch (InputValidationException ex){
                ex.printStackTrace(System.err);
            }
        } else if("-findCourse".equalsIgnoreCase(args[0])) {
            validateArgs(args, 2, new int[]{1});

            // [findById] CourseServiceClient -findCourse <courseId>

            try {
                ClientCourseDto course = clientCourseService.findCourse(Long.parseLong(args[1]));

                System.out.println("Course ID: " + course.getCourseId() + " Name: " + course.getName() +
                        " City: " + course.getCity() + " Start Date: " + course.getStartDate() +
                        " Price: " + course.getPrice() + " Max Places: " + course.getMaxPlaces() +
                        " Reserved Places: " + course.getReservedPlaces() + "\n");
            } catch (InputValidationException | InstanceNotFoundException e) {
                e.printStackTrace(System.err);
            }
        } else if("-joinCourse".equalsIgnoreCase(args[0])) {
            validateArgs(args, 4, new int[] {2});

            // [joinCourse] CourseServiceClient -joinCourse <userEmail> <courseId> <creditCardNumber>

            Long inscriptionId;
            try {
                inscriptionId = clientCourseService.joinCourse(args[1],
                        Long.parseLong(args[2]), args[3]);

                System.out.println("Successfully enrolled in Course " + args[2] +
                        "with inscription number " + inscriptionId);
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
        }else if ("-cancelInscription".equalsIgnoreCase(args[0])) {
            validateArgs(args, 3, new int[]{1});

            // [cancelInscription] CurseServiceClient -cancelInscription <inscriptionId> <email>

            try {
                clientCourseService.cancelInscription(Long.parseLong(args[1]), args[2]);
                System.out.println("Inscripción cancelada exitosamente.");
            } catch (InputValidationException | InstanceNotFoundException | ClientAlreadyCancelledException |
                     ClientOutOfDateException | ClientWrongUserException e) {
                e.printStackTrace(System.err);
            }catch (Exception e) {
                throw new RuntimeException(e);
            }
        } else if ("-findInscriptions".equalsIgnoreCase(args[0])) {
            validateArgs(args, 2, new int[] {});

            // [findByUserEmail] CourseServiceClient -findInscriptions <userEmail>

            try {
                List<ClientInscriptionDto> inscriptions = clientCourseService.findInscriptions(args[1]);
                for (ClientInscriptionDto inscription : inscriptions) {
                    System.out.println("Inscripción ID: " + inscription.getInscriptionId() +
                            " Curso ID: " + inscription.getCourseId() +
                            " Usuario: " + inscription.getUserEmail() +
                            " Credit Card: " + inscription.getCreditCardNumber() +
                            " Fecha Inscripción: " + inscription.getInscriptionDate() +
                            " Cancel Date: " + inscription.getCancelDate());
                }
            } catch (InputValidationException e) {
                e.printStackTrace(System.err);
            }catch (Exception exception) {
                throw new RuntimeException(exception);
            }
        }


    }

    private static void validateArgs(String[] args, int expectedArgs,
                                     int[] numericArguments) {
        if(expectedArgs != args.length) {
            printUsageAndExit();
        }
        for(int i = 0 ; i< numericArguments.length ; i++) {
            int position = numericArguments[i];
            try {
                Double.parseDouble(args[position]);
            } catch(NumberFormatException n) {
                printUsageAndExit();
            }
        }
    }

    private static void printUsageAndExit() {
        printUsage();
        System.exit(-1);
    }

    private static void printUsage() {
        System.err.println("Usage:\n" +
                "    [add] CourseServiceClient -addCourse <name> <city> <startDate> <price> <maxPlaces>\n" +
                "    [findByCity] CourseServiceClient -findCourses <city>\n" +
                "    [findById] CourseServiceClient -findCourse <courseId>\n" +
                "    [joinCourse] CourseServiceClient -joinCourse <userEmail> <courseId> <creditCardNumber>\n" +
                "    [cancelInscription] CurseServiceClient -cancelInscription <inscriptionId> <email>\n"+
                "    [findByUserEmail] CourseServiceClient -findInscriptions <userEmail>\n");
    }
}