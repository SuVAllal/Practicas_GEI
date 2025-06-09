const source = "http://127.0.0.1:8000";

async function getPatients() {
    try {
        const response = await fetch(`${source}/patients`);

        if(!response.ok) {
            throw new Error("No se pudo cargar los pacientes.");
        }

        return await response.json();
    } catch(e) {
        throw new Error("No se pudo cargar los pacientes.");
    }
}

async function getPatient(patientId) {
    try {
        response = await fetch(`${source}/patients/${patientId}`, {method: 'GET'});

        if(!response.ok) {
            throw new Error("Paciente no encontrado.");
        }

        return await response.json();
    } catch(e) {
        throw new Error("No se pudo encontrar el paciente.");
    }
}

async function getMedications(patientId) {
    try {
        response = await fetch(`${source}/patients/${patientId}/medications`);

        if(!response.ok) {
            throw new Error("No se pudo obtener las medicaciones.");
        }

        return await response.json();
    } catch(e) {
        throw new Error("No se pudo obtener las medicaciones.");
    }
}

async function getPosologies(patientId, medicationId) {
    try {
        response = await fetch(`${source}/patients/${patientId}/medications/${medicationId}/posologies`);

        if(!response.ok) {
            const errorDetails = await response.text();
            console.error("Error al obtener las posologías:", errorDetails);
            throw new Error("No se pudo obtener las posologías.");
        }

        return await response.json();
    } catch(e) {
        throw new Error("No se pudo obtener las posologías.");
    }
}

async function getIntakes(patientId, medicationId, startDate = null, endDate = null) {
    try {
        let url = `${source}/patients/${patientId}/medications/${medicationId}/intakes`;

        // Si se pasan las fechas, las añadimos a los parámetros de consulta
        if (startDate && endDate) {
            url += `?start_date=${encodeURIComponent(startDate)}&end_date=${encodeURIComponent(endDate)}`;
        }

        response = await fetch(url);

        if(!response.ok) {
            const errorDetails = await response.text();
            console.error("Error al obtener las ingestas:", errorDetails);
            throw new Error("No se pudo cargar las ingestas.");
        }

        return await response.json();
    } catch(e) {
        throw new Error("No se pudo cargar las ingestas.");
    }
}