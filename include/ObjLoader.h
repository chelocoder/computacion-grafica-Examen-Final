#pragma once

#include <vector>
#include <string>

struct VerticeOBJ {
    float x, y, z;
};

struct CaraOBJ {
    int v1, v2, v3;
};

class ModeloOBJ {
public:
    std::vector<VerticeOBJ> vertices;
    std::vector<CaraOBJ> caras;

    bool cargar(const std::string& ruta);
    void dibujar() const;
};