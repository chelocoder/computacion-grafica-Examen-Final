#pragma once

struct Punto3D {
    float x;
    float y;
    float z;
};

extern bool mostrarPuntosControl;

Punto3D calcularBezier(float t);
void calcularPerpendicular(float t, float& px, float& pz);
void dibujarCanalBezier();