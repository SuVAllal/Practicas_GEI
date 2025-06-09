document.addEventListener("DOMContentLoaded", function() {
    const patientListView = document.getElementById("patientListView");
    const patientList = document.getElementById("patient");
    const patientInformeView = document.getElementById("patientInformeView");
    const selectPatient = document.getElementById("selectPatient");
    const filterSelect = document.getElementById("filterSelect");
    const nDiasInput = document.getElementById("nDaysInput");
    const customDateRange = document.getElementById("customDateRange");
    const timeRangeTitle = document.getElementById("timeRangeTitle");
    const reportBody = document.getElementById("reportBody");
    const applyFilterButton = document.getElementById("applyFilterButton");
    const backButton = document.getElementById("backButton");

    // Función de formateo de fechas para la API
    function formatDateForAPI(date) {
        // Obtenemos el año, mes, día, hora y minutos en el formato requerido
        const year = date.getFullYear();
        const month = (date.getMonth() + 1).toString().padStart(2, "0"); // Mes con dos dígitos
        const day = date.getDate().toString().padStart(2, "0"); // Día con dos dígitos
        const hours = date.getHours().toString().padStart(2, "0"); // Hora con dos dígitos
        const minutes = date.getMinutes().toString().padStart(2, "0"); // Minutos con dos dígitos

        // Retornar la fecha en el formato YYYY-MM-DDTHH:MM
        return `${year}-${month}-${day}T${hours}:${minutes}`;
    }

    // Calcular las fechas del "Último mes"
    function getLastMonthRange() {
        const currentDate = new Date();
        const lastMonthStart = new Date(currentDate);
        lastMonthStart.setMonth(currentDate.getMonth() - 1); // A la fecha actual le quitamos un mes
        const lastMonthEnd = new Date(currentDate); // El final del rango es la fecha actual

        // Formateamos las fechas a un formato válido para la API
        return {
            start_date: formatDateForAPI(lastMonthStart),
            end_date: formatDateForAPI(lastMonthEnd)
        };
    }

    // Calcular las fechas para los últimos N días
    function getLastNDaysRange(days) {
        const currentDate = new Date();
        const nDaysAgo = new Date(currentDate);
        nDaysAgo.setDate(currentDate.getDate() - days);

        return {
            start_date: formatDateForAPI(nDaysAgo),
            end_date: formatDateForAPI(currentDate)
        };
    }

    //Llamar a la API para cargar los pacientes
    async function loadPatients() {
        try {
            // Llamamos al model
            const patients = await getPatients();

            // Borramos los existentes
            patientList.innerHTML = "";

            // Añade cada paciente como una opcion
            patients.forEach(patient => {
                const option = document.createElement("option");
                option.value = patient.id;
                option.textContent = `${patient.surname}, ${patient.name}`;
                patientList.appendChild(option);
            });
        } catch (e) {
            alert("No se pudo cargar los pacientes.");
        }
    }

    // Llamar a la API para cargar las medicaciones
    async function loadMedications(patientId, start_date, end_date) {
        try {
            const medications = await getMedications(patientId);

            // Borramos las existentes
            reportBody.innerHTML = "";

            // Insertar cada medicación como una fila de la tabla
            for(const medication of medications) {
                try {
                    // Obtenemos las posologías del medicamento
                    const posologies = await getPosologies(patientId, medication.id);

                    // Obtenemos las ingestas de ese medicamento
                    const intakes = await getIntakes(patientId, medication.id, start_date, end_date);

                    // Creamos una fila por cada posología
                    for(const intake of intakes) {
                        const row = document.createElement("tr");

                        // Crear las celdas para cada columna
                        const nameCell = document.createElement("td");
                        nameCell.textContent = medication.name; // Nombre de la medicacion
                        
                        // Día de la ingesta
                        const dayCell = document.createElement("td");
                        const intakeDate = new Date(intake.date);
                        const intakeDay = `${intakeDate.getDate().toString().padStart(2, "0")}/${(intakeDate.getMonth() + 1).toString().padStart(2, "0")}/${intakeDate.getFullYear()}`;
                        dayCell.textContent = intakeDay;

                        // Hora de la ingesta
                        const timeCell = document.createElement("td");
                        const intakeTime = `${intakeDate.getHours().toString().padStart(2, "0")}:${intakeDate.getMinutes().toString().padStart(2, "0")}`;
                        timeCell.textContent = intakeTime;

                        // Buscamos la posología más cercana en hora (su correspondiente)
                        let closestPosology = null;
                        let closestPosologyTime = "";
                        let minDeviation = Infinity;
                        
                        posologies.forEach(posology => {
                            const posologyTime = new Date();
                            posologyTime.setHours(posology.hour, posology.minute, 0, 0);

                            const intakePosologyTime = new Date();
                            intakePosologyTime.setHours(intakeDate.getHours(), intakeDate.getMinutes(), 0, 0);

                            // Calcular la desviación entre las dos en minutos
                            const deviation = Math.abs((intakePosologyTime - posologyTime) / (1000 * 60));

                            // Comparamos para encontrar la posología más cercana
                            if(deviation < minDeviation) {
                                minDeviation = deviation;
                                closestPosology = posology;
                                closestPosologyTime = `${posology.hour.toString().padStart(2, "0")}:${posology.minute.toString().padStart(2, "0")}`;
                            }
                        });

                        // Hora de la ingesta correcta
                        const correctTimeCell = document.createElement("td");
                        if(closestPosology) {
                            correctTimeCell.textContent = closestPosologyTime;
                        } else {
                            correctTimeCell.textContent = "-"; // Si no se encuentra una posología 
                        }

                        // Calculamos la desviación en minutos si hay una posología más cercana
                        const deviationCell = document.createElement("td");
                        if(closestPosology) {
                            deviationCell.textContent = `${Math.round(minDeviation)} min`;
                        } else {
                            deviationCell.textContent = "-"; // Si no se encontró posología cercana
                        }

                        // Añadir las celdas a la fila
                        row.appendChild(nameCell);
                        row.appendChild(dayCell);
                        row.appendChild(timeCell);
                        row.appendChild(correctTimeCell);
                        row.appendChild(deviationCell);

                        // Añadir la fila al cuerpo de la tabla
                        reportBody.appendChild(row);
                    }
                } catch(e) {
                    alert("No se pudo cargar las posologías o ingestas.");
                }
            };
        } catch (e) {
            alert("No se pudo cargar las medicaciones.");
        }
    }

    // Llamamos a loadPatients al cargar la página
    loadPatients();

    // Mostrar informe al seleccionar paciente
    document.getElementById("patientList").addEventListener("submit", function(event) {
        event.preventDefault();  // Evita la recarga de la página

        // Obtener el paciente seleccionado
        const selectedPatientId = patientList.value;
        if (selectedPatientId) {
            // Actualizar el título del informe con el paciente seleccionado
            selectPatient.textContent = `Informe de Paciente ${selectedPatientId}`;

            // Cambiar de vista
            patientListView.style.display = "none";  // Ocultar la vista de lista de pacientes
            patientInformeView.style.display = "block";  // Mostrar la vista del informe

            // Mostrar la flecha para volver
            backButton.style.display = "block";

            // Cargamos las medicaciones para el paciente seleccionado
            loadMedications(selectedPatientId);
        }
    });

    // Detectar el cambio en el filtro de selección
    filterSelect.addEventListener("change", function() {
        nDiasInput.style.display = "none";
        customDateRange.style.display = "none";

        if (filterSelect.value === "nDias") {
            nDiasInput.style.display = "block";
            customDateRange.style.display = "none";
            timeRangeTitle.textContent = "Últimos N días";
        } else if (filterSelect.value === "custom") {
            customDateRange.style.display = "block";
            nDiasInput.style.display = "none";
            timeRangeTitle.textContent = "Fecha inicio - Fecha fin";
        } else {
            customDateRange.style.display = "none";
            nDiasInput.style.display = "none";
            timeRangeTitle.textContent = "Último mes";
        }
    });

    // Aplicar el filtro cuando se pulse en "Aplicar"
    applyFilterButton.addEventListener("click", function() {
        const selectedPatientId = patientList.value;

        if(selectedPatientId) {
            // Uso de filtros
            let start_date, end_date;
            
            if(filterSelect.value === "nDias") { // Filtro de N días
                const nDays = parseInt(nDiasInput.querySelector("input").value, 10); // Obtenemos el valor de los N días
                if(isNaN(nDays) || nDays <= 0) {
                    alert("Por favor, ingrese un número válido de días.");
                    return;
                }

                // Obtener las fechas para los últimos N días
                ({start_date, end_date} = getLastNDaysRange(nDays));
            } else if (filterSelect.value === "custom") { // Filtro de fechas personalizadas
                start_date = document.querySelector("#startDate").value;
                end_date = document.querySelector("#endDate").value;

                if(!start_date || !end_date) {
                    alert("Por favor, selecciona un rango de fechas válido.");
                    return;
                }

                start_date = formatDateForAPI(new Date(start_date));
                end_date = formatDateForAPI(new Date(end_date));

            } else { // Como predeterminado, filtro de último mes
                ({start_date, end_date} = getLastMonthRange());
            }

            // Cargar informe con el filtro aplicado
            loadMedications(selectedPatientId, start_date, end_date);
        }
    });

    // Funcionalidad del botón Volver
    backButton.addEventListener("click", function() {
        patientInformeView.style.display = "none"; // Ocultar la vista del informe
        patientListView.style.display = "block"; // Volver a mostrar la lista de pacientes

        // Ocultamos la flecha de nuevo
        backButton.style.display = "none";
    })
    
});