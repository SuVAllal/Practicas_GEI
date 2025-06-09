from view import View, run # se encarga de la interfaz de usuario
from model import Model # se encarga de la logica de datos

from gi.repository import GLib
import threading
import gettext

COUNT = 10 # numero de pacientes por pagina de la lista

_ = gettext.gettext

# Definimos el presenter, quien actua de intermediario entre la vista y el modelo
class Presenter:
    def __init__(self, view: View, model: Model):
        self.view = view
        self.model = model
        self.selected_patient_id = None # no hay ningun paciente seleccionado al iniciar
        self.selected_medication_id = None # no hay ningun medicamento seleccionado al iniciar


    # Metodo que inicia la aplicacion
    def run(self, application_id: str): # application_id identifica la aplicacion
        self.view.set_handler(self) # configuramos Presenter como el controlador de la vista
        run(application_id, on_activate=self.view.on_activate)

    # Metodo que se ejecuta cuando se selecciona un paciente
    def on_patient_selected(self, id: int):
        def on_patient_selected_thread(id: int) -> None:
            try:
                patient = self.model.get_patient(id)  # obtenemos la informacion del paciente desde el modelo
                self.selected_patient_id = patient.id  # guardamos el id del paciente seleccionado
                # Habilitamos los botones
                GLib.idle_add(self.view.set_sensitive, self.view.add_medication, True)

                GLib.idle_add(self.view.set_patient, patient.code, patient.name, patient.surname)
                self.init_list_m()  # cargamos las medicaciones asociadas al paciente
                GLib.idle_add(self.view.stop_spinner, self.view.spinner)
            except Exception as e:  # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                GLib.idle_add(self.view.show_message, "No se ha podido seleccionar a un paciente, inténtelo más tarde")
                GLib.idle_add(self.view.stop_spinner, self.view.spinner)

        self.view.start_spinner(self.view.spinner)
        threading.Thread(target=on_patient_selected_thread, args=(id,)).start()


    # Metodo que se ejecuta cuando se selecciona un medicamento
    def on_medication_selected(self, medication_id: int) -> None:
        def on_medication_selected_thread(medication_id: int) -> None:
            try:
                medication = self.model.get_medication(self.selected_patient_id, medication_id) # obtenemos la informacion del medicamento
                self.selected_medication_id = medication.id # guardamos el id del medicamento seleccionado

                GLib.idle_add(self.view.medication_details_window)
                GLib.idle_add(self.view.set_medication, medication.name, medication.dosage, medication.start_date, medication.treatment_duration)
                GLib.idle_add(self.view.stop_spinner, self.view.spinner)
                self.init_list_posologies() # cargamos las posologias asociadas al medicamento
            except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                GLib.idle_add(self.view.show_message, "No se ha podido seleccionar una medicacion, inténtelo más tarde")
                GLib.idle_add(self.view.stop_spinner, self.view.spinner)

        self.view.start_spinner(self.view.spinner)
        threading.Thread(target=on_medication_selected_thread, args=(medication_id,)).start()


    # Metodo que inicializa la lista de pacientes
    def init_list_p(self):
        def init_list_p_thread() -> None:
            try:
                patients = self.model.get_patients(0, COUNT) # obtiene la lista de pacientes del modelo, comenzando en la pagina 0 y limitando la cantidad a COUNT
                if len(patients) > 0: # si hay pacientes
                    GLib.idle_add(self.view.set_patients, patients) # muestra la vista con la lista de pacientes
                    GLib.idle_add(self.view.set_current_page_p, 0) # establece la pagina actual en 0
                    GLib.idle_add(self.view.stop_spinner, self.view.spinner)
                else: # si NO hay pacientes
                    GLib.idle_add(self.view.set_sensitive_next_p, False) # deshabilita el boton "siguiente"
                    GLib.idle_add(self.view.show_message, "No hay mas pacientes") # muestra mensaje en la vista
                    GLib.idle_add(self.view.stop_spinner, self.view.spinner)
            except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                GLib.idle_add(self.view.show_message, "No se ha podido inicializar la lista de pacientes, inténtelo más tarde")
                GLib.idle_add(self.view.stop_spinner, self.view.spinner)

        self.view.start_spinner(self.view.spinner)
        threading.Thread(target=init_list_p_thread, args=()).start()


    # Metodo que inicializa la lista de medicamentos
    def init_list_m(self) -> None:
        def init_list_m_thread() -> None:
            if self.selected_patient_id is None:
                return  # no se pueden mostrar medicamentos si no hay un paciente seleccionado
            try:
                medications = self.model.get_medications(self.selected_patient_id)  # obtenemos la lista de medicamentos desde el modelo
                if len(medications) > 0: # si hay medicamentos
                    GLib.idle_add(self.view.set_medications, medications) # muestra la vista con la lista de medicamentos
                else: # si NO hay medicamentos
                    GLib.idle_add(self.view.show_message, "No hay mas medicamentos")  # muestra mensaje en la vista
            except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                GLib.idle_add(self.view.show_message, "No se ha podido inicializar la lista de medicamentos, inténtelo más tarde")

        threading.Thread(target=init_list_m_thread, args=()).start()


    # Metodo que inicializa la lista de posologias
    def init_list_posologies(self) -> None:
        def init_list_posologies_thread() -> None:
            if self.selected_medication_id is None:
                return # no se pueden mostrar posologias si no hay un medicamento seleccionado
            try:
                # obtenemos la lista de posologias desde el modelo
                posologies = self.model.get_posologies(self.selected_patient_id, self.selected_medication_id)

                if len(posologies) > 0: # si hay posologias
                    GLib.idle_add(self.view.set_posologies, posologies) # muestra la vista con la lista de posologias
                    #self.view.set_posologies(posologies) # muestra la vista con la lista de posologias
                else: # si NO hay posologias
                    GLib.idle_add(self.view.posologies_data.remove_all) # limpiamos la informacion de las posologias
            except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                GLib.idle_add(self.view.show_message, "No se ha podido inicializar la lista de posologias, inténtelo más tarde")

        threading.Thread(target=init_list_posologies_thread, args=()).start()


    # Metodo que se ejecuta cuando se activa el boton de agregar un medicamento
    def on_add_medication(self, button) -> None:
        if self.selected_patient_id is None:
            return # no se pueden añadir medicamentos sin seleccionar antes a un paciente

        try:
            # abrimos la ventana de la vista para agregar un nuevo medicamento
            self.view.add_medication_window()
        except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
            self.view.show_message("El boton de añadir no funciona, inténtelo más tarde")


    # Metodo que se ejecuta cuando se confirma la accion de crear un medicamento
    def on_accept_add_medication(self, button) -> None:
        def on_accept_add_medication_thread() -> None:
            try:
                # obtenemos el texto ingresado en los campos de entrada
                name = self.view.name_entry_add_m.get_text().strip()
                dosage = self.view.dosage_entry_add_m.get_text().strip()
                start_date = self.view.start_date_entry_add_m.get_text().strip()
                treatment_duration = self.view.treatment_duration_entry_add_m.get_text().strip()

                # Deshabilitamos los botones durante la carga
                GLib.idle_add(self.view.set_sensitive, self.view.accept_button_add_m, False)
                GLib.idle_add(self.view.set_sensitive, self.view.cancel_button_add_m, False)

                # agregamos el medicamento desde el modelo
                self.model.add_medication(name, float(dosage), start_date, int(treatment_duration), self.selected_patient_id)
                # actualizamos la lista de medicamentos en la vista
                self.init_list_m()
                # cerramos la ventana donde se ingresaron los datos una vez se confirma la accion
                GLib.idle_add(self.view.add_medication_dialog.destroy)
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_m)
                GLib.idle_add(self.view.show_message, "Medicamento añadido exitosamente")
            except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                GLib.idle_add(self.view.show_message, "El medicamento no ha sido añadido de forma corrrecta, inténtelo más tarde")
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_m)

                # Volvemos a habilitar los botones para permitir al usuario volver a probar
                GLib.idle_add(self.view.set_sensitive, self.view.accept_button_add_m, True)
                GLib.idle_add(self.view.set_sensitive, self.view.cancel_button_add_m, True)

        self.view.start_spinner(self.view.spinner_m)
        threading.Thread(target=on_accept_add_medication_thread, args=()).start()


    # Metodo que se ejecuta cuando se activa el boton de modificar un medicamento
    def on_modify_medication(self, button) -> None:
        try:
            # abrimos la ventana de la vista para modificar un medicamento
            self.view.modify_medication_window()
        except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
            self.view.show_message("El boton de modificar no funciona, inténtelo más tarde")


    # Metodo que se ejecuta cuando se confirma la accion de modificar un medicamento
    def on_accept_modify_medication(self, button) -> None:
        def on_accept_modify_medication_thread() -> None:
            try:
                # obtenemos el texto ingresado en los campos de entrada
                id = self.selected_medication_id
                name = self.view.name_entry_modify_m.get_text().strip()
                dosage = self.view.dosage_entry_modify_m.get_text().strip()
                start_date = self.view.start_date_entry_modify_m.get_text().strip()
                treatment_duration = self.view.treatment_duration_entry_modify_m.get_text().strip()

                # Deshabilitamos los botones durante la carga
                GLib.idle_add(self.view.set_sensitive, self.view.accept_button_mod_m, False)
                GLib.idle_add(self.view.set_sensitive, self.view.cancel_button_mod_m, False)

                # modificamos el medicamento desde el modelo
                self.model.modify_medication(id, name, float(dosage), start_date, int(treatment_duration), self.selected_patient_id)
                # cerramos la ventana donde se ingresaron los datos una vez se confirma la accion
                GLib.idle_add(self.view.modify_medication_dialog.destroy)
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_modify_m)
                # actualizamos la lista de medicamentos en la vista
                self.init_list_m()
                GLib.idle_add(self.view.show_message, "Medicamento modificado exitosamente")

            except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                GLib.idle_add(self.view.show_message, "El medicamento no ha sido modificado de forma corrrecta, inténtelo más tarde")
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_modify_m)
                # Volvemos a habilitar los botones para permitir al usuario volver a probar
                GLib.idle_add(self.view.set_sensitive, self.view.accept_button_mod_m, True)
                GLib.idle_add(self.view.set_sensitive, self.view.cancel_button_mod_m, True)

        self.view.start_spinner(self.view.spinner_modify_m)
        threading.Thread(target=on_accept_modify_medication_thread, args=()).start()


    # Metodo que se ejecuta cuando se activa el boton de borrar una medicacion
    def on_delete_medication(self, button) -> None:
        def on_on_delete_medication_thread() -> None:
            try:
                # Deshabilitamos los botones durante la carga
                GLib.idle_add(self.view.set_sensitive, self.view.modify_medication, False)
                GLib.idle_add(self.view.set_sensitive, self.view.delete_medication, False)
                GLib.idle_add(self.view.set_sensitive, self.view.add_posology, False)
                GLib.idle_add(self.view.set_sensitive, self.view.modify_posology, False)
                GLib.idle_add(self.view.set_sensitive, self.view.delete_posology, False)

                # borramos el medicamento desde el modelo
                self.model.delete_medication(self.selected_medication_id, self.selected_patient_id)
                # cerramos la ventana donde se mostraba la informacion del medicamento
                GLib.idle_add(self.view.medication_details_dialog.destroy)
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_delete_m)
                # actualizamos la lista de medicamentos de la vista
                self.init_list_m()
                GLib.idle_add(self.view.show_message, "Medicamento eliminado exitosamente")
            except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                GLib.idle_add(self.view.show_message, "El medicamento no ha sido eliminado de forma corrrecta, inténtelo más tarde")
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_delete_m)
                # Volvemos a habilitar los botones para permitir al usuario volver a probar
                GLib.idle_add(self.view.set_sensitive, self.view.modify_medication, False)
                GLib.idle_add(self.view.set_sensitive, self.view.delete_medication, False)
                GLib.idle_add(self.view.set_sensitive, self.view.add_posology, False)
                GLib.idle_add(self.view.set_sensitive, self.view.modify_posology, False)
                GLib.idle_add(self.view.set_sensitive, self.view.delete_posology, False)

        self.view.start_spinner(self.view.spinner_delete_m)
        threading.Thread(target=on_on_delete_medication_thread, args=()).start()

    # Metodo que se ejecuta cuando se confirma la accion de crear una posologia
    def on_add_posology(self, button) -> None:
        if self.selected_medication_id is None:
            return # no se pueden añadir posologias sin seleccionar antes a un medicamento

        try:
            # abrimos la ventana de la vista para agregar una nueva posologia
            self.view.add_posology_window()
        except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
            self.view.show_message("El boton de añadir no funciona, inténtelo más tarde")


    # Metodo que se ejecuta cuando se confirma la accion de crear una posologia
    def on_accept_add_posology(self, button) -> None:
        def on_accept_add_posolog_thread() -> None:
            try:
                # obtenemos el texto ingresado en los campos de entrada
                id = self.selected_patient_id
                medication_id = self.selected_medication_id
                hour = self.view.hour_entry_add_p.get_text().strip()
                minute = self.view.minute_entry_add_p.get_text().strip()

                # Deshabilitamos los botones durante la carga
                GLib.idle_add(self.view.set_sensitive, self.view.accept_button_add_p, False)
                GLib.idle_add(self.view.set_sensitive, self.view.cancel_button_add_p, False)

                # creamos la posologia desde el modelo
                self.model.add_posology(id, medication_id, int(hour), int(minute))
                # cerramos la ventana donde se ingresaron los datos una vez se confirma la accion
                GLib.idle_add(self.view.add_posology_dialog.destroy)
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_add_p)

                # actualizamos la lista de posologias en la vista
                self.init_list_posologies()
                GLib.idle_add(self.view.show_message, "Posología añadida exitosamente")

            except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                GLib.idle_add(self.view.show_message, "La posologia no ha sido añadida de forma corrrecta, inténtelo más tarde")
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_add_p)
                # Volvemos a habilitar los botones para permitir al usuario volver a probar
                GLib.idle_add(self.view.set_sensitive, self.view.accept_button_add_p, True)
                GLib.idle_add(self.view.set_sensitive, self.view.cancel_button_add_p, True)

        self.view.start_spinner(self.view.spinner_add_p)
        threading.Thread(target=on_accept_add_posolog_thread, args=()).start()


    # Metodo que se ejecuta cuando se activa el boton de modificar una posologia
    def on_modify_posology(self, button) -> None:
        try:
            # abrimos la ventana de la vista para modificar una posologia
            self.view.modify_posology_window()
        except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
            self.view.show_message("El boton de modificar no funciona, inténtelo más tarde")


    # Metodo que se ejecuta cuando se confirma la accion de modificar una posologia
    def on_accept_modify_posology(self, button) -> None:
        def on_accept_modify_posology_thread() -> None:
            try:
                # obtenemos el texto ingresado en los campos de entrada
                id = self.selected_patient_id
                medication_id = self.selected_medication_id
                hour = self.view.hour_entry_modify_p.get_text().strip()
                minute = self.view.minute_entry_modify_p.get_text().strip()
                posology_id = self.view.id_entry_modify_p.get_text().strip()

                # Deshabilitamos los botones durante la carga
                GLib.idle_add(self.view.set_sensitive, self.view.accept_button_mod_p, False)
                GLib.idle_add(self.view.set_sensitive, self.view.cancel_button_mod_p, False)

                # modificamos la posologia desde el modelo
                self.model.modify_posology(id, medication_id, int(hour), int(minute), posology_id)
                # cerramos la ventana donde se ingresaron los datos una vez se confirma la accion
                GLib.idle_add(self.view.modify_posology_dialog.destroy)
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_modify_p)
                # actualizamos la lista de posologias en la vista
                self.init_list_posologies()
            except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                GLib.idle_add(self.view.show_message, "La posologia no ha sido modificada de forma corrrecta, inténtelo más tarde")
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_modify_p)
                # Volvemos a habilitar los botones para permitir al usuario volver a probar
                GLib.idle_add(self.view.set_sensitive, self.view.accept_button_mod_p, True)
                GLib.idle_add(self.view.set_sensitive, self.view.cancel_button_mod_p, True)

        self.view.start_spinner(self.view.spinner_modify_p)
        threading.Thread(target=on_accept_modify_posology_thread, args=()).start()


    # Metodo que se ejecuta cuando se activa el boton de borrar una posologia
    def on_delete_posology(self, button) -> None:
        try:
            # abrimos la ventana para borrar una posologia
            self.view.delete_posology_window()
        except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
            self.view.show_message("El boton de eliminar no funciona, inténtelo más tarde")


    # Metodo que se ejecuta cuando se confirma la accion de borrar una posologia
    def on_accept_delete_posology(self, button) -> None:
        def on_accept_delete_posology_thread() -> None:
            try:
                # obtenemos el id de la medicacion a borrar
                id = self.view.id_entry_delete_p.get_text().strip()

                # Deshabilitamos los botones durante la carga
                GLib.idle_add(self.view.set_sensitive, self.view.accept_button_del_p, False)
                GLib.idle_add(self.view.set_sensitive, self.view.cancel_button_del_p, False)

                # borramos la posologia desde el modelo
                self.model.delete_posology(self.selected_patient_id, self.selected_medication_id, id)
                # cerramos la ventana donde se ingreso el id una vez se confirma la accion
                GLib.idle_add(self.view.delete_posology_dialog.destroy)
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_delete_p)
                # actualizamos la lista de posologias en la vista
                self.init_list_posologies()
            except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                GLib.idle_add(self.view.show_message, "La posologia no ha sido eliminada de forma corrrecta, inténtelo más tarde")
                GLib.idle_add(self.view.stop_spinner, self.view.spinner_delete_p)
                # Volvemos a habilitar los botones para permitir al usuario volver a probar
                GLib.idle_add(self.view.set_sensitive, self.view.accept_button_del_p, True)
                GLib.idle_add(self.view.set_sensitive, self.view.cancel_button_del_p, True)

        self.view.start_spinner(self.view.spinner_delete_p)
        threading.Thread(target=on_accept_delete_posology_thread, args=()).start()


    # Metodo que carga una pagina especifica de pacientes en la vista
    def on_load_page(self, idx: int) -> None:
        def on_load_page_thread() -> None:
            if idx >= 0: # si el indice de la pagina es mayor o igual a 0 (su valor inicial)
                try:
                    # obtenemos la lista de pacientes con un desplazamiento para la paginacion (COUNT * idx)
                    patients = self.model.get_patients(COUNT * idx, COUNT)
                    if len(patients) > 0: # si hay pacientes
                        GLib.idle_add(self.view.set_patients, patients) # muestra la vista con la lista de pacientes
                        # habilita los botones de navegacion en la vista
                        GLib.idle_add(self.view.set_sensitive_previous_p, True)
                        GLib.idle_add(self.view.set_sensitive_next_p, True)
                        # establece la pagina actual en la vista usando el indice de la pagina (idx)
                        GLib.idle_add(self.view.set_current_page_p, idx)

                        GLib.idle_add(self.view.stop_spinner, self.view.spinner)
                    else: # si NO hay pacientes
                        GLib.idle_add(self.view.set_sensitive_next_p, False) # deshabilita el boton "siguiente"
                        GLib.idle_add(self.view.show_message, _("No hay mas pacientes"))
                        GLib.idle_add(self.view.stop_spinner, self.view.spinner)
                except Exception as e: # captura cualquier excepcion que ocurra y muestra un mensaje de error en la vista
                    GLib.idle_add(self.view.show_message, "No se han podido cargar los pacientes, inténtelo más tarde")
                    GLib.idle_add(self.view.stop_spinner, self.view.spinner)
            else: # si el indice es menor que 0
                GLib.idle_add(self.view.set_sensitive_next_p, False) # deshabilita el boton "siguiente"
                GLib.idle_add(self.view.show_message, _("No hay mas pacientes"))
                GLib.idle_add(self.view.stop_spinner, self.view.spinner)

        self.view.start_spinner(self.view.spinner)
        threading.Thread(target=on_load_page_thread, args=()).start()

