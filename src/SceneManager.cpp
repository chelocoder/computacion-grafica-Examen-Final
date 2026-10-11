#include <GL/freeglut.h>

#include <cstdio>
#include <string>
#include <fstream>

#include "SceneManager.h"
#include "Texture.h"

SceneManager::SceneManager() {
    siguienteId = 1;
    indiceSeleccionado = -1;
}

void SceneManager::agregarObjeto(TipoObjeto tipo) {
    std::string nombre;

    switch (tipo) {
        case CUBO:
            nombre = "Cubo_";
            break;

        case ESFERA:
            nombre = "Esfera_";
            break;

        case CILINDRO:
            nombre = "Cilindro_";
            break;

        case PLANO:
            nombre = "Plano_";
            break;
    }

    nombre += std::to_string(siguienteId);

    objetos.emplace_back(
        siguienteId,
        nombre,
        tipo
    );

    indiceSeleccionado = objetos.size() - 1;

    printf("Objeto creado: %s\n", nombre.c_str());

    siguienteId++;
}

void SceneManager::eliminarSeleccionado() {
    if (indiceSeleccionado < 0 || objetos.empty())
        return;

    printf(
        "Objeto eliminado: %s\n",
        objetos[indiceSeleccionado].nombre.c_str()
    );

    objetos.erase(objetos.begin() + indiceSeleccionado);

    if (objetos.empty()) {
        indiceSeleccionado = -1;
    } else if (indiceSeleccionado >= (int)objetos.size()) {
        indiceSeleccionado = objetos.size() - 1;
    }
}

void SceneManager::duplicarSeleccionado() {
    if (indiceSeleccionado < 0 || objetos.empty())
        return;

    Objeto3D copia = objetos[indiceSeleccionado];

    copia.id = siguienteId;
    copia.nombre += "_copia";

    copia.transformacion.x += 1.0f;

    objetos.push_back(copia);

    indiceSeleccionado = objetos.size() - 1;

    printf(
        "Objeto duplicado: %s\n",
        copia.nombre.c_str()
    );

    siguienteId++;
}

void SceneManager::seleccionarSiguiente() {
    if (objetos.empty())
        return;

    indiceSeleccionado++;

    if (indiceSeleccionado >= (int)objetos.size())
        indiceSeleccionado = 0;

    printf(
        "Seleccionado: %s\n",
        objetos[indiceSeleccionado].nombre.c_str()
    );
}

void SceneManager::seleccionarAnterior() {
    if (objetos.empty())
        return;

    indiceSeleccionado--;

    if (indiceSeleccionado < 0)
        indiceSeleccionado = objetos.size() - 1;

    printf(
        "Seleccionado: %s\n",
        objetos[indiceSeleccionado].nombre.c_str()
    );
}

Objeto3D* SceneManager::obtenerSeleccionado() {
    if (indiceSeleccionado < 0 || objetos.empty())
        return nullptr;

    return &objetos[indiceSeleccionado];
}

void SceneManager::dibujarJerarquia(int indice) const {
    const Objeto3D& objeto = objetos[indice];

    glPushMatrix();

    objeto.aplicarTransformacion();
    objeto.dibujarGeometria();

    if (indice == indiceSeleccionado)
        objeto.dibujarGeometriaSeleccion();

    for (int i = 0; i < (int)objetos.size(); i++) {
        if (objetos[i].padreId == objeto.id)
            dibujarJerarquia(i);
    }

    glPopMatrix();
}

void SceneManager::dibujarEscena() const {
    for (int i = 0; i < (int)objetos.size(); i++) {
        if (objetos[i].padreId == -1)
            dibujarJerarquia(i);
    }
}

int SceneManager::cantidadObjetos() const {
    return objetos.size();
}

void SceneManager::moverSeleccionado(float dx, float dy, float dz) {
    Objeto3D* objeto = obtenerSeleccionado();

    if (!objeto)
        return;

    objeto->transformacion.x += dx;
    objeto->transformacion.y += dy;
    objeto->transformacion.z += dz;

    printf(
        "%s - Posicion: %.2f %.2f %.2f\n",
        objeto->nombre.c_str(),
        objeto->transformacion.x,
        objeto->transformacion.y,
        objeto->transformacion.z
    );
}

void SceneManager::rotarSeleccionado(float rx, float ry, float rz) {
    Objeto3D* objeto = obtenerSeleccionado();

    if (!objeto)
        return;

    objeto->transformacion.rotX += rx;
    objeto->transformacion.rotY += ry;
    objeto->transformacion.rotZ += rz;

    printf(
        "%s - Rotacion: %.2f %.2f %.2f\n",
        objeto->nombre.c_str(),
        objeto->transformacion.rotX,
        objeto->transformacion.rotY,
        objeto->transformacion.rotZ
    );
}

void SceneManager::escalarSeleccionado(float factor) {
    Objeto3D* objeto = obtenerSeleccionado();

    if (!objeto)
        return;

    objeto->transformacion.escalaX *= factor;
    objeto->transformacion.escalaY *= factor;
    objeto->transformacion.escalaZ *= factor;

    printf(
        "%s - Escala: %.2f %.2f %.2f\n",
        objeto->nombre.c_str(),
        objeto->transformacion.escalaX,
        objeto->transformacion.escalaY,
        objeto->transformacion.escalaZ
    );
}

const std::vector<Objeto3D>& SceneManager::obtenerObjetos() const {
    return objetos;
}

int SceneManager::obtenerIndiceSeleccionado() const {
    return indiceSeleccionado;
}

void SceneManager::seleccionarIndice(int indice) {
    if (indice >= 0 && indice < (int)objetos.size())
        indiceSeleccionado = indice;
}

void SceneManager::agregarModeloOBJ(const std::string& ruta) {
    std::string nombre = "ModeloOBJ_" + std::to_string(siguienteId);

    objetos.emplace_back(
        siguienteId,
        nombre,
        MODELO_OBJ
    );

    indiceSeleccionado = objetos.size() - 1;

    if (!objetos[indiceSeleccionado].modelo.cargar(ruta)) {
        printf("Error al cargar OBJ: %s\n", ruta.c_str());

        objetos.pop_back();
        indiceSeleccionado = objetos.empty()
            ? -1
            : objetos.size() - 1;

        return;
    }

    objetos[indiceSeleccionado].rutaModelo = ruta;

    printf("OBJ cargado: %s\n", ruta.c_str());

    siguienteId++;
}

bool SceneManager::guardarEscena(const std::string& ruta) const {
    std::ofstream archivo(ruta);

    if (!archivo.is_open()) {
        printf("No se pudo guardar la escena.\n");
        return false;
    }

    archivo << objetos.size() << "\n";

    for (const auto& objeto : objetos) {
        archivo
            << objeto.id << " "
            << (int)objeto.tipo << " "
            << objeto.nombre << " "
            << (objeto.rutaModelo.empty() ? "-" : objeto.rutaModelo) << " "

            << objeto.transformacion.x << " "
            << objeto.transformacion.y << " "
            << objeto.transformacion.z << " "

            << objeto.transformacion.rotX << " "
            << objeto.transformacion.rotY << " "
            << objeto.transformacion.rotZ << " "

            << objeto.transformacion.escalaX << " "
            << objeto.transformacion.escalaY << " "
            << objeto.transformacion.escalaZ << " "

            << objeto.material.r << " "
            << objeto.material.g << " "
            << objeto.material.b << " "

            << objeto.material.especular << " "
            << objeto.material.brillo << " "

            << objeto.usaTextura << " "
            << (objeto.rutaTextura.empty() ? "-" : objeto.rutaTextura)

            << "\n";
    }

    printf("Escena guardada: %s\n", ruta.c_str());

    return true;
}

bool SceneManager::cargarEscena(const std::string& ruta) {
    std::ifstream archivo(ruta);

    bool usaTextura;
    std::string rutaTextura;

    if (!archivo.is_open()) {
        printf("No se pudo cargar la escena.\n");
        return false;
    }

    objetos.clear();
    indiceSeleccionado = -1;
    siguienteId = 1;

    int cantidad;
    archivo >> cantidad;

    for (int i = 0; i < cantidad; i++) {
        int id;
        int tipoInt;

        std::string nombre;
        std::string rutaModelo;

        Transformacion t;
        MaterialObjeto m;

        archivo
            >> id
            >> tipoInt
            >> nombre
            >> rutaModelo

            >> t.x
            >> t.y
            >> t.z

            >> t.rotX
            >> t.rotY
            >> t.rotZ

            >> t.escalaX
            >> t.escalaY
            >> t.escalaZ

            >> m.r
            >> m.g
            >> m.b

            >> m.especular
            >> m.brillo

            >> usaTextura
            >> rutaTextura;

        TipoObjeto tipo = (TipoObjeto)tipoInt;

        objetos.emplace_back(
            id,
            nombre,
            tipo
        );

        Objeto3D& objeto = objetos.back();

        objeto.transformacion = t;
        objeto.material = m;

        if (usaTextura && rutaTextura != "-") {
            GLuint textura = 0;

            if (cargarTexturaBMP(rutaTextura.c_str(), textura)) {
                objeto.texturaId = textura;
                objeto.rutaTextura = rutaTextura;
                objeto.usaTextura = true;

                printf(
                    "Textura restaurada: %s\n",
                    rutaTextura.c_str()
                );
            }
        }

        if (tipo == MODELO_OBJ && rutaModelo != "-") {
            objeto.rutaModelo = rutaModelo;

            if (objeto.modelo.cargar(rutaModelo)) {
                printf("OBJ restaurado: %s\n", rutaModelo.c_str());
            }
            else {
                printf("ERROR restaurando OBJ: %s\n", rutaModelo.c_str());
            }
        }

        if (id >= siguienteId)
            siguienteId = id + 1;
    }

    if (!objetos.empty())
        indiceSeleccionado = 0;

    printf("Escena cargada: %s\n", ruta.c_str());

    return true;
}

void SceneManager::iniciarAnimacionSeleccionado() {
    Objeto3D* objeto = obtenerSeleccionado();

    if (!objeto)
        return;

    if (!objeto->animacionPreparada) {
        objeto->rotacionInicialY =
            objeto->transformacion.rotY;

        objeto->animacionPreparada = true;
    }

    objeto->animacionActiva = true;

    printf(
        "Animacion iniciada: %s\n",
        objeto->nombre.c_str()
    );
}

void SceneManager::pausarAnimacionSeleccionado() {
    Objeto3D* objeto = obtenerSeleccionado();

    if (!objeto)
        return;

    objeto->animacionActiva = false;

    printf(
        "Animacion pausada: %s\n",
        objeto->nombre.c_str()
    );
}

void SceneManager::resetAnimacionSeleccionado() {
    Objeto3D* objeto = obtenerSeleccionado();

    if (!objeto)
        return;

    objeto->animacionActiva = false;

    if (objeto->animacionPreparada) {
        objeto->transformacion.rotY =
            objeto->rotacionInicialY;
    }

    objeto->animacionPreparada = false;

    printf(
        "Animacion reiniciada: %s\n",
        objeto->nombre.c_str()
    );
}

void SceneManager::actualizarAnimaciones(float deltaTime) {
    for (auto& objeto : objetos) {
        if (objeto.animacionActiva) {
            objeto.transformacion.rotY +=
                45.0f * deltaTime;

            if (objeto.transformacion.rotY >= 360.0f)
                objeto.transformacion.rotY -= 360.0f;
        }
    }
}

void SceneManager::crearBrazoRobot() {
    int idBase = siguienteId;

    objetos.emplace_back(
        siguienteId++,
        "Robot_Base",
        CUBO
    );

    Objeto3D& base = objetos.back();
    base.transformacion.x = 0.0f;
    base.transformacion.y = 0.5f;
    base.transformacion.z = 0.0f;

    base.material.r = 0.25f;
    base.material.g = 0.35f;
    base.material.b = 0.75f;

    int idBrazo = siguienteId;

    objetos.emplace_back(
        siguienteId++,
        "Robot_Brazo",
        CUBO
    );

    Objeto3D& brazo = objetos.back();

    brazo.padreId = idBase;

    brazo.transformacion.x = 0.0f;
    brazo.transformacion.y = 1.2f;
    brazo.transformacion.z = 0.0f;

    brazo.transformacion.escalaX = 0.45f;
    brazo.transformacion.escalaY = 1.8f;
    brazo.transformacion.escalaZ = 0.45f;

    brazo.material.r = 0.80f;
    brazo.material.g = 0.25f;
    brazo.material.b = 0.20f;

    objetos.emplace_back(
        siguienteId++,
        "Robot_Antebrazo",
        CUBO
    );

    Objeto3D& antebrazo = objetos.back();

    antebrazo.padreId = idBrazo;

    antebrazo.transformacion.x = 0.0f;
    antebrazo.transformacion.y = 1.0f;
    antebrazo.transformacion.z = 0.0f;

    antebrazo.transformacion.escalaX = 0.70f;
    antebrazo.transformacion.escalaY = 0.80f;
    antebrazo.transformacion.escalaZ = 0.70f;

    antebrazo.material.r = 0.20f;
    antebrazo.material.g = 0.75f;
    antebrazo.material.b = 0.30f;

    indiceSeleccionado =
        (int)objetos.size() - 2;

    printf("Brazo robotico FK creado.\n");
}

bool SceneManager::asignarTexturaSeleccionado(const std::string& ruta) {
    Objeto3D* objeto = obtenerSeleccionado();

    if (!objeto)
        return false;

    GLuint nuevaTextura = 0;

    if (!cargarTexturaBMP(ruta.c_str(), nuevaTextura)) {
        printf("Error cargando textura: %s\n", ruta.c_str());
        return false;
    }

    objeto->texturaId = nuevaTextura;
    objeto->rutaTextura = ruta;
    objeto->usaTextura = true;

    printf(
        "Textura asignada a %s: %s\n",
        objeto->nombre.c_str(),
        ruta.c_str()
    );

    return true;
}

void SceneManager::quitarTexturaSeleccionado() {
    Objeto3D* objeto = obtenerSeleccionado();

    if (!objeto)
        return;

    objeto->usaTextura = false;

    printf(
        "Textura desactivada: %s\n",
        objeto->nombre.c_str()
    );
}