#include <GL/freeglut.h>
#include <fstream>
#include <sstream>
#include <string>

#include "ObjLoader.h"

bool ModeloOBJ::cargar(const std::string& ruta) {
    std::ifstream archivo(ruta);

    if (!archivo.is_open())
        return false;

    vertices.clear();
    caras.clear();

    std::string linea;

    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);

        std::string tipo;
        ss >> tipo;

        if (tipo == "v") {
            VerticeOBJ v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }

        else if (tipo == "f") {
            CaraOBJ cara;

            std::string a, b, c;

            ss >> a >> b >> c;

            cara.v1 = std::stoi(a) - 1;
            cara.v2 = std::stoi(b) - 1;
            cara.v3 = std::stoi(c) - 1;

            caras.push_back(cara);
        }
    }

    return true;
}

void ModeloOBJ::dibujar() const {
    glBegin(GL_TRIANGLES);

    for (const auto& cara : caras) {
        const VerticeOBJ& v1 = vertices[cara.v1];
        const VerticeOBJ& v2 = vertices[cara.v2];
        const VerticeOBJ& v3 = vertices[cara.v3];

        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
    }

    glEnd();
}