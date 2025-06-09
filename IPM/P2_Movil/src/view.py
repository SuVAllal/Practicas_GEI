from typing import Callable, Protocol, Any

import gi
import gettext

gi.require_version('Gtk', '4.0')
from gi.repository import Gtk, Gio, GObject

_ = gettext.gettext


def run(application_id: str, on_activate: Callable) -> None:
    app = Gtk.Application(application_id=application_id)
    app.connect('activate', on_activate)
    app.run()

# Clase que representa al paciente
class GPatient(GObject.GObject):
    def __init__(self, id: int, code: str, name: str, surname: str):
        super().__init__()
        self.id = id
        self.code = code
        self.name = name
        self.surname = surname


# Clase que representa la medicacion de un paciente
class GMedication(GObject.GObject):
    def __init__(self, id: int, name: str, dosage: float, start_date: str, treatment_duration: int, patient_id: int):
        super().__init__()
        self.id = id
        self.name = name
        self.dosage = dosage
        self.start_date = start_date
        self.treatment_duration = treatment_duration
        self.patient_id = patient_id


# Clase que representa la posologia de un medicamento
class GPosology(GObject.GObject):
    def __init__(self, id: int, hour: int, minute: int, medication_id: int):
        super().__init__()
        self.id = id
        self.hour = hour
        self.minute = minute
        self.medication_id = medication_id


class ViewHandler(Protocol):
    def on_patient_selected(self, id: int) -> None: pass

    def on_medication_selected(self, medication_id: int) -> None: pass

    def on_add_medication(self, button) -> None: pass

    def on_accept_add_medication(self, button) -> None: pass

    def on_modify_medication(self, button) -> None: pass

    def on_accept_modify_medication(self, button) -> None: pass

    def on_delete_medication(self, button) -> None: pass

    def on_add_posology(self, button) -> None: pass

    def on_accept_add_posology(self, button) -> None: pass

    def on_modify_posology(self, button) -> None: pass

    def on_accept_modify_posology(self, button) -> None: pass

    def on_delete_posology(self, button) -> None: pass

    def on_accept_delete_posology(self, button) -> None: pass

    def on_load_page(self, idx: int) -> None: pass


# Clase principal que representa la interfaz grafica de la aplicacion
class View:
    def __init__(self, translations):
        self.handler = None # manejador de eventos
        self.patient_data = Gio.ListStore(item_type=GPatient)  # almacena pacientes
        self.medication_data = Gio.ListStore(item_type=GMedication) # almacena medicaciones
        self.posologies_data = Gio.ListStore(item_type=GPosology) # almacena posologias
        self.current_page_p = 0  # Página actual de pacientes
        _ = gettext.gettext(translations)
        self.app = None


    # Metodo que se ejecuta cuando se activa la aplicacion
    def on_activate(self, app: Gtk.Application) -> None:
        self.app = app # inicializa la aplicacion
        self.pantalla_principal() # crea la pantalla principal


    # Metodo que establece el manejador
    def set_handler(self, handler: ViewHandler) -> None:
        self.handler = handler


    # Crea y configura la ventana principal de la aplicacion
    def pantalla_principal(self) -> None:
        # creamos una ventana principal con titulo y que permite que se expanda el contenido horizontalmente
        self.window = win = Gtk.ApplicationWindow(
            title=_("Lista de Pacientes"), hexpand=True)
        self.app.add_window(win)  # añade la ventana a la aplicación
        win.connect("destroy", lambda win: win.close())
        win.set_default_size(1080, 720)  # tamaño por defecto de la ventana

        # Creamos spinner de carga
        self.spinner = Gtk.Spinner()
        self.spinner.stop()

        # Creamos la lista de pacientes
        self.create_patient_list()

        # Caja para la vista del paciente
        patient_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=8)
        patient_box.append(self.patient_list) # en la vista de paciente añadimos la lista

        # Creamos un grid con separadores para organizar el contenido
        grid = Gtk.Grid(column_spacing=10, row_spacing=10, margin_start=30, margin_top=10, margin_bottom=30, margin_end=30)
        separator = Gtk.Separator(orientation=Gtk.Orientation.VERTICAL)

        # Informacion del paciente
        self._create_patient_form()

        # Caja para la vista de las medicaciones
        medications_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=8)
        self._create_medication_section(medications_box) # en la vista de medicaciones añadimos la lista

        # Organizamos los elementos
        grid.attach(patient_box, 0, 0, 1, 1)
        grid.attach(separator, 1, 0, 1, 1)
        # Seccion con la informacion concreta del paciente:
        grid.attach(self.patient_form, 2, 0, 1, 1)
        grid.attach(medications_box, 3, 0, 1, 1)

        # Caja organizadora
        main_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=8, margin_top = 20)
        main_box.append(self.spinner)
        main_box.append(grid)

        win.set_child(main_box) # establecemos el grid como el contenido principal de la ventana
        win.present() # hacemos la ventana visible


    # Metodo que crea y organiza una lista de pacientes
    def create_patient_list(self) -> None:
        title_label = Gtk.Label(label=_("Pacientes"), halign=Gtk.Align.START, margin_top=30, margin_start=30)
        title_label.add_css_class("title-2")

        # Listbox de Pacientes
        self.patient_listbox = Gtk.ListBox(hexpand=True, vexpand=True, margin_start=30, margin_top=10, margin_bottom=10,
                                           margin_end=10)
        self.patient_listbox.set_selection_mode(Gtk.SelectionMode.SINGLE)
        self.patient_listbox.bind_model(self.patient_data, self._create_patient_row, None)
        self.patient_listbox.connect("row-activated", lambda _, row: self.handler.on_patient_selected(
            self.patient_data[row.get_index()].id))

        # Habilita el scroll en el ListBox
        patient_scrolledwindow = Gtk.ScrolledWindow()
        patient_scrolledwindow.set_child(self.patient_listbox)

        # Botones de paginación
        self.prev_button_p = Gtk.Button.new_from_icon_name("go-previous-symbolic")
        self.next_button_p = Gtk.Button.new_from_icon_name("go-next-symbolic")
        self.prev_button_p.connect("clicked", lambda _: self.handler.on_load_page(self.current_page_p - 1))
        self.next_button_p.connect("clicked", lambda _: self.handler.on_load_page(self.current_page_p + 1))
        self.page_label_p = Gtk.Label(label=f"{self.current_page_p + 1}", margin_start=8, margin_end=8)

        pagination_box_p = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, halign=Gtk.Align.CENTER, homogeneous=False,
                                   spacing=10, margin_start=10, margin_top=10, margin_bottom=30, margin_end=10)
        pagination_box_p.append(self.prev_button_p)
        pagination_box_p.append(self.page_label_p)
        pagination_box_p.append(self.next_button_p)

        # Una vez construida la lista, la inicializamos llamando al presenter para obtener los datos
        self.handler.init_list_p()

        # Caja vertical para la lista
        self.patient_list = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=8)

        # Organizamos los elementos
        self.patient_list.append(title_label)
        self.patient_list.append(patient_scrolledwindow)
        self.patient_list.append(pagination_box_p)


    # Metodo que genera una fila de la lista de pacientes
    def _create_patient_row(self, item: GPatient, user_data: Any) -> Gtk.Widget:
        box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,
                      margin_start=4, margin_end=4, margin_top=10, margin_bottom=10)
        box.append(Gtk.Label(label=f"{item.name} {item.surname}", hexpand=True, halign=Gtk.Align.START))
        return box


    # Metodo que muestra la informacion de un paciente
    def _create_patient_form(self):
        self.patient_form = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, halign=Gtk.Align.START, spacing=8, margin_start =10, margin_top=30, margin_bottom=50, margin_end=50)
        title_label = Gtk.Label(label=_("Paciente"), halign=Gtk.Align.START)
        title_label.add_css_class("title-2")
        self.patient_form.append(title_label)

        # Campos donde se mostrara la informacion
        self.code_entry = Gtk.Entry()
        self.name_entry = Gtk.Entry()
        self.surname_entry = Gtk.Entry()

        # Mostramos la informacion
        self.patient_form.append(Gtk.Label(label=_("Código"), halign=Gtk.Align.START, margin_top=8))
        self.patient_form.append(self.code_entry)
        self.patient_form.append(Gtk.Label(label=_("Nombre"), halign=Gtk.Align.START, margin_top=8))
        self.patient_form.append(self.name_entry)
        self.patient_form.append(Gtk.Label(label=_("Apellido"), halign=Gtk.Align.START, margin_top=8))
        self.patient_form.append(self.surname_entry)


    # Metodo para crear la seccion de medicamentos en la ventana principal
    def _create_medication_section(self, main_box):
        # Titulo
        title_label = Gtk.Label(label=_("Medicaciones del paciente"), halign=Gtk.Align.START, margin_top=30)
        title_label.add_css_class("title-2")

        # Listbox de Medicamentos
        self.medication_listbox = Gtk.ListBox(hexpand=True, vexpand=True, margin_top=10, margin_bottom=10, margin_end=30)
        self.medication_listbox.set_selection_mode(Gtk.SelectionMode.SINGLE)
        self.medication_listbox.bind_model(self.medication_data, self._create_medication_row, None)
        self.medication_listbox.connect("row-activated", lambda _, row: self.handler.on_medication_selected(
            self.medication_data[row.get_index()].id))

        # Botones de paginación
        self.add_medication = Gtk.Button.new_from_icon_name("list-add-symbolic")
        self.add_medication.set_sensitive(False)
        self.add_medication.connect("clicked", self.handler.on_add_medication)
        button_box_m = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, halign=Gtk.Align.CENTER, homogeneous=False,
                                   spacing=10, margin_start=10, margin_top=10, margin_bottom=30, margin_end=10)
        button_box_m.append(self.add_medication)

        # Habilita el scroll en el ListBox
        medication_scrolledwindow = Gtk.ScrolledWindow()
        medication_scrolledwindow.set_child(self.medication_listbox)

        # Caja vertical para Medicamentos
        medication_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=8)

        # Organizamos los elementos
        medication_box.append(title_label)
        medication_box.append(medication_scrolledwindow)
        medication_box.append(button_box_m)

        # Añadir a la caja principal
        main_box.append(medication_box)


    # Metodo para crear una fila de la lista de medicaciones de un paciente
    def _create_medication_row(self, item: GMedication, user_data: Any) -> Gtk.Widget:
        box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,
                      margin_start=4, margin_end=4, margin_top=2, margin_bottom=2)
        box.append(Gtk.Label(label=item.name, hexpand=True, halign=Gtk.Align.START))
        return box


    # Metodo que crea la ventana con los detalles de un medicamento
    def medication_details_window(self) -> None:
        # Creamos una ventana hija de la ventana principal y bloqueamos la interaccion con la ventana principal hasta que se cierre
        self.medication_details_dialog = Gtk.Dialog(title=_("Detalles de la medicación"), transient_for=self.window, modal=True)
        self.medication_details_dialog.set_default_size(800, 500)
        content_area = self.medication_details_dialog.get_content_area()

        # Creamos spinner de carga
        self.spinner_delete_m = Gtk.Spinner()
        self.spinner_delete_m.stop()

        # Detalles de la medicacion
        self._create_medication_form()

        # Seccion posologias
        self.create_posologies_section()

        # Creamos un grid para organizar los elementos
        grid = Gtk.Grid(column_spacing=10, row_spacing=10, margin_start=10, margin_top=10, margin_bottom=30,
                        margin_end=30)
        separator = Gtk.Separator(orientation=Gtk.Orientation.VERTICAL)
        grid.attach(self.medication_form, 0, 0, 1, 1)
        grid.attach(separator, 1, 0, 1, 1)
        grid.attach(self.posologies_box, 2, 0, 1, 1)

        # Caja organizativa
        main_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=8, margin_top=20)
        main_box.append(self.spinner_delete_m)
        main_box.append(grid)

        # Añadimos el grid al contenido de la ventana
        content_area.append(main_box)
        self.medication_details_dialog.present() # hacemos visible el contenido


    # Metodo para mostrar la informacion de un medicamento
    def _create_medication_form(self):
        self.medication_form = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, halign=Gtk.Align.START, spacing=8)
        title_label = Gtk.Label(label=_("Medicamento"), halign=Gtk.Align.START)
        title_label.add_css_class("title-2")
        self.medication_form.append(title_label)

        # Campos donde se mostrara la informacion
        self.name_entry_m = Gtk.Entry()
        self.dosage_entry = Gtk.Entry()
        self.start_date_entry = Gtk.Entry()
        self.treatment_duration_entry = Gtk.Entry()

        # Mostramos la informacion
        self.medication_form.append(Gtk.Label(label=_("Nombre"), halign=Gtk.Align.START, margin_top=8))
        self.medication_form.append(self.name_entry_m)
        self.medication_form.append(Gtk.Label(label=_("Dosis"), halign=Gtk.Align.START, margin_top=8))
        self.medication_form.append(self.dosage_entry)
        self.medication_form.append(Gtk.Label(label=_("Fecha de Inicio"), halign=Gtk.Align.START, margin_top=8))
        self.medication_form.append(self.start_date_entry)
        self.medication_form.append(Gtk.Label(label=_("Duración del Tratamiento"), halign=Gtk.Align.START, margin_top=8))
        self.medication_form.append(self.treatment_duration_entry)

        # Botones para editar o borrar una medicacion
        self.modify_medication = Gtk.Button.new_from_icon_name("edit-symbolic")
        self.delete_medication = Gtk.Button.new_from_icon_name("user-trash-symbolic")
        self.modify_medication.connect("clicked", self.handler.on_modify_medication)
        self.delete_medication.connect("clicked", self.handler.on_delete_medication)

        self.button_box_m = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, halign=Gtk.Align.CENTER, homogeneous=False,
                               spacing=10, margin_start=10, margin_top=10, margin_bottom=30, margin_end=10)

        self.button_box_m.append(self.modify_medication)
        self.button_box_m.append(self.delete_medication)

        self.medication_form.append(self.button_box_m)


    # Metodo que crea la seccion de posologias en los detalles de una medicacion
    def create_posologies_section(self) -> None:
        title_label = Gtk.Label(label=_("Posologias del paciente"), halign=Gtk.Align.START)
        title_label.add_css_class("title-2")

        # Listbox para las posologias
        self.posologies_listbox = Gtk.ListBox(hexpand=True, vexpand=True)
        self.posologies_listbox.set_selection_mode(Gtk.SelectionMode.SINGLE)
        self.posologies_listbox.bind_model(self.posologies_data, self._create_posology_row, None)

        # Botones para añadir, modificar o borrar una posologia
        self.add_posology = Gtk.Button.new_from_icon_name("list-add-symbolic")
        self.modify_posology = Gtk.Button.new_from_icon_name("edit-symbolic")
        self.delete_posology = Gtk.Button.new_from_icon_name("user-trash-symbolic")
        self.add_posology.connect("clicked", self.handler.on_add_posology)
        self.modify_posology.connect("clicked", self.handler.on_modify_posology)
        self.delete_posology.connect("clicked", self.handler.on_delete_posology)

        button_box_po = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, halign=Gtk.Align.CENTER, homogeneous=False,
                                spacing=10, margin_start=10, margin_top=10, margin_bottom=30, margin_end=10)
        button_box_po.append(self.add_posology)
        button_box_po.append(self.modify_posology)
        button_box_po.append(self.delete_posology)

        # Habilita el scroll en el ListBox
        posology_scrolledwindow = Gtk.ScrolledWindow()
        posology_scrolledwindow.set_child(self.posologies_listbox)

        # Organizamos los elementos
        self.posologies_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=8)
        self.posologies_box.append(title_label)
        self.posologies_box.append(posology_scrolledwindow)
        self.posologies_box.append(button_box_po)


    # Metodo para crear una fila de la lista de posologias
    def _create_posology_row(self, item: GPosology, user_data: Any) -> Gtk.Widget:
        box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,
                      margin_start=4, margin_end=4, margin_top=2, margin_bottom=2)
        box.append(Gtk.Label(label=_(f"Posología {item.id}: {item.hour} horas, {item.minute} minutos"), hexpand=True, halign=Gtk.Align.START))
        return box


    # Metodo para crear la ventana de Añadir Medicamento
    def add_medication_window(self) -> None:
        self.add_medication_dialog = Gtk.Dialog(title=_("Añadir Medicación"), transient_for=self.window, modal=True)
        self.add_medication_dialog.set_default_size(800, 500)
        content_area = self.add_medication_dialog.get_content_area()

        # Campos donde se mostrara la informacion
        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10, margin_top=10, margin_end=10, margin_bottom=10,
                       margin_start=10)

        # Creamos spinner de carga
        self.spinner_m = Gtk.Spinner()
        self.spinner_m.stop()

        self.name_entry_add_m = Gtk.Entry()
        self.name_entry_add_m.set_placeholder_text(_("Ingrese nombre del medicamento"))

        self.dosage_entry_add_m = Gtk.Entry()
        self.dosage_entry_add_m.set_placeholder_text(_("Ingrese dosis del tratamiento"))

        self.start_date_entry_add_m = Gtk.Entry()
        self.start_date_entry_add_m.set_placeholder_text(_("Ingrese fecha de inicio (YYYY-MM-DD)"))

        self.treatment_duration_entry_add_m = Gtk.Entry()
        self.treatment_duration_entry_add_m.set_placeholder_text(_("Ingrese la duración del tratamiento (días)"))

        # Botones aceptar y cancelar
        button_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=10, halign=Gtk.Align.CENTER, margin_top=10)

        self.accept_button_add_m = Gtk.Button(label=_("Aceptar"))
        self.accept_button_add_m.set_sensitive(True)
        self.accept_button_add_m.connect("clicked", self.handler.on_accept_add_medication)
        button_box.append(self.accept_button_add_m)

        self.cancel_button_add_m = Gtk.Button(label=_("Cancelar"))
        self.cancel_button_add_m.set_sensitive(True)
        self.cancel_button_add_m.connect("clicked", lambda _: self.add_medication_dialog.close())
        button_box.append(self.cancel_button_add_m)

        # Organizamos los elementos
        vbox.append(self.name_entry_add_m)
        vbox.append(self.dosage_entry_add_m)
        vbox.append(self.start_date_entry_add_m)
        vbox.append(self.treatment_duration_entry_add_m)
        vbox.append(button_box)
        vbox.append(self.spinner_m)

        # Añadimos los elementos al contenido de la ventana
        content_area.append(vbox)

        self.add_medication_dialog.present() # hacemos el contenido visible


    # Metodo para crear la ventana de Modificar Medicamento
    def modify_medication_window(self) -> None:
        self.modify_medication_dialog = Gtk.Dialog(title=_("Modificar medicacion"), transient_for=self.medication_details_dialog, modal=True)
        self.modify_medication_dialog.set_default_size(800, 500)
        content_area = self.modify_medication_dialog.get_content_area()

        # Campos a modificar
        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10, margin_top=10, margin_end=10, margin_bottom=10,
                       margin_start=10)

        # Creamos spinner de carga
        self.spinner_modify_m = Gtk.Spinner()
        self.spinner_modify_m.stop()

        self.name_entry_modify_m = Gtk.Entry()
        self.name_entry_modify_m.set_placeholder_text(_("Ingrese nombre del medicamento"))

        self.dosage_entry_modify_m = Gtk.Entry()
        self.dosage_entry_modify_m.set_placeholder_text(_("Ingrese dosis del tratamiento"))

        self.start_date_entry_modify_m = Gtk.Entry()
        self.start_date_entry_modify_m.set_placeholder_text(_("Ingrese fecha de inicio (YYYY-MM-DD)"))

        self.treatment_duration_entry_modify_m = Gtk.Entry()
        self.treatment_duration_entry_modify_m.set_placeholder_text(_("Ingrese la duración del tratamiento (días)"))


        # Botones aceptar y cancelar
        button_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=10, halign=Gtk.Align.CENTER, margin_top=10)

        self.accept_button_mod_m = Gtk.Button(label=_("Aceptar"))
        self.accept_button_mod_m.set_sensitive(True)
        self.accept_button_mod_m.connect("clicked", self.handler.on_accept_modify_medication)
        button_box.append(self.accept_button_mod_m)

        self.cancel_button_mod_m = Gtk.Button(label=_("Cancelar"))
        self.cancel_button_mod_m.set_sensitive(True)
        self.cancel_button_mod_m.connect("clicked", lambda _: self.modify_medication_dialog.close())
        button_box.append(self.cancel_button_mod_m)

        # Organizamos los elementos
        vbox.append(self.name_entry_modify_m)
        vbox.append(self.dosage_entry_modify_m)
        vbox.append(self.start_date_entry_modify_m)
        vbox.append(self.treatment_duration_entry_modify_m)
        vbox.append(button_box)
        vbox.append(self.spinner_modify_m)

        # Añadimos los elementos al contenido de la ventana
        content_area.append(vbox)

        self.modify_medication_dialog.present() # hacemos la ventana visible


    # Metodo para crear la ventana de Añadir Posologia
    def add_posology_window(self) -> None:
        self.add_posology_dialog = Gtk.Dialog(title=_("Añadir posologia"), transient_for=self.medication_details_dialog, modal=True)
        self.add_posology_dialog.set_default_size(800, 500)
        content_area = self.add_posology_dialog.get_content_area()

        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10, margin_top=10, margin_end=10, margin_bottom=10,
                       margin_start=10)

        # Creamos spinner de carga
        self.spinner_add_p = Gtk.Spinner()
        self.spinner_add_p.stop()

        # Campos a rellenar
        self.hour_entry_add_p = Gtk.Entry()
        self.hour_entry_add_p.set_placeholder_text(_("Ingrese la hora de la posologia"))

        self.minute_entry_add_p = Gtk.Entry()
        self.minute_entry_add_p.set_placeholder_text(_("Ingrese el minuto de la posologia"))


        # Botones aceptar y cancelar
        button_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=10, halign=Gtk.Align.CENTER, margin_top=10)

        self.accept_button_add_p = Gtk.Button(label=_("Aceptar"))
        self.accept_button_add_p.set_sensitive(True)
        self.accept_button_add_p.connect("clicked", self.handler.on_accept_add_posology)
        button_box.append(self.accept_button_add_p)

        self.cancel_button_add_p = Gtk.Button(label=_("Cancelar"))
        self.cancel_button_add_p.set_sensitive(True)
        self.cancel_button_add_p.connect("clicked", lambda _: self.add_posology_dialog.close())
        button_box.append(self.cancel_button_add_p)

        # Organizamos los elementos
        vbox.append(self.hour_entry_add_p)
        vbox.append(self.minute_entry_add_p)
        vbox.append(button_box)
        vbox.append(self.spinner_add_p)


        # Añadimos los elementos al contenido de la ventana
        content_area.append(vbox)

        self.add_posology_dialog.present() # hacemos el contenido visible


    # Metodo para crear la ventana de Modificar Posologia
    def modify_posology_window(self) -> None:
        self.modify_posology_dialog = Gtk.Dialog(title=_("Modificar posologia"), transient_for=self.medication_details_dialog, modal=True)
        self.modify_posology_dialog.set_default_size(800, 500)
        content_area = self.modify_posology_dialog.get_content_area()

        # Campos a modificar
        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10, margin_top=10, margin_end=10, margin_bottom=10,
                       margin_start=10)

        # Creamos spinner de carga
        self.spinner_modify_p = Gtk.Spinner()
        self.spinner_modify_p.stop()

        self.id_entry_modify_p = Gtk.Entry()
        self.id_entry_modify_p.set_placeholder_text(_("Ingrese el ID de la posologia a modificar"))

        self.hour_entry_modify_p = Gtk.Entry()
        self.hour_entry_modify_p.set_placeholder_text(_("Ingrese la hora de la posologia"))

        self.minute_entry_modify_p = Gtk.Entry()
        self.minute_entry_modify_p.set_placeholder_text(_("Ingrese el minuto de la posologia"))

        # Botones aceptar y cancelar
        button_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=10, halign=Gtk.Align.CENTER, margin_top=10)

        self.accept_button_mod_p = Gtk.Button(label=_("Aceptar"))
        self.accept_button_mod_p.set_sensitive(True)
        self.accept_button_mod_p.connect("clicked", self.handler.on_accept_modify_posology)
        button_box.append(self.accept_button_mod_p)

        self.cancel_button_mod_p = Gtk.Button(label=_("Cancelar"))
        self.cancel_button_mod_p.set_sensitive(True)
        self.cancel_button_mod_p.connect("clicked", lambda _: self.modify_posology_dialog.close())
        button_box.append(self.cancel_button_mod_p)

        # Organizamos los elementos
        vbox.append(self.id_entry_modify_p)
        vbox.append(self.hour_entry_modify_p)
        vbox.append(self.minute_entry_modify_p)
        vbox.append(button_box)
        vbox.append(self.spinner_modify_p)

        # Añadimos los elementos al contenido de la ventana
        content_area.append(vbox)

        self.modify_posology_dialog.present() # hacemos la ventana visible


    # Metodo para crear la ventana de Borrar Posologia
    def delete_posology_window(self) -> None:
        self.delete_posology_dialog = Gtk.Dialog(title=_("Borrar posologia"),
                                                 transient_for=self.medication_details_dialog, modal=True)
        self.delete_posology_dialog.set_default_size(800, 500)
        content_area = self.delete_posology_dialog.get_content_area()

        # Campos a rellenar
        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10, margin_top=10, margin_end=10, margin_bottom=10,
                       margin_start=10)

        # Creamos spinner de carga
        self.spinner_delete_p = Gtk.Spinner()
        self.spinner_delete_p.stop()

        self.id_entry_delete_p = Gtk.Entry()
        self.id_entry_delete_p.set_placeholder_text(_("Ingrese el ID de la posologia a borrar"))

        # Botones aceptar y cancelar
        button_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=10, halign=Gtk.Align.CENTER, margin_top=10)

        self.accept_button_del_p = Gtk.Button(label=_("Aceptar"))
        self.accept_button_del_p.set_sensitive(True)
        self.accept_button_del_p.connect("clicked", self.handler.on_accept_delete_posology)
        button_box.append(self.accept_button_del_p)

        self.cancel_button_del_p = Gtk.Button(label=_("Cancelar"))
        self.cancel_button_del_p.set_sensitive(True)
        self.cancel_button_del_p.connect("clicked", lambda _: self.delete_posology_dialog.close())
        button_box.append(self.cancel_button_del_p)

        # Organizamos los elementos
        vbox.append(self.id_entry_delete_p)
        vbox.append(button_box)
        vbox.append(self.spinner_delete_p)

        # Añadimos los elementos al contenido de la ventana
        content_area.append(vbox)

        self.delete_posology_dialog.present() # hacemos el contenido visible


    # Metodo que actualiza los datos de pacientes en la interfaz
    def set_patients(self, patients: list) -> None:
        self.patient_data.remove_all()
        for patient in patients:
            self.patient_data.append(GPatient(patient.id, patient.code,
                                      patient.name, patient.surname))

    # Metodo que actualiza la informacion de un paciente
    def set_patient(self, code: str, name: str, surname: str) -> None:
        self.code_entry.get_buffer().set_text(code, -1)
        self.name_entry.get_buffer().set_text(name, -1)
        self.surname_entry.get_buffer().set_text(surname, -1)

    # Metodo que actualiza los datos de las medicaciones en la interfaz
    def set_medications(self, medications: list) -> None:
        self.medication_data.remove_all()
        for medication in medications:
            self.medication_data.append(GMedication(medication.id, medication.name, medication.dosage,
                                         medication.start_date, medication.treatment_duration, medication.patient_id))

    # Metodo que actualiza la informacion de un medicamento
    def set_medication(self, name: str, dosage: int, start_date: str, treatment_duration: int) -> None:
        self.name_entry_m.get_buffer().set_text(name, -1)
        self.dosage_entry.get_buffer().set_text(str(dosage), -1)  # Convert dosage to string for the entry
        self.start_date_entry.get_buffer().set_text(start_date, -1)
        self.treatment_duration_entry.get_buffer().set_text(str(treatment_duration),
                                                            -1)  # Convert treatment_duration to string

    # Metodo que actualiza los datos de las posologias en la interfaz
    def set_posologies(self, posologies: list) -> None:
        self.posologies_data.remove_all()
        for posology in posologies:
            self.posologies_data.append(GPosology(posology.id, posology.hour, posology.minute, posology.medication_id))

    # Metodo que actualiza la pagina de la lista de pacientes
    def set_current_page_p(self, page: int) -> None:
        self.current_page_p = page
        self.page_label_p.set_text(f"{self.current_page_p + 1}")
        if self.current_page_p == 0:
            self.set_sensitive_previous_p(False)

    # Metodo que muestra mensajes en la interfaz
    def show_message(self, msg: str) -> None:
        dialog = Gtk.Window(
            title=_("Warning"), modal=True, resizable=False, transient_for=self.window)
        if len(msg) > 200:
            dialog.set_default_size(120, 120)

        box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=16,
                      margin_top=24, margin_bottom=24, margin_start=48, margin_end=48)
        box.append(Gtk.Label(label=msg, wrap=True))

        accept_button = Gtk.Button.new_with_label(_("Accept"))
        accept_button.connect("clicked", lambda _: dialog.close())

        box.append(accept_button)
        dialog.set_child(box)
        dialog.present()

    # Metodo que habilita el boton de siguiente en la lista de pacientes
    def set_sensitive_next_p(self, is_sensitive: bool) -> None:
        self.next_button_p.set_sensitive(is_sensitive)

    # Metodo que habilita el boton de anterior en la lista de pacientes
    def set_sensitive_previous_p(self, is_sensitive: bool) -> None:
        self.prev_button_p.set_sensitive(is_sensitive)

    def start_spinner(self, spinner) -> None:
        spinner.start()

    def stop_spinner(self, spinner) -> None:
        spinner.stop()

    def set_sensitive(self, button: Gtk.Button, is_sensitive: bool) -> None:
        button.set_sensitive(is_sensitive)