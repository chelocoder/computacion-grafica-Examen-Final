#pragma once

#include <string>
#include "ObjLoader.h"

enum TipoObjeto {
    CUBO,
    ESFERA,
    CILINDRO,
    PLANO,
    MODELO_OBJ
};

struct Transformacion {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    float rotX = 0.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;

    float escalaX = 1.0f;
    float escalaY = 1.0f;
    float escalaZ = 1.0f;
};

struct MaterialObjeto {
    float r = 0.7f;
    float g = 0.7f;
    float b = 0.7f;

    float especular = 0.2f;
    float brillo = 30.0f;
};

class Objeto3D {
public:
    int id;
    int padreId = -1;
    
    std::string nombre;
    std::string rutaModelo;
    TipoObjeto tipo;

    Transformacion transformacion;
    MaterialObjeto material;

    bool usaTextura = false;
    unsigned int texturaId = 0;
    std::string rutaTextura;

    ModeloOBJ modelo;
    
    bool animacionActiva = false;
    bool animacionPreparada = false;
    float rotacionInicialY = 0.0f;

    Objeto3D(int id, const std::string& nombre, TipoObjeto tipo);

    void aplicarTransformacion() const;
    void dibujarGeometria() const;
    void dibujarGeometriaSeleccion() const;

    void dibujar() const;

    void dibujarSeleccion() const;
    
};