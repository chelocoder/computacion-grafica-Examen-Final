#include <GL/glut.h>
#include <cmath>

#include "Curves.h"
#include "Lighting.h"

bool mostrarPuntosControl = false;

Punto3D P0 = {-6.5f, 0.10f, -3.8f};
Punto3D P1 = {-3.2f, 0.10f,  0.5f};
Punto3D P2 = { 2.8f, 0.10f, -0.5f};
Punto3D P3 = { 6.7f, 0.10f,  4.4f};

Punto3D calcularBezier(float t) {
    Punto3D p;
    float u = 1.0f - t;

    p.x = u * u * u * P0.x +
          3.0f * u * u * t * P1.x +
          3.0f * u * t * t * P2.x +
          t * t * t * P3.x;

    p.y = u * u * u * P0.y +
          3.0f * u * u * t * P1.y +
          3.0f * u * t * t * P2.y +
          t * t * t * P3.y;

    p.z = u * u * u * P0.z +
          3.0f * u * u * t * P1.z +
          3.0f * u * t * t * P2.z +
          t * t * t * P3.z;

    return p;
}

void calcularPerpendicular(float t, float& px, float& pz) {
    Punto3D p = calcularBezier(t);

    float t2 = t + 0.01f;

    if (t2 > 1.0f)
        t2 = 1.0f;

    Punto3D siguiente = calcularBezier(t2);

    float dx = siguiente.x - p.x;
    float dz = siguiente.z - p.z;
    float longitud = sqrt(dx * dx + dz * dz);

    if (longitud < 0.0001f) {
        px = 0.0f;
        pz = 1.0f;
        return;
    }

    dx /= longitud;
    dz /= longitud;

    px = -dz;
    pz = dx;
}

void dibujarCanalBezier() {
    const float anchoAgua = 0.80f;
    const float anchoExterior = 1.40f;

    const float alturaAgua = 0.11f;
    const float alturaBorde = 0.34f;
    const float profundidad = -0.10f;

    // Fondo
    aplicarMaterial(0.05f, 8.0f);
    glColor3f(0.25f, 0.23f, 0.20f);

    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);

    for (float t = 0.0f; t <= 1.001f; t += 0.02f) {
        Punto3D p = calcularBezier(t);

        float px, pz;
        calcularPerpendicular(t, px, pz);

        float mitad = anchoAgua / 2.0f;

        glVertex3f(p.x + px * mitad, profundidad, p.z + pz * mitad);
        glVertex3f(p.x - px * mitad, profundidad, p.z - pz * mitad);
    }

    glEnd();

    // Agua
    aplicarMaterial(0.75f, 80.0f);
    glColor3f(0.08f, 0.47f, 0.82f);

    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);

    for (float t = 0.0f; t <= 1.001f; t += 0.02f) {
        Punto3D p = calcularBezier(t);

        float px, pz;
        calcularPerpendicular(t, px, pz);

        float mitad = anchoAgua / 2.0f;

        glVertex3f(p.x + px * mitad, alturaAgua, p.z + pz * mitad);
        glVertex3f(p.x - px * mitad, alturaAgua, p.z - pz * mitad);
    }

    glEnd();

    // Pared izquierda
    aplicarMaterial(0.08f, 10.0f);
    glColor3f(0.44f, 0.41f, 0.37f);

    glBegin(GL_QUAD_STRIP);

    for (float t = 0.0f; t <= 1.001f; t += 0.02f) {
        Punto3D p = calcularBezier(t);

        float px, pz;
        calcularPerpendicular(t, px, pz);

        float interior = anchoAgua / 2.0f;
        float exterior = anchoExterior / 2.0f;

        glNormal3f(px, 0.45f, pz);

        glVertex3f(p.x + px * interior, profundidad, p.z + pz * interior);
        glVertex3f(p.x + px * exterior, alturaBorde, p.z + pz * exterior);
    }

    glEnd();

    // Pared derecha
    glBegin(GL_QUAD_STRIP);

    for (float t = 0.0f; t <= 1.001f; t += 0.02f) {
        Punto3D p = calcularBezier(t);

        float px, pz;
        calcularPerpendicular(t, px, pz);

        float interior = anchoAgua / 2.0f;
        float exterior = anchoExterior / 2.0f;

        glNormal3f(-px, 0.45f, -pz);

        glVertex3f(p.x - px * interior, profundidad, p.z - pz * interior);
        glVertex3f(p.x - px * exterior, alturaBorde, p.z - pz * exterior);
    }

    glEnd();

    // Borde superior izquierdo
    glColor3f(0.53f, 0.49f, 0.43f);

    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);

    for (float t = 0.0f; t <= 1.001f; t += 0.02f) {
        Punto3D p = calcularBezier(t);

        float px, pz;
        calcularPerpendicular(t, px, pz);

        float interior = anchoAgua / 2.0f;
        float exterior = anchoExterior / 2.0f;

        glVertex3f(p.x + px * interior, alturaBorde, p.z + pz * interior);
        glVertex3f(p.x + px * exterior, alturaBorde, p.z + pz * exterior);
    }

    glEnd();

    // Borde superior derecho
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);

    for (float t = 0.0f; t <= 1.001f; t += 0.02f) {
        Punto3D p = calcularBezier(t);

        float px, pz;
        calcularPerpendicular(t, px, pz);

        float interior = anchoAgua / 2.0f;
        float exterior = anchoExterior / 2.0f;

        glVertex3f(p.x - px * interior, alturaBorde, p.z - pz * interior);
        glVertex3f(p.x - px * exterior, alturaBorde, p.z - pz * exterior);
    }

    glEnd();

    // Puntos de control
    if (mostrarPuntosControl) {
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(11.0f);

        glBegin(GL_POINTS);
        glVertex3f(P0.x, 0.62f, P0.z);
        glVertex3f(P1.x, 0.62f, P1.z);
        glVertex3f(P2.x, 0.62f, P2.z);
        glVertex3f(P3.x, 0.62f, P3.z);
        glEnd();

        glColor3f(1.0f, 0.85f, 0.0f);
        glLineWidth(2.0f);

        glBegin(GL_LINE_STRIP);
        glVertex3f(P0.x, 0.60f, P0.z);
        glVertex3f(P1.x, 0.60f, P1.z);
        glVertex3f(P2.x, 0.60f, P2.z);
        glVertex3f(P3.x, 0.60f, P3.z);
        glEnd();

        glLineWidth(1.0f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }
}