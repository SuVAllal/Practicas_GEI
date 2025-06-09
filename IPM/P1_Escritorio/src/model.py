import requests # Importamos el modulo requests para realizar solicitudes hacia la API
from time import sleep # Libreria para simular un delay
import gettext

# URL usada en las solicitudes
SERVER_URL = "http://localhost:8000"

_ = gettext.gettext

# Clase utilizada para manejar errores y excepciones
class ModelException(Exception):
    def __init__(self, msg: str):
        super().__init__(msg)


# Clase para representar un paciente
class Patient:
    def __init__(self, data=None):
        if data is not None: # establecemos la informacion del paciente
            for key, value in data.items():
                setattr(self, key, value)


# Clase para representar una medicacion
class Medication:
    def __init__(self, data=None):
        if data is not None: # establecemos la informacion de la medicacion
            for key, value in data.items():
                setattr(self, key, value)


# Clase para representar una posologia
class Posology:
    def __init__(self, data=None):
        if data is not None: # establecemos la informacion de la posologia
            for key, value in data.items():
                setattr(self, key, value)


# Clase Modelo, contiene los metodos para interactuar con la API
class Model:
    def __init__(self):
        pass

    def get_patients(self, start_index: int, count: int) -> list:
        url = f"{SERVER_URL}/patients" # obtenemos la lista de pacientes

        # Añadimos los parametros de consulta a la URL para usar una lista paginada
        if start_index is not None or count is not None:
            url += "?"
            if start_index is not None:
                url += f"start_index={start_index}&"
            if count is not None:
                url += f"count={count}"

        response = requests.get(url) # hacemos una consulta GET

        sleep(2) # simulamos el delay despues de la peticion

        data = response.json() # procesamos la respuesta con JSON

        if response.ok: # si la consulta es exitosa
            patient_list = []
            for item in data: # convertimos el JSON en una lista de pacientes
                patient_list.append(Patient(item))
            return patient_list
        else: # si la consulta no es exitosa mandamos el mensaje de error de la API
            raise ModelException("No se ha podido cargar la lista de pacientes, inténtelo más tarde")


    def get_patient(self, id: int) -> Patient:
        url = f"{SERVER_URL}/patients/{id}" # obtenemos la informacion de un paciente en concreto

        response = requests.get(url) # hacemos una consulta GET

        sleep(2) # simulamos el delay despues de la peticion

        data = response.json() # procesamos la respuesta con JSON

        if response.ok: # si la consulta es exitosa
            return Patient(data) # devolvemos el paciente
        else: # si la consulta no es exitosa mandamos el mensaje de error de la API
            raise ModelException("No se ha podido cobtener la informacion del paciente, inténtelo más tarde")


    def get_medications(self, id: int) -> list:
        url = f"{SERVER_URL}/patients/{id}/medications" # obtenemos la lista de medicaciones de un paciente

        response = requests.get(url) # hacemos una consulta GET

        data = response.json() # procesamos la respuesta con JSON

        if response.ok: # si la consulta es exitosa
            medication_list = []
            for item in data: # convertimos el JSON en una lista de medicaciones
                medication_list.append(Medication(item))
            return medication_list
        else: # si la consulta no es exitosa mandamos el mensaje de error de la API
            raise ModelException("No se ha podido obtener la lista de medicacion del paciente, inténtelo más tarde")


    def get_medication(self, patient_id: int, medication_id: int) -> Medication:
        url = f"{SERVER_URL}/patients/{patient_id}/medications/{medication_id}" # obtenemos la informacion de una medicacion

        response = requests.get(url) # hacemos una consulta GET

        sleep(2) # simulamos el delay despues de la peticion

        data = response.json() # procesamos la respuesta con JSON

        if response.ok: # si la consulta es exitosa
            return Medication(data) # devolvemos la medicacion
        else: # si la consulta no es exitosa mandamos el mensaje de error de la API
            raise ModelException("No se ha podido obtener la informacion de la medicacion, inténtelo más tarde")


    def get_posologies(self, id: int, medication_id: int) -> list:
        url = f"{SERVER_URL}/patients/{id}/medications/{medication_id}/posologies" # obtenemos las posologias de una medicacion

        response = requests.get(url) # hacemos una consulta GET

        data = response.json() # procesamos la respuesta con JSON

        if response.ok: # si la consulta es exitosa
            posologies_list = []
            for item in data: # convertimos el JSON en una lista de medicaciones
                posologies_list.append(Posology(item))
            return posologies_list
        else: # si la consulta no es exitosa mandamos el mensaje de error de la API
            raise ModelException("No se ha podido obtener la lista de medicacion del posologias, inténtelo más tarde")



    def add_medication(self, name: str, dosage: float, start_date: str, treatment_duration: int, patient_id: int) -> Medication:
        url = f"{SERVER_URL}/patients/{patient_id}/medications" # creamos una medicacion nueva para un paciente

        response = requests.post(url, json={ # hacemos una consulta POST
            "id": None,
            "name": name,
            "dosage": dosage,
            "start_date": start_date,
            "treatment_duration": treatment_duration,
            "patient_id": patient_id
        })

        sleep(2) # simulamos el delay despues de la peticion

        data = response.json() # procesamos la respuesta con JSON

        if response.ok: # si la consulta es exitosa
            return Medication(data) # devolvemos la medicacion creada
        else: # si la consulta no es exitosa mandamos el mensaje de error de la API
            raise ModelException("No se ha podido añadir la medicacion, inténtelo más tarde")

    def modify_medication(self, medication_id: int, name: str, dosage: float, start_date: str, treatment_duration: int, patient_id: int) -> None:
        url = f"{SERVER_URL}/patients/{patient_id}/medications/{medication_id}" # modificamos la informacion de una medicacion de un paciente

        response = requests.patch(url, json={ # hacemos una consulta PATCH
            "id": medication_id,
            "name": name,
            "dosage": dosage,
            "start_date": start_date,
            "treatment_duration": treatment_duration,
            "patient_id": patient_id
        })

        sleep(2) # simulamos el delay despues de la peticion

        if response.status_code != 204: # si la consulta no es exitosa mandamos el mensaje de error de la API
            raise ModelException("No se ha podido modificar la medicacion, inténtelo más tarde")

    def delete_medication(self, medication_id: int, patient_id: int) -> None:
        url = f"{SERVER_URL}/patients/{patient_id}/medications/{medication_id}" # borramos una medicacion de un paciente

        response = requests.delete(url) # hacemos una consulta DELETE

        sleep(2) # simulamos el delay despues de la peticion

        if response.status_code != 204: # si la consulta no es exitosa mandamos el mensaje de error de la API
            raise ModelException("No se ha podido eliminar la medicacion, inténtelo más tarde")


    def add_posology(self, patient_id: int, medication_id: int, hour: int, minute: int) -> Posology:
        url = f"{SERVER_URL}/patients/{patient_id}/medications/{medication_id}/posologies" # creamos una posologia nueva para una medicacion de un paciente

        response = requests.post(url, json={ # hacemos una consulta POST
            "id": None,
            "hour": hour,
            "minute": minute,
            "medication_id": medication_id,
        })

        sleep(2) # simulamos el delay despues de la peticion

        data = response.json() # procesamos la respuesta con JSON

        if response.ok: # si la consulta es exitosa
            return Posology(data) # devolvemos la posologia creada
        else: # si la consulta no es exitosa mandamos el mensaje de error de la API
            raise ModelException("No se ha podido añadir la posologia, inténtelo más tarde")

    def modify_posology(self, patient_id: int, medication_id: int, hour: int, minute: int, posology_id: int) -> None:
        url = f"{SERVER_URL}/patients/{patient_id}/medications/{medication_id}/posologies/{posology_id}" # modificamos una posologia

        response = requests.patch(url, json={ # hacemos una consulta PATCH
            "id": posology_id,
            "hour": hour,
            "minute": minute,
            "medication_id": medication_id,
        })

        sleep(2) # simulamos el delay despues de la peticion

        if response.status_code != 204: # si la consulta no es exitosa mandamos el mensaje de error de la API
            raise ModelException("No se ha podido modificar la posologia, inténtelo más tarde")

    def delete_posology(self, patient_id: int, medication_id: int, posology_id: int) -> None:
        url = f"{SERVER_URL}/patients/{patient_id}/medications/{medication_id}/posologies/{posology_id}" # borramos una posologia

        response = requests.delete(url) # hacemos una consulta DELETE

        sleep(2) # simulamos el delay despues de la peticion

        if response.status_code != 204: # si la consulta no es exitosa mandamos el mensaje de error de la API
            raise ModelException("No se ha podido eliminar la posologia, inténtelo más tarde")