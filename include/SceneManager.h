#pragma once

#include <vector>
#include "Object3D.h"

class SceneManager {
private:
    std::vector<Objeto3D> objetos;
    int siguienteId;
    int indiceSeleccionado;

    void dibujarJerarquia(int indice) const;

public:
    SceneManager();

    void agregarObjeto(TipoObjeto tipo);
    void eliminarSeleccionado();
    void duplicarSeleccionado();

    void seleccionarSiguiente();
    void seleccionarAnterior();

    Objeto3D* obtenerSeleccionado();

    void moverSeleccionado(float dx, float dy, float dz);
    void rotarSeleccionado(float rx, float ry, float rz);
    void escalarSeleccionado(float factor);

    void iniciarAnimacionSeleccionado();
    void pausarAnimacionSeleccionado();
    void resetAnimacionSeleccionado();
    void actualizarAnimaciones(float deltaTime);

    const std::vector<Objeto3D>& obtenerObjetos() const;
    int obtenerIndiceSeleccionado() const;
    void seleccionarIndice(int indice);

    void dibujarEscena() const;

    bool guardarEscena(const std::string& ruta) const;
    bool cargarEscena(const std::string& ruta);

    int cantidadObjetos() const;

    void agregarModeloOBJ(const std::string& ruta);

    void crearBrazoRobot();

    bool asignarTexturaSeleccionado(const std::string& ruta);
    void quitarTexturaSeleccionado();
};