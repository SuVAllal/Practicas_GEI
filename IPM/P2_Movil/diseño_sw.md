# Diseño Software
Usamos un modelo MCV (Model Controller View). El sistema está separado en 3 partes:
- **El controller:** es el cerebro del programa, vincula el resto de partes y las organiza.
- **El modelo:** actúa como recaudador de información.
- **El view:** se encarga de crear los objetos e interfaces que se le mostrarán al usuario.


### Diagrama estático:
```mermaid
classDiagram
	class Model{
		__init__()
		get_patients(start_index: int, count: int): List<Patient>
		get_patient(id: int): Patient
		get_medications(id: int): List<Medication>
		get_medication(patient_id: int, medication_id: int): Medication
		get_posologies(id: int, medication_id: int): List<Posology>
        add_medication(name: str, dosage: float, start_date: str, treatment_duration: int, patient_id: int): Medication
        modify_medication(medication_id: int, name: str, dosage: float, start_date: str, treatment_duration: int, patient_id: int)
        delete_medication(medication_id: int, patient_id: int)
        add_posology(patient_id: int, medication_id: int, hour: int, minute: int): List<Posology>
        modify_posology(patient_id: int, medication_id: int, hour: int, minute: int, posology_id: int)
        delete_posology(patient_id: int, medication_id: int, posology_id: int)
	}
	class ModelException {
		__init__()
	}

    class Patient {
        __init__()
    }

    class Medication {
        __init__()
    }

    class Posology {
        __init__()
    }

	Model ..> ModelException : << detects and manages the errors >>
    Model ..> Patient: << represents a patient >>
    Model ..> Medication: << represents a medication >>
    Model ..> Posology: << represents a posology >>

	class Presenter{
        - model: Model
        - view: View

		__init__()
		run()
		on_patient_selected(id: int)
		on_medication_selected(medication_id: int)
        init_list_p()
        init_list_m()
        init_list_posologies()
        on_add_medication()
        on_accept_add_medication()
        on_modify_medication()
        on_accept_modify_medication()
        on_delete_medication()
        on_add_posology()
        on_accept_add_posology()
        on_modify_posology()
        on_accept_modify_posology()
        on_delete_posology()
        on_accept_delete_posology()
        on_load_page(idx: int)
	}
	
    class View {
        - handler: ViewHandler
        - patient_data: List<GPatient>
        - medication_data: List<GMedication>
        - posologies_data: List<GPosology>
        - current_page_p: int
        - app: Application
        - _: string

		run()
        __init__(translations)
        on_activate(app: Gtk.Application)
        set_handler(handler: ViewHandler)
        pantalla_principal()
        create_patient_list()
        _create_patient_row(item: GPatient, user_data: Any): Widget
        _create_patient_form()
        _create_medication_section(main_box: Box)
        _create_medication_row(item: GMedication, user_data: Any): Widget
        medication_details_window()
        _create_medication_form()
        create_posologies_section()
        _create_posology_row(item: GPosology, user_data: Any): Widget
        add_medication_window()
        modify_medication_window()
        add_posology_window()
        modify_posology_window()
        delete_posology_window()
        set_patients(patients: list)
        set_patient(code: string, name: string, surname: string)
        set_medications(medications: list)
        set_medication(name: string, dosage: int, start_date: string, treatment_duration: int)
        set_posologies(posologies: list)
        set_current_page_p(page: int)
        show_message(msg: string)
        set_sensitive_next_p(is_sensitive: bool)
        set_sensitive_previous_p(is_sensitive: bool)
        start_spinner(spinner: Spinner)
        stop_spinner(spinner: Spinner)
        set_sensitive(button: Button, is_sensitive: bool)
	}

	class ViewHandler{
		on_patient_selected(id: int)
		on_medication_selected(medication_id: int)
		on_add_medication()
        on_accept_add_medication()
        on_modify_medication()
        on_accept_modify_medication()
        on_delete_medication()
        on_add_posology()
        on_accept_add_posology()
        on_modify_posology()
        on_accept_modify_posology()
        on_delete_posology()
        on_accept_delete_posology()
        on_load_page(idx: int)
	}
    
	View ..> ViewHandler

	class GPatient{
		int id
		string code
		string name
		string surname
		__init__()
	}

    class GMedication{
        int id
        string name
        float dosage
        string start_date
        int treatment_duration
        int patient_id
        __init__()
    }

    class GPosology{
        int id
        int hour
        int minute
        int medication_id
        __init__()
    }

	View ..> GPatient : << creates a patient object with the information >>
    View ..> GMedication: << creates a medication object with the information >>
    View ..> GPosology: << creates a posology object with de information >>

	Model --> Presenter : sends the information that was asked
	Presenter --> Model : asks for the information the user requires
	Presenter --> View : sends the information so the view can create the interface
```

### Diagramas dinámicos:
**Seleccionar un paciente:**
```mermaid
sequenceDiagram
    participant User
    participant View
    participant Presenter
    participant Model
    User ->> View: Seleccionar paciente
    View ->> Presenter: on_patient_selected(id)
    Presenter ->> Presenter: Crea un thread
    Presenter ->> Model: get_patient(id)
    Model ->> Presenter: Retorna paciente
    Presenter ->> View: set_patient(code, name, surname)
    Presenter ->> Model: get_medications(patient_id)
    Model ->> Presenter: Retorna lista de medicamentos
    Presenter ->> View: set_medications(lista_medicamentos)
    View ->> User: Muestra detalles del paciente y lista de medicamentos
```

**Seleccionar un medicamento:**
```mermaid
sequenceDiagram
    participant User
    participant View
    participant Presenter
    participant Model
    
    User ->> View: Seleccionar medicamento
    View ->> Presenter: on_medication_selected(patiend_id, medication_id)
    Presenter ->> Presenter: Crea un thread
    Presenter ->> Model: get_medication(medication_id, patient_id)
    Model ->> Presenter: Retorna medicamento
    Presenter ->> View: set_medication(name, dosage, start_date, treatment_duration)
    Presenter ->> Model: get_posologies(patient_id, medication_id)
    Model ->> Presenter: Retorna lista de posologías
    Presenter ->> View: set_posologies(lista_posologías)
    View ->> User: Muestra detalles del medicamento y lista de posologías
```

**Añadir medicamentos:**
```mermaid
sequenceDiagram
    participant User
    participant View
    participant Presenter
    participant Model


    User ->> View: Añadir Medicación
    View ->> Presenter: on_add_medication()
    Presenter ->> View: add_medication_window()


    View ->> Presenter: on_accept_add_medication()
    Presenter ->> Presenter: Crea un thread
    Presenter ->> Presenter: Bloquea los botones para evitar más acciones durante la carga
    Presenter ->> Model: add_medication(name, dosage, start_date, treatment_duration)
    Model ->> Presenter: Retorna medicamento nuevo
    Presenter ->> Presenter: Desbloquea los botones (la carga finalizó)
    Presenter ->> View: Actualiza la lista de medicamentos
    View ->> User: Muestra la lista con el nuevo medicamento
```

**Modificar un medicamento:**
```mermaid
sequenceDiagram
    participant User
    participant View
    participant Presenter
    participant Model

    User ->> View: Modificar medicación
    View ->> Presenter: on_modify_medication()
    Presenter ->> View: modify_medication_window()

    View ->> Presenter: on_accept_modify_medication()
    Presenter ->> Presenter: Crea un thread
    Presenter ->> Presenter: Bloquea los botones para evitar más acciones durante la carga
    Presenter ->> Model: modify_medication(name, dosage, start_date, treatment_duration)
    Model ->> Presenter: Retorna medicamento modificado
    Presenter ->> Presenter: Desbloquea los botones (la carga finalizó)
    Presenter ->> View: Actualiza la lista de medicamentos
    View ->> User: Muestra la lista con el medicamento modificado
```

**Eliminar un medicamento:**
```mermaid
sequenceDiagram
    participant User
    participant View
    participant Presenter
    participant Model

    User ->> View: Eliminar medicación
    View ->> Presenter: on_delete_medication()
    Presenter ->> Presenter: Crea un thread
    Presenter ->> Presenter: Bloquea los botones para evitar más acciones durante la carga
    Presenter ->> Model: delete_medication(medication_id, patient_id)
    Model ->> Presenter: Retorna medicación eliminada
    Presenter ->> Presenter: Desbloquea los botones (la carga finalizó)
    Presenter ->> View: Actualiza la lista de medicamentos
    View ->> User: Lista actualizada sin la medicación eliminada
```

**Añadir una posología:**
```mermaid
sequenceDiagram
    participant User
    participant View
    participant Presenter
    participant Model

    User ->> View: Añadir posología
    View ->> Presenter: on_add_posology()
    Presenter ->> View: add_posology_window()

    View ->> Presenter: on_accept_add_posology()
    Presenter ->> Presenter: Crea un thread
    Presenter ->> Presenter: Bloquea los botones para evitar más acciones durante la carga
    Presenter ->> Model: add_posology(patient_id, medication_id, hour, minute)
    Model ->> Presenter: Retorna posología nueva
    Presenter ->> Presenter: Desbloquea los botones (la carga finalizó)
    Presenter ->> View: Actualiza la lista de posologías
    View ->> User: Muestra la lista con la nueva posología
```

**Modificar posologia:**
```mermaid
sequenceDiagram
    participant User
    participant View
    participant Presenter
    participant Model

    User ->> View: Modificar posología
    View ->> Presenter: on_modify_posology()
    Presenter ->> View: modify_posology_window()

    View ->> Presenter: on_accept_modify_posology()
    Presenter ->> Presenter: Crea un thread
    Presenter ->> Presenter: Bloquea los botones para evitar más acciones durante la carga
    Presenter ->> Model: modify_posology(patient_id, medication_id, hour, minute)
    Model ->> Presenter: Retorna posología modificada
    Presenter ->> Presenter: Desbloquea los botones (la carga finalizó)
    Presenter ->> View: Actualiza la lista de posologías
    View ->> User: Muestra la lista con la posología modificada
```

**Eliminar una posologia:**
```mermaid
sequenceDiagram
    participant User
    participant View
    participant Presenter
    participant Model

    User ->> View: Eliminar posología
    View ->> Presenter: on_delete_posology()
    Presenter ->> View: delete_posology_window()

    View ->> Presenter: on_accept_delete_posology()
    Presenter ->> Presenter: Crea un thread
    Presenter ->> Presenter: Bloquea los botones para evitar más acciones durante la carga
    Presenter ->> Model: delete_posology(patient_id, medication_id, posology_id)
    Model ->> Presenter: Retorna posología eliminada
    Presenter ->> Presenter: Desbloquea los botones (la carga finalizó)
    Presenter ->> View: Actualiza la lista de posologías
    View ->> User: Muestra la lista actualizada sin la posología eliminada
```
