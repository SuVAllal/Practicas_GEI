## Plantilla para los ejercicios

| < ID >                | < Nombre de la métrica > |
| :-------------------- | :----------------------- |
| Objetivo              |                          |
| Fórmula de cálculo    |                          |
| Unidad de medida      |                          |
| Origen de los datos   |                          |
| Datos de entrada      |                          |
| Periodicidad          |                          |
| Criterios de análisis |                          |
> **NOTA:** Otro campo útil en la plantilla sería el encargado de tomar la métrica.
## Ejercicio 1
#### Defina, en una plantilla, una métrica que permita conocer el grado de cumplimiento actual de los costes definidos en el Plan de Proyecto.

| 1                     | Cumplimiento de costes                                                                                             |
| --------------------- | ------------------------------------------------------------------------------------------------------------------ |
| Objetivo              | Conocer el grado de cumplimiento actual de los costes definidos en el Plan de Proyecto.                            |
| Fórmula de cálculo    | CA / CP x 100                                                                                                      |
| Unidad de medida      | %                                                                                                                  |
| Origen de los datos   | Plan de proyecto                                                                                                   |
| Datos de entrada      | CA: Costes actuales            CP: Costes planificados                                                             |
| Periodicidad          | Seguimiento                                                                                                        |
| Criterios de análisis | Si el valor > valor previsto para ese momento en el Plan de Proyecto, entonces deben tomarse acciones correctivas. |
## Ejercicio 2
#### Defina, en una plantilla, una métrica que permita conocer el nivel de productividad de los programadores de un proyecto en comparación con otros proyectos de la empresa.
| 2                     | Nivel de productividad de los programadores                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           |
| --------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Objetivo              | Conocer el nivel de productividad de los programadores de un proyecto en comparación con otros proyectos.                                                                                                                                                                                                                                                                                                                                                                                                                             |
| Fórmula de cálculo    | PFC / PFP x 100                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       |
| Unidad de medida      | %                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     |
| Origen de los datos   | Plan de proyecto                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      |
| Datos de entrada      | PFC: Puntos Función Completados                 PFP: Puntos Función Planificados                                                                                                                                                                                                                                                                                                                                                                                                                                                      |
| Periodicidad          | Seguimiento                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           |
| Criterios de análisis | Si se detecta que un programador tiene un alto porcentaje de rendimiento en un proyecto y bajo porcentaje en otro, se le asignarán más tareas del primero y se le asignarán más tareas del primero y se reducirán proporcionalmente las tareas del segundo, ya que un programador con alto % en un proyecto suele estar realizando tareas en las que es productivo. Para no afectar a la duración de los proyectos se reasignarán también las horas de otro programador de forma inversa basándose en los resultados de esta métrica. |
## Ejercicio 3
#### Defina, en una plantilla, una métrica que permita conocer el grado de implementación de los casos de uso previstos para la iteración actual.
| 3                     | Grado de implementación de casos de uso                                                                             |
| :-------------------- | :------------------------------------------------------------------------------------------------------------------ |
| Objetivo              | Conocer el grado de implementación de los casos de uso previstos para la iteración actual.                          |
| Fórmula de cálculo    | NCUI / NCU x 100                                                                                                    |
| Unidad de medida      | %                                                                                                                   |
| Origen de los datos   | ERS                                                                                                                 |
| Datos de entrada      | NCUI: Número de Casos de Uso Implementados                            NCU: Número de Casos de Uso                   |
| Periodicidad          | Seguimiento                                                                                                         |
| Criterios de análisis | Cuanto mayor sea el porcentaje, más cerca se está de cumplir con los requisitos pedidos por el cliente en esa fase. |
## Ejercicio 4
#### Defina, en una plantilla, una métrica que permita conocer el tiempo medio invertido en las revisiones de progreso del proyecto.
| 4                     | Tiempo invertido en las revisiones                                                                                                             |
| :-------------------- | :--------------------------------------------------------------------------------------------------------------------------------------------- |
| Objetivo              | Conocer el tiempo invertido en las revisiones de progreso del proyecto.                                                                        |
| Fórmula de cálculo    | TTS / NR                                                                                                                                       |
| Unidad de medida      | Horas                                                                                                                                          |
| Origen de los datos   | Seguimiento                                                                                                                                    |
| Datos de entrada      | TTS: Tiempo Total Revisiones                                                               NR: Número de Revisiones                            |
| Periodicidad          | Al cerrar el proyecto                                                                                                                          |
| Criterios de análisis | Si alguna de las revisiones de un proyecto ha durado una cantidad de tiempo muy diferente a la media se debe revisar el motivo y justificarlo. |
## Ejercicio 5
#### Defina, en una plantilla, una métrica que permita determinar el grado de criticidad de un proyecto.
| 5                     | Grado de criticidad                                                                                                                                                           |
| :-------------------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Objetivo              | Determinar el grado de criticidad de un proyecto.                                                                                                                             |
| Fórmula de cálculo    | NTC                                                                                                                                                                           |
| Unidad de medida      | Valor numérico                                                                                                                                                                |
| Origen de los datos   | Plan de Proyecto                                                                                                                                                              |
| Datos de entrada      | NTC: Número de Tareas Críticas                                                                                                                                                |
| Periodicidad          | Seguimiento                                                                                                                                                                   |
| Criterios de análisis | El impacto de las consecuencias del riesgo se mide del 1 al 5, siendo 1 un riesgo asumible y 5 un riesgo perjudicial. Cuanto mayor sea el número, más crítico es el proyecto. |
## Ejercicio 6
#### Defina, en una plantilla, una métrica que permita detectar el grado de desviación en tareas críticas de un proyecto.
| 6                     | Grado de desviación en tareas críticas                                                                                            |
| :-------------------- | :-------------------------------------------------------------------------------------------------------------------------------- |
| Objetivo              | Detectar el grado de desviación en tareas críticas de un proyecto.                                                                |
| Fórmula de cálculo    | (TRTC - TPTC) / TPTC x 100                                                                                                        |
| Unidad de medida      | %                                                                                                                                 |
| Origen de los datos   | Plan de Proyecto                                                                                                                  |
| Datos de entrada      | TRTC: Tiempo Real Tareas Críticas                                                        TPTC: Tiempo Planificado Tareas Críticas |
| Periodicidad          | Seguimiento                                                                                                                       |
| Criterios de análisis | Un porcentaje positiva indica desviaciones, y por tanto, un aumento de la duración del camino crítico y la duración del proyecto. |
## Ejercicio 7
#### Defina, en una plantilla, una métrica que permita conocer el grado de acoplamiento entre las clases de diseño en un proyecto.
| 7                     | Grado de acoplamiento                                                                       |
| :-------------------- | :------------------------------------------------------------------------------------------ |
| Objetivo              | Conocer el grado de acoplamiento entre las clases de diseño en un proyecto.                 |
| Fórmula de cálculo    | NCER / TC                                                                                   |
| Unidad de medida      | Valor numérico                                                                              |
| Origen de los datos   | Diagramas de diseño de bajo nivel (DBN)                                                     |
| Datos de entrada      | NCER: Número de Clases Externas Referenciadas por la clase              TC: Total de Clases |
| Periodicidad          | Fase de diseño de bajo nivel                                                                |
| Criterios de análisis | Cuanto mayor sea el nivel, mayor será el acoplamiento de la clase con otras del sistema.    |
## Ejercicio 8
#### Defina, en una plantilla, una métrica que permita conocer el grado de compleción de los requisitos iniciales de un proyecto.
| 8                     | Grado de compleción                                                                                                 |
| :-------------------- | :------------------------------------------------------------------------------------------------------------------ |
| Objetivo              | Conocer el grado de compleción de los requisitos iniciales de un proyecto.                                          |
| Fórmula de cálculo    | NRC / TR x 100                                                                                                      |
| Unidad de medida      | %                                                                                                                   |
| Origen de los datos   | ERS                                                                                                                 |
| Datos de entrada      | NRC: Número Requisitos Completados                                                TR: Total Requisitos Iniciales    |
| Periodicidad          | Seguimiento                                                                                                         |
| Criterios de análisis | Cuanto mayor sea el %, mayor será el número de requisitos completos sugeridos por el cliente en la primera reunión. |
