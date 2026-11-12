# Práctica Evaluada 03 — Modelo B: Pila de acciones (deshacer y rehacer)

**Asignatura:** Programación 2 (PR2) — Sección 02  
**Modalidad:** grupos de 3 o 4 integrantes  
**Lenguaje:** C++17, sin librerías externas  
**Duración:** miércoles 11 de noviembre de 2026, 12:00 p.m. – 2:00 p.m. (90 min de práctica + 30 min de revisión) + trabajo en casa  
**Puntaje:** 20 puntos (12 en clase + 8 en casa)  
**Estructura asignada:** pila enlazada (LIFO)

> [!IMPORTANT]
> **Medio de entrega:** correo electrónico a **`jdanieldp99@gmail.com`**.
> **No se adjuntan archivos `.cpp`.** El mensaje debe incluir **la captura de `make test` funcionando durante la clase** y **el enlace del repositorio** de GitHub/GitLab. Ver la plantilla completa en la **Sección 10**.
>
> **Fecha límite:** **miércoles 11 de noviembre de 2026, 11:59 p.m.** (el mismo día de la práctica).
> **Retardo:** **−5 puntos** sobre la nota final.
> **Puntos extra:** quienes entreguen el **resultado completo durante el tiempo de clase** recibirán puntos extra **a criterio del preparador**.

---

## 1. Contexto

En la Evaluación 1 modelaron un sistema de mensajería con usuarios, chats, mensajes y estados (`Pending → Sent → Delivered → Opened`). Ahora deben extender ese sistema con **deshacer y rehacer**: cada envío, edición o eliminación de un mensaje se registra en una pila, el usuario puede revertir la última acción y también volver a aplicarla.

Para eso se usan **dos pilas**: una de acciones realizadas y otra de acciones deshechas. La pila debe implementarse **desde cero** con nodos enlazados. No se permite usar `std::stack` ni `std::vector` como almacenamiento principal. El núcleo de mensajería (`User`, `TextMessage`, `Chat`) es una **adaptación mínima de la carpeta original de la Evaluación 1** y ya está resuelto en `nucleo/`, junto con la clase `Accion`.

---

## 2. Qué debe implementar

### A. `PilaAcciones` (`PilaAcciones.hpp` / `.cpp`)

Pila enlazada de `Accion`. Cada nodo debe tener la acción y un apuntador al siguiente nodo.

| Método | Comportamiento |
| :--- | :--- |
| `PilaAcciones()` | Pila vacía: `vacia() == true`, `tamanio() == 0`, `cima() == nullptr`. |
| `apilar(const Accion&)` | Inserta en la cima. Retorna `true`. |
| `desapilar()` | Retira la cima y libera el nodo. Retorna `false` si la pila está vacía. |
| `cima()` | Retorna apuntador a la acción superior o `nullptr`. Debe existir versión `const` y no `const`. |
| `limpiar()` | Deja la pila vacía. |
| `aVector()` | Retorna las acciones de **cima a base**, sin modificar la pila. |
| `buscarPorId(int id)` | Busca la acción cuyo mensaje tiene ese `id` y retorna un apuntador o `nullptr`. Debe existir versión `const` y no `const`. |
| `eliminarPorId(int id)` | Elimina la acción con ese `id` esté donde esté en la pila, reconectando los enlaces. Retorna `false` si no existe. |
| `moverAlCima(int id)` | Saca la acción de su posición y la coloca en la cima. Retorna `false` si no existe. |
| `invertir()` | Invierte el orden de los nodos: la base pasa a ser la cima y viceversa. |
| `guardarCSV(ruta)` | Guarda una acción por línea, de cima a base. Retorna `false` si no puede abrir. |
| `cargarCSV(ruta)` | Limpia la pila y carga las acciones conservando el orden LIFO. Retorna `false` si no puede abrir. |

**Copia, movimiento y liberación (regla de tres/cinco):** copia profunda y movimiento seguro (el origen queda vacío), sin fugas ni dobles liberaciones.

### B. `ChatPila` (`ChatPila.hpp` / `.cpp`)

Hereda de `Chat`, guarda los mensajes en un `std::vector<TextMessage>` y las acciones en **dos** `PilaAcciones`: `pila` (acciones realizadas) y `pilaRehacer` (acciones deshechas).

| Método | Comportamiento |
| :--- | :--- |
| `enviarMensaje(TextMessage)` | Valida participante, id duplicado y estado `Pending`. Llama a `markSent(getId())`, guarda el mensaje, **limpia `pilaRehacer`** y apila una acción `Enviar`. |
| `editarMensaje(int id, const std::string& nuevoTexto)` | Busca el mensaje, guarda el texto anterior, cambia el contenido, **limpia `pilaRehacer`** y apila una acción `Editar` con el texto anterior. Retorna `false` si el mensaje no existe o el texto está vacío. |
| `eliminarMensaje(int id)` | Guarda una copia del mensaje, lo elimina del vector, **limpia `pilaRehacer`** y apila una acción `Eliminar` con la copia. Retorna `false` si no existe. |
| `deshacer()` | Toma la cima de `pila`, revierte la acción, la desapila y guarda en `pilaRehacer` la acción inversa. Retorna `false` si la pila está vacía. |
| `rehacer()` | Toma la cima de `pilaRehacer`, vuelve a aplicar la acción, la desapila y la devuelve a `pila`. Retorna `false` si no hay nada que rehacer. |
| `getMensajes()` | Retorna el vector de mensajes. |
| `getPila()` | Retorna la pila de acciones realizadas. |
| `getPilaRehacer()` | Retorna la pila de acciones deshechas. |
| `cantidadMensajes()` | Retorna la cantidad de mensajes actuales. |
| `buscarMensaje(int id)` | Retorna apuntador al mensaje o `nullptr`. Debe existir versión `const` y no `const`. |

### C. Semántica de `deshacer()` y `rehacer()`

`deshacer()` trabaja sobre la cima de `pila` y `rehacer()` sobre la cima de `pilaRehacer`.

| Acción | Qué debe hacer `deshacer()` | Qué debe hacer `rehacer()` |
| :--- | :--- | :--- |
| `Enviar` | Eliminar el mensaje con el id indicado. | Volver a insertar el mensaje enviado. |
| `Eliminar` | Restaurar el mensaje guardado en la acción, si no existe ya. | Volver a eliminar el mensaje restaurado. |
| `Editar` | Restaurar el `textoAnterior` del mensaje con el id indicado. | Volver a aplicar el texto que tenía antes de deshacer. |

Las acciones se deshacen en orden inverso al que se hicieron (LIFO). Al deshacer, la acción inversa se guarda en `pilaRehacer`; al rehacer, vuelve a `pila`. Cualquier acción nueva limpia `pilaRehacer`.

```text
enviar m1        → pila: [Enviar m1]                         rehacer: []
editar m1 "b"    → pila: [Editar m1, Enviar m1]              rehacer: []
editar m1 "c"    → pila: [Editar m1, Editar m1, Enviar m1]   rehacer: []
deshacer()       → m1 vuelve a "b"                           rehacer: [Editar m1]
rehacer()        → m1 vuelve a "c"                           rehacer: []
deshacer()       → m1 vuelve a "b"                           rehacer: [Editar m1]
deshacer()       → m1 vuelve a "hola"                        rehacer: [Editar m1, Editar m1]
```

---

## 3. Reglas técnicas

- **Memoria:** pueden usar apuntadores inteligentes (`std::shared_ptr`, `std::unique_ptr`, `std::weak_ptr`) o apuntadores normales con `new` y `delete`. Si usan apuntadores normales, deben liberar todos los nodos y evitar dobles liberaciones. La ausencia de fugas es parte de la evaluación.
- **No usar** `std::stack` ni `std::vector` como almacenamiento principal de la pila. Sí se permite `std::vector` para los mensajes del chat y para `aVector()`.
- Nada de atributos públicos en las clases evaluadas.
- Una clase por par `.hpp` / `.cpp`.
- Compilar con `make test` sin errores y, preferiblemente, sin advertencias con `-Wall -Wextra -Wpedantic`.
- Comentarios en el código sin acentos.
- No modifiquen `Accion.hpp` ni `Accion.cpp`: ya están resueltos. Tampoco modifiquen `nucleo/`.

---

## 4. Formato CSV de la pila

Cada línea representa una acción, de cima a base:

```text
Tipo,id,"remitente","contenido",Estado,"textoAnterior"
```

Ejemplo:

```text
Editar,2,"u1","mensaje 2",sent,"texto viejo"
Enviar,2,"u1","mensaje 2",sent,""
Enviar,1,"u1","mensaje 1",sent,""
```

Al cargar, el orden debe quedar igual: la primera línea del archivo debe ser la cima de la pila.

---

## 5. Estructura de la carpeta

Este modelo es **independiente**: todo lo necesario está dentro de `modelo_b_pila/`.

```text
modelo_b_pila/
├── enunciado.md
├── Makefile
├── main.cpp                  ← demo propia (evaluada en A5)
├── Accion.hpp / .cpp         ← código dado (no evaluado)
├── PilaAcciones.hpp / .cpp   ← completar
├── ChatPila.hpp / .cpp       ← completar
├── nucleo/                   ← código dado (no evaluado)
│   ├── User.hpp / .cpp
│   ├── MessageStatus.hpp / .cpp
│   ├── TextMessage.hpp / .cpp
│   ├── Chat.hpp / .cpp
│   └── CSV.hpp
└── tests/test_modelo.cpp     ← suite automática
```

Los archivos `.hpp` y `.cpp` de `PilaAcciones` y `ChatPila`, y `main.cpp`, incluyen al inicio el bloque de **datos del grupo**. Deben completarlo.

---

## 6. Inicio rápido y verificación mínima

```bash
cd modelo_b_pila

make test        # verifica requisitos mínimos (NO asigna nota)
make main        # compila y ejecuta tu demo (main.cpp)
make clean
```

La suite `tests/test_modelo.cpp` **NO asigna nota**. Solo verifica si el esqueleto ya cumple los requisitos mínimos:

| Bloque | Qué verifica |
| :--- | :--- |
| 1. Estructura desde cero | Nodos, enlaces, inserciones y recorridos |
| 2. Operaciones núcleo | Búsquedas, eliminaciones y operaciones propias del modelo |
| 3. Integración con mensajería | Uso real de la estructura dentro del chat |
| 4. Casos borde y robustez | Estructura vacía, ids inexistentes, copia y movimiento |
| 5. Persistencia CSV | Guardar y cargar conservando orden y datos |

Al final imprime `CUMPLE EL MINIMO` o `AÚN NO CUMPLE EL MINIMO` con el número de verificaciones superadas. La nota la asigna el preparador con la rúbrica.

**`main.cpp` es una plantilla que ustedes deben completar.** No lo evalúa la suite: forma parte de la rúbrica (A5) como demo propia del grupo.

---

## 7. Reparto del trabajo (grupos de 3 o 4)

La práctica está diseñada para que **no alcance con que una sola persona programe todo**. Cada integrante debe tener una responsabilidad verificable en Git.

### Grupos de 4

| Rol | Responsabilidad | Evidencia en Git |
| :--- | :--- | :--- |
| **Estructura** | Nodos, enlaces, apilar/desapilar, copia y liberación (regla de tres/cinco), recorridos | Rama `feature/estructura-<nombre>` y commits de los `.cpp` de la estructura |
| **Integración** | `ChatPila`, deshacer/rehacer, validaciones de participantes y estados | Rama `feature/integracion-<nombre>` y PR con revisión |
| **Pruebas y persistencia** | Casos borde, CSV, ejecución de `make test`, reporte de fallos | Rama `feature/tests-<nombre>` y comentarios en issues |
| **Main, documentación y QA** | `main.cpp`, README, `.gitignore`, tablero de issues, revisión final | Rama `feature/docs-<nombre>`, PR final y tag |

### Grupos de 3

| Rol | Responsabilidad |
| :--- | :--- |
| **Estructura** | Nodos, enlaces, operaciones, copia y liberación |
| **Integración + persistencia** | `ChatPila`, deshacer/rehacer, validaciones y CSV |
| **Pruebas + main + docs + Git** | Casos borde, demo, README, issues, PRs y tag |

> El rol no es excluyente: todos deben entender todo. La participación de cada integrante se evidencia en el historial de Git y en la observación durante la clase.

---

## 8. Cronograma de la sesión

**Miércoles 11 de noviembre de 2026**

| Hora | Actividad |
| :--- | :--- |
| **12:00 – 12:10** | Formar grupos, asignar modelo, crear repositorio y tablero de issues |
| **12:10 – 12:20** | Leer el enunciado, acordar el diseño de nodos y firmar los headers |
| **12:20 – 1:05** | Implementar bloques 1-2 de la suite (estructura y operaciones núcleo) |
| **1:05 – 1:25** | Integrar con el chat (bloque 3) y ejecutar `make test` |
| **1:25 – 1:30** | Commit/PR de cierre, captura del resultado y checkpoint Git |
| **1:30 – 2:00** | Revisión del avance, captura de `make test` y `make main` y cierre de la entrega de clase |

Al terminar la clase deben estar resueltos, como mínimo, los **12 primeros puntos**. El resto (casos borde, persistencia y Git colaborativo) queda como **entrega de casa** desde las 2:00 p.m. hasta las **11:59 p.m. del mismo día**.

> **Puntos extra:** los grupos que entreguen el **resultado completo durante el tiempo de clase** recibirán puntos extra **a criterio del preparador**.

---

## 9. Git/GitHub obligatorio

Cada grupo debe entregar un repositorio con:

1. `README.md` con integrantes, modelo asignado, instrucciones de compilación, uso de IA y reparto de roles.
2. `.gitignore` que excluya `build/`, `*.o`, `*.exe`, binarios, archivos de prueba y temporales.
3. Una rama por integrante y commits con mensajes claros.
4. Al menos **2 Pull Requests** revisados por otro integrante y fusionados a `main`.
5. Un tag `v1.0-entrega` al final.
6. Enlace del repositorio en la entrega.

---

## 10. Rúbrica y entrega

La evaluación se distribuye así:

| Bloque | Puntos | Momento |
| :--- | ---: | :--- |
| A1. Estructura desde cero | 4.0 | En clase |
| A2. Operaciones núcleo | 3.0 | En clase |
| A3. Integración con mensajería | 3.0 | En clase |
| A4. Checkpoint Git | 1.0 | En clase |
| A5. Demo propia (`main.cpp`) | 1.0 | En clase |
| B1. Casos borde y robustez | 3.0 | En casa |
| B2. Persistencia CSV | 2.0 | En casa |
| B3. Git colaborativo y documentación | 3.0 | En casa |
| **Total** | **20.0** | |

La nota es **grupal** y luego se multiplica por el **factor individual** de la rúbrica, calculado con la participación real en Git y la observación durante la clase.

### 10.1. Qué se entrega y cómo

> [!IMPORTANT]
> **No se envían archivos `.cpp` adjuntos.** La entrega es un **mensaje por correo electrónico** a **`jdanieldp99@gmail.com`** con:
>
> 1. **Captura de pantalla** de `make test` y `make main` funcionando **durante la clase**, donde se vean el resultado (`CUMPLE` / `AÚN NO CUMPLE`), la terminal y la fecha/hora.
> 2. **Enlace del repositorio** de GitHub/GitLab (público o con acceso aceptado para el preparador).
> 3. **Nombres, cédulas y roles** de los integrantes.
> 4. **Modelo asignado:** B (pila LIFO con deshacer y rehacer).
> 5. **Resultado de `make test`:** `CUMPLE EL MINIMO` o `AÚN NO CUMPLE EL MINIMO`, indicando cuántas verificaciones se superaron.
> 6. **Herramientas de IA** utilizadas y para qué.

La captura de la clase es la evidencia del **checkpoint de 12 pts**. El repositorio es la evidencia del trabajo completo y de los **8 pts restantes**.

### 10.2. Formato del mensaje

**Asunto:**

```text
[PR2-B2026] Práctica 03 - Modelo B - <Apellido1, Apellido2, ...> - C.I. <V-XXXXXXXX>
```

**Cuerpo:**

```text
Profesor Jesús Díaz y equipo de preparaduría,

Entrego la Práctica Evaluada 03 correspondiente al Modelo B (pila LIFO con deshacer y rehacer).

Datos del grupo:
- Integrante 1: <Nombre y Apellido> - C.I. <V-XXXXXXXX> - Rol: <rol>
- Integrante 2: <Nombre y Apellido> - C.I. <V-XXXXXXXX> - Rol: <rol>
- Integrante 3: <Nombre y Apellido> - C.I. <V-XXXXXXXX> - Rol: <rol>
- Integrante 4 (si aplica): <Nombre y Apellido> - C.I. <V-XXXXXXXX> - Rol: <rol>

Enlace del repositorio:
<https://github.com/usuario/repositorio>

Evidencia:
- Captura de make test durante la clase: adjunta al correo.
- Resultado de make test: <CUMPLE EL MINIMO / AUN NO CUMPLE EL MINIMO> — <X>/<Y> verificaciones.
- Demo propia: `main.cpp` compila y se ejecuta con `make main`.
- Tag de entrega: v1.0-entrega.

Herramientas de IA utilizadas:
<ninguna / ChatGPT / Claude / Copilot / otra> — uso: <para qué>.

Quedo atento a cualquier observación.
```

### 10.3. Fechas y penalización

- **Fecha límite final:** **miércoles 11 de noviembre de 2026, 11:59 p.m.** (el mismo día de la práctica).
- **Retardo:** **−5 puntos** sobre la nota final.
- **Puntos extra:** quienes entreguen el **resultado completo durante el tiempo de clase** recibirán puntos extra **a criterio del preparador**.
- Si el repositorio no es accesible o el enlace está roto, la entrega se considera **no recibida** hasta que se corrija.
- El código debe estar **en el repositorio**, no en el correo. No se aceptan `.cpp`, `.zip`, `.exe` ni `.o` adjuntos.

---

## 11. Uso de inteligencia artificial

Se permite usar asistentes de IA como apoyo (ChatGPT, Claude, Copilot, etc.), pero:

- Deben declararlo en el `README.md` indicando **qué herramienta** usaron y **para qué**.
- Son responsables de todo el código entregado: deben poder explicar cada clase, cada método y cada decisión, especialmente la reversión de acciones y la pila de rehacer.
- Si un integrante no participó en el repositorio ni durante la clase, se aplica el factor individual de la rúbrica.
- No se permite entregar código generado sin comprenderlo ni adaptarlo al modelo asignado.

---

## 12. Checklist rápido

- [ ] La pila funciona vacía, con una acción y con varias.
- [ ] `apilar` y `desapilar` respetan LIFO.
- [ ] `cima` retorna la acción superior o `nullptr`.
- [ ] `aVector` retorna de cima a base.
- [ ] `buscarPorId`, `eliminarPorId`, `moverAlCima` e `invertir` funcionan sin romper la pila.
- [ ] La copia es profunda y el movimiento deja el origen vacío.
- [ ] `ChatPila` valida participante, duplicados y estado.
- [ ] `deshacer` revierte Enviar, Eliminar y Editar correctamente.
- [ ] `rehacer` vuelve a aplicar la acción deshecha y una acción nueva limpia la pila de rehacer.
- [ ] Varias acciones se deshacen y rehacen en orden LIFO.
- [ ] El CSV conserva el orden LIFO y los datos de cada acción.
- [ ] No hay `std::stack`; la memoria se libera correctamente.
- [ ] El repositorio tiene ramas, PRs, README, `.gitignore` y tag.

---

*Modelo B de la Práctica Evaluada 03 — PR2 Sección 02, ULA.*
