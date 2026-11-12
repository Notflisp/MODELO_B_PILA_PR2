// ====================================================================
// PRACTICA 03 - MODELO B (PILA ENLAZADA)
// Verificador de requisitos minimos.
// NO asigna nota: solo indica si el esqueleto ya cumple lo minimo.
// La calificacion la realiza el preparador con la rubrica.
// ====================================================================

#include <cstdio>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Accion.hpp"
#include "ChatPila.hpp"
#include "TextMessage.hpp"
#include "PilaAcciones.hpp"

using namespace std;

static int total[6] = {0, 0, 0, 0, 0, 0};
static int superadas[6] = {0, 0, 0, 0, 0, 0};

static void test(int bloque, const string& nombre, bool ok)
{
    ++total[bloque];
    if (ok)
    {
        ++superadas[bloque];
    }

    cout << "  [" << (ok ? "OK" : "FALLA") << "] " << nombre << endl;
}

static Accion accionEnviar(int id)
{
    return Accion(TipoAccion::Enviar, TextMessage(id, "u1", "mensaje " + to_string(id)));
}

int main()
{
    cout << "====================================================================" << endl;
    cout << "        PRACTICA 03 - MODELO B: PILA ENLAZADA (C++)                 " << endl;
    cout << "====================================================================\n" << endl;

    // ------------------------------------------------------------------
    // BLOQUE 1: ESTRUCTURA DESDE CERO
    // ------------------------------------------------------------------
    cout << "--- BLOQUE 1: Estructura desde cero ---" << endl;

    PilaAcciones vacia;
    const bool b1_1 = vacia.vacia() && vacia.tamanio() == 0 && vacia.cima() == nullptr;
    test(1, "1.1 Pila recien creada: vacia, tamanio 0, cima nullptr", b1_1);

    PilaAcciones pila;
    const Accion a1 = accionEnviar(1);
    const Accion a2 = accionEnviar(2);
    const Accion a3 = accionEnviar(3);
    const bool p1 = pila.apilar(a1);
    const bool p2 = pila.apilar(a2);
    const bool p3 = pila.apilar(a3);
    const bool b1_2 = p1 && p2 && p3 && pila.tamanio() == 3 && !pila.vacia() &&
                      pila.cima() != nullptr && pila.cima()->getMensaje().getId() == 3;
    test(1, "1.2 apilar() respeta LIFO: la ultima accion queda en la cima", b1_2);

    const bool desapilo = pila.desapilar();
    const bool b1_3 = desapilo && pila.tamanio() == 2 &&
                      pila.cima() != nullptr && pila.cima()->getMensaje().getId() == 2;
    test(1, "1.3 desapilar() retira la cima y actualiza tamanio", b1_3);

    PilaAcciones pilaUna;
    pilaUna.apilar(a1);
    const bool sacaUna = pilaUna.desapilar();
    const bool sacaVacia = pilaUna.desapilar();
    const bool b1_4 = sacaUna && !sacaVacia && pilaUna.vacia();
    test(1, "1.4 desapilar() retorna false cuando la pila ya esta vacia", b1_4);

    PilaAcciones pilaVector;
    pilaVector.apilar(a1);
    pilaVector.apilar(a2);
    pilaVector.apilar(a3);
    const vector<Accion> acciones = pilaVector.aVector();
    const bool b1_5 = acciones.size() == 3 &&
                      acciones[0].getMensaje().getId() == 3 &&
                      acciones[1].getMensaje().getId() == 2 &&
                      acciones[2].getMensaje().getId() == 1;
    test(1, "1.5 aVector() retorna las acciones de cima a base", b1_5);

    cout << endl;

    // ------------------------------------------------------------------
    // BLOQUE 2: OPERACIONES NUCLEO
    // ------------------------------------------------------------------
    cout << "--- BLOQUE 2: Operaciones nucleo ---" << endl;

    const bool cimaVacia = vacia.cima() == nullptr;
    const bool cimaPila = pila.cima() != nullptr && pila.cima()->getMensaje().getId() == 2;
    test(2, "2.1 cima() retorna la accion superior o nullptr", cimaVacia && cimaPila);

    PilaAcciones pilaLimpiar;
    pilaLimpiar.apilar(a1);
    pilaLimpiar.apilar(a2);
    const bool teniaDos = pilaLimpiar.tamanio() == 2;
    pilaLimpiar.limpiar();
    const bool b2_2 = teniaDos && pilaLimpiar.vacia() && pilaLimpiar.tamanio() == 0 &&
                      pilaLimpiar.cima() == nullptr;
    test(2, "2.2 limpiar() deja la pila vacia", b2_2);

    PilaAcciones original;
    original.apilar(a1);
    original.apilar(a2);
    PilaAcciones copia = original;
    const bool copiaOk = copia.tamanio() == 2 && copia.cima() != nullptr &&
                         copia.cima()->getMensaje().getId() == 2;
    copia.apilar(a3);
    const bool independiente = original.tamanio() == 2 && copia.tamanio() == 3;
    test(2, "2.3 Copia profunda: modificar la copia no afecta a la original", copiaOk && independiente);

    PilaAcciones movible;
    movible.apilar(a1);
    movible.apilar(a2);
    PilaAcciones movida = std::move(movible);
    const bool b2_4 = movida.tamanio() == 2 && movida.cima() != nullptr &&
                      movida.cima()->getMensaje().getId() == 2 && movible.vacia();
    test(2, "2.4 Movimiento seguro de la pila (regla de cinco)", b2_4);

    PilaAcciones pilaBuscar;
    pilaBuscar.apilar(a1);
    pilaBuscar.apilar(a2);
    pilaBuscar.apilar(a3);
    const Accion* encontrada = pilaBuscar.buscarPorId(2);
    const bool b2_5 = encontrada != nullptr && encontrada->getMensaje().getId() == 2 &&
                      pilaBuscar.buscarPorId(99) == nullptr && pilaBuscar.tamanio() == 3;
    test(2, "2.5 buscarPorId() recorre la pila y retorna nullptr si no existe", b2_5);

    PilaAcciones pilaEliminar;
    pilaEliminar.apilar(a1);
    pilaEliminar.apilar(a2);
    pilaEliminar.apilar(a3);
    const bool eliminoMedio = pilaEliminar.eliminarPorId(2);
    const vector<Accion> trasEliminar = pilaEliminar.aVector();
    const bool b2_6 = eliminoMedio && pilaEliminar.tamanio() == 2 &&
                      trasEliminar.size() == 2 &&
                      trasEliminar[0].getMensaje().getId() == 3 &&
                      trasEliminar[1].getMensaje().getId() == 1 &&
                      !pilaEliminar.eliminarPorId(99);
    test(2, "2.6 eliminarPorId() elimina un nodo intermedio y retorna false si no existe", b2_6);

    PilaAcciones pilaMover;
    pilaMover.apilar(a1);
    pilaMover.apilar(a2);
    pilaMover.apilar(a3);
    const bool movio = pilaMover.moverAlCima(1);
    const vector<Accion> trasMover = pilaMover.aVector();
    const bool b2_7 = movio && pilaMover.tamanio() == 3 &&
                      trasMover[0].getMensaje().getId() == 1 &&
                      trasMover[1].getMensaje().getId() == 3 &&
                      trasMover[2].getMensaje().getId() == 2 &&
                      !pilaMover.moverAlCima(99);
    test(2, "2.7 moverAlCima() reubica un nodo intermedio sin romper la pila", b2_7);

    PilaAcciones pilaInvertir;
    pilaInvertir.apilar(a1);
    pilaInvertir.apilar(a2);
    pilaInvertir.apilar(a3);
    pilaInvertir.invertir();
    const vector<Accion> trasInvertir = pilaInvertir.aVector();
    const bool b2_8 = pilaInvertir.tamanio() == 3 &&
                      trasInvertir.size() == 3 &&
                      trasInvertir[0].getMensaje().getId() == 1 &&
                      trasInvertir[1].getMensaje().getId() == 2 &&
                      trasInvertir[2].getMensaje().getId() == 3 &&
                      pilaInvertir.cima() != nullptr &&
                      pilaInvertir.cima()->getMensaje().getId() == 1;
    test(2, "2.8 invertir() deja la base en la cima y conserva el tamanio", b2_8);

    cout << endl;

    // ------------------------------------------------------------------
    // BLOQUE 3: INTEGRACION CON MENSAJERIA
    // ------------------------------------------------------------------
    cout << "--- BLOQUE 3: Integracion con mensajeria ---" << endl;

    ChatPila chat(1, "Equipo PR2", {"u1", "u2"});
    TextMessage m1(1, "u1", "hola");
    const bool envio = chat.enviarMensaje(m1);
    const bool b3_1 = envio && chat.cantidadMensajes() == 1 &&
                      chat.getPila().tamanio() == 1 &&
                      chat.getPila().cima() != nullptr &&
                      chat.getPila().cima()->getTipo() == TipoAccion::Enviar;
    test(3, "3.1 enviarMensaje() guarda el mensaje y apila la accion Enviar", b3_1);

    const bool edito = chat.editarMensaje(1, "nuevo texto");
    const TextMessage* editado = chat.buscarMensaje(1);
    const bool b3_2a = edito && editado != nullptr && editado->getContent() == "nuevo texto" &&
                       chat.getPila().tamanio() == 2;
    const bool deshizoEdicion = chat.deshacer();
    const TextMessage* restaurado = chat.buscarMensaje(1);
    const bool b3_2b = deshizoEdicion && restaurado != nullptr &&
                       restaurado->getContent() == "hola" && chat.getPila().tamanio() == 1;
    test(3, "3.2 editarMensaje() y deshacer() restauran el texto anterior", b3_2a && b3_2b);

    const bool elimino = chat.eliminarMensaje(1);
    const bool b3_3a = elimino && chat.buscarMensaje(1) == nullptr &&
                       chat.getPila().tamanio() == 2;
    const bool deshizoEliminar = chat.deshacer();
    const TextMessage* vuelto = chat.buscarMensaje(1);
    const bool b3_3b = deshizoEliminar && vuelto != nullptr &&
                       vuelto->getContent() == "hola" && chat.getPila().tamanio() == 1;
    test(3, "3.3 eliminarMensaje() y deshacer() restauran el mensaje", b3_3a && b3_3b);

    ChatPila chatRehacer(5, "Rehacer", {"u1", "u2"});
    chatRehacer.enviarMensaje(TextMessage(1, "u1", "original"));
    chatRehacer.editarMensaje(1, "editado");
    const bool deshizoRehacerEdicion = chatRehacer.deshacer();
    const bool volvioOriginal = chatRehacer.buscarMensaje(1) != nullptr &&
                                chatRehacer.buscarMensaje(1)->getContent() == "original" &&
                                chatRehacer.getPilaRehacer().tamanio() == 1;
    const bool rehizoEdicion = chatRehacer.rehacer();
    const bool volvioEditado = chatRehacer.buscarMensaje(1) != nullptr &&
                               chatRehacer.buscarMensaje(1)->getContent() == "editado" &&
                               chatRehacer.getPilaRehacer().vacia() &&
                               chatRehacer.getPila().tamanio() == 2;
    test(3, "3.4 rehacer() reaplica una edicion deshecha", deshizoRehacerEdicion && volvioOriginal && rehizoEdicion && volvioEditado);

    ChatPila chatRehacerEliminar(6, "RehacerEliminar", {"u1", "u2"});
    chatRehacerEliminar.enviarMensaje(TextMessage(1, "u1", "hola"));
    chatRehacerEliminar.eliminarMensaje(1);
    const bool restauro = chatRehacerEliminar.deshacer();
    const bool existeTrasDeshacer = chatRehacerEliminar.buscarMensaje(1) != nullptr;
    const bool rehizoEliminar = chatRehacerEliminar.rehacer();
    const bool b3_5 = restauro && existeTrasDeshacer && rehizoEliminar &&
                      chatRehacerEliminar.buscarMensaje(1) == nullptr &&
                      chatRehacerEliminar.getPila().tamanio() == 2 &&
                      chatRehacerEliminar.getPilaRehacer().vacia();
    test(3, "3.5 rehacer() vuelve a eliminar un mensaje restaurado", b3_5);

    ChatPila chatLimpiar(7, "LimpiarRehacer", {"u1", "u2"});
    chatLimpiar.enviarMensaje(TextMessage(1, "u1", "hola"));
    chatLimpiar.editarMensaje(1, "editado");
    chatLimpiar.deshacer();
    const bool teniaRehacer = chatLimpiar.getPilaRehacer().tamanio() == 1;
    chatLimpiar.enviarMensaje(TextMessage(2, "u2", "nuevo"));
    const bool b3_6 = teniaRehacer && chatLimpiar.getPilaRehacer().vacia() &&
                      !chatLimpiar.rehacer();
    test(3, "3.6 Una accion nueva limpia la pila de rehacer", b3_6);

    cout << endl;

    // ------------------------------------------------------------------
    // BLOQUE 4: CASOS BORDE
    // ------------------------------------------------------------------
    cout << "--- BLOQUE 4: Casos borde y robustez ---" << endl;

    ChatPila chatVacio(2, "Vacio", {"u1", "u2"});
    const bool b4_1 = !chatVacio.deshacer() && chatVacio.getPila().vacia();
    test(4, "4.1 deshacer() retorna false si no hay acciones", b4_1);

    ChatPila chatEditar(3, "Editar", {"u1", "u2"});
    chatEditar.enviarMensaje(TextMessage(1, "u1", "original"));
    const bool editoNo = chatEditar.editarMensaje(99, "no existe");
    const bool b4_2 = !editoNo && chatEditar.getPila().tamanio() == 1 &&
                      chatEditar.buscarMensaje(1) != nullptr &&
                      chatEditar.buscarMensaje(1)->getContent() == "original";
    test(4, "4.2 editar un id inexistente retorna false y no apila nada", b4_2);

    ChatPila chatMulti(4, "Multi", {"u1", "u2"});
    chatMulti.enviarMensaje(TextMessage(1, "u1", "hola"));
    chatMulti.editarMensaje(1, "b");
    chatMulti.editarMensaje(1, "c");
    const bool u1 = chatMulti.deshacer();
    const bool ok1 = chatMulti.buscarMensaje(1) != nullptr &&
                     chatMulti.buscarMensaje(1)->getContent() == "b";
    const bool u2 = chatMulti.deshacer();
    const bool ok2 = chatMulti.buscarMensaje(1) != nullptr &&
                     chatMulti.buscarMensaje(1)->getContent() == "hola";
    const bool u3 = chatMulti.deshacer();
    const bool ok3 = chatMulti.buscarMensaje(1) == nullptr && chatMulti.getPila().vacia();
    test(4, "4.3 Varias acciones se deshacen en orden inverso (LIFO)", u1 && ok1 && u2 && ok2 && u3 && ok3);

    ChatPila chatSinRehacer(8, "SinRehacer", {"u1", "u2"});
    const bool b4_4 = !chatSinRehacer.rehacer() && chatSinRehacer.getPilaRehacer().vacia();
    test(4, "4.4 rehacer() retorna false si no hay acciones deshechas", b4_4);

    cout << endl;

    // ------------------------------------------------------------------
    // BLOQUE 5: PERSISTENCIA
    // ------------------------------------------------------------------
    cout << "--- BLOQUE 5: Persistencia CSV ---" << endl;

    const string ruta = "test_pila.csv";
    PilaAcciones persistente;
    persistente.apilar(a1);
    persistente.apilar(a2);
    persistente.apilar(Accion(TipoAccion::Editar, TextMessage(2, "u1", "mensaje 2"), "texto viejo"));
    const bool guardo = persistente.guardarCSV(ruta);
    PilaAcciones cargada;
    const bool cargo = cargada.cargarCSV(ruta);
    const vector<Accion> recuperadas = cargada.aVector();
    PilaAcciones inexistente;
    const bool noCarga = inexistente.cargarCSV("archivo_inexistente_12345.csv");
    const bool b5 = guardo && cargo && recuperadas.size() == 3 &&
                    recuperadas[0].getTipo() == TipoAccion::Editar &&
                    recuperadas[0].getTextoAnterior() == "texto viejo" &&
                    recuperadas[1].getMensaje().getId() == 2 &&
                    recuperadas[2].getMensaje().getId() == 1 &&
                    !noCarga && inexistente.vacia();
    std::remove(ruta.c_str());
    test(5, "5.1 Guardar y cargar conserva el orden LIFO y los datos de cada accion", b5);

    // ------------------------------------------------------------------
    // VERIFICACION MINIMA
    // ------------------------------------------------------------------
    int totalOk = 0;
    int totalChecks = 0;
    for (int b = 1; b <= 5; ++b)
    {
        totalOk += superadas[b];
        totalChecks += total[b];
    }

    cout << "\n====================================================================" << endl;
    cout << "            VERIFICACION DE REQUISITOS MINIMOS                     " << endl;
    cout << "====================================================================" << endl;
    for (int b = 1; b <= 5; ++b)
    {
        cout << "  Bloque " << b << ": " << superadas[b] << " / " << total[b]
             << " verificaciones" << endl;
    }
    cout << "  TOTAL: " << totalOk << " / " << totalChecks
         << " verificaciones superadas" << endl;
    cout << "  RESULTADO: ";
    if (totalOk == totalChecks)
    {
        cout << "CUMPLE EL MINIMO" << endl;
    }
    else
    {
        cout << "AUN NO CUMPLE EL MINIMO" << endl;
    }
    cout << "\n  Esta suite NO asigna nota." << endl;
    cout << "  La calificacion la asigna el preparador con la rubrica:" << endl;
    cout << "  resultado grupal + aporte individual evidenciado en Git." << endl;
    cout << "====================================================================" << endl;

    return 0;
}
