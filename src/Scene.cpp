#include <GL/glut.h>
#include <cmath>

#include "Scene.h"
#include "Curves.h"
#include "Lighting.h"
#include "Texture.h"

const float PI_ESCENA = 3.14159265f;

bool modoWireframe = false;

float anguloMonumento = 0.0f;
float rotacionEscena = 0.0f;


// ============================================================
// FUNCIONES AUXILIARES
// ============================================================

void dibujarCubo(float x, float y, float z, float sx, float sy, float sz,
                 float r, float g, float b) {
    glPushMatrix();

    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);

    glColor3f(r, g, b);
    glutSolidCube(1.0f);

    glPopMatrix();
}

void dibujarDisco(float x, float y, float z, float radio,
                  float r, float g, float b) {
    glColor3f(r, g, b);

    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x, y, z);

    for (int i = 0; i <= 40; i++) {
        float angulo = 2.0f * PI_ESCENA * i / 40.0f;

        glVertex3f(
            x + cos(angulo) * radio,
            y,
            z + sin(angulo) * radio
        );
    }

    glEnd();
}


// ============================================================
// TERRENO
// ============================================================

void dibujarTerreno() {
    aplicarMaterial(0.05f, 8.0f);

    glColor3f(0.28f, 0.52f, 0.23f);

    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-15.0f, 0.0f, -12.0f);
    glVertex3f( 15.0f, 0.0f, -12.0f);
    glVertex3f( 15.0f, 0.0f,  12.0f);
    glVertex3f(-15.0f, 0.0f,  12.0f);

    glEnd();
}


// ============================================================
// CAMINO Y PLAZA
// ============================================================

void dibujarCamino() {
    aplicarMaterial(0.08f, 10.0f);

    glColor3f(0.56f, 0.45f, 0.31f);

    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);

    glVertex3f(-2.6f, 0.025f, -12.0f);
    glVertex3f( 2.6f, 0.025f, -12.0f);
    glVertex3f( 2.0f, 0.025f,   3.0f);
    glVertex3f(-2.0f, 0.025f,   3.0f);

    glEnd();

    dibujarDisco(0.0f, 0.03f, 4.3f, 4.3f, 0.48f, 0.43f, 0.36f);
}


// ============================================================
// SUPERFICIES TEXTURIZADAS
// ============================================================

void dibujarSuperficieTexturizadaFrontal(float x1, float y1, float x2, float y2, float z) {
    glNormal3f(0.0f, 0.0f, 1.0f);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x1, y1, z);

    glTexCoord2f(2.0f, 0.0f);
    glVertex3f(x2, y1, z);

    glTexCoord2f(2.0f, 2.0f);
    glVertex3f(x2, y2, z);

    glTexCoord2f(0.0f, 2.0f);
    glVertex3f(x1, y2, z);

    glEnd();
}

void dibujarSuperficieTexturizadaPosterior(float x1, float y1, float x2, float y2, float z) {
    glNormal3f(0.0f, 0.0f, -1.0f);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x2, y1, z);

    glTexCoord2f(2.0f, 0.0f);
    glVertex3f(x1, y1, z);

    glTexCoord2f(2.0f, 2.0f);
    glVertex3f(x1, y2, z);

    glTexCoord2f(0.0f, 2.0f);
    glVertex3f(x2, y2, z);

    glEnd();
}


// ============================================================
// PORTAL
// ============================================================

void dibujarPortal() {
    aplicarMaterial(0.10f, 12.0f);

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -8.5f);

    for (int i = 0; i < 4; i++) {
        dibujarCubo(
            -2.3f, 0.55f + i * 1.05f, 0.0f,
            1.25f, 1.0f, 1.3f,
            0.46f, 0.43f, 0.39f
        );

        dibujarCubo(
             2.3f, 0.55f + i * 1.05f, 0.0f,
             1.25f, 1.0f, 1.3f,
             0.43f, 0.41f, 0.37f
        );
    }

    dibujarCubo(
        0.0f, 4.55f, 0.0f,
        5.8f, 1.0f, 1.4f,
        0.42f, 0.39f, 0.35f
    );

    if (texturaActiva && texturaPiedra != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaPiedra);

        glColor3f(1.0f, 1.0f, 1.0f);

        // Cara frontal
        dibujarSuperficieTexturizadaFrontal(
            -2.925f, 0.05f,
            -1.675f, 4.05f,
            0.655f
        );

        dibujarSuperficieTexturizadaFrontal(
            1.675f, 0.05f,
            2.925f, 4.05f,
            0.655f
        );

        dibujarSuperficieTexturizadaFrontal(
            -2.9f, 4.05f,
            2.9f, 5.05f,
            0.705f
        );

        // Cara posterior
        dibujarSuperficieTexturizadaPosterior(
            -2.925f, 0.05f,
            -1.675f, 4.05f,
            -0.655f
        );

        dibujarSuperficieTexturizadaPosterior(
            1.675f, 0.05f,
            2.925f, 4.05f,
            -0.655f
        );

        dibujarSuperficieTexturizadaPosterior(
            -2.9f, 4.05f,
            2.9f, 5.05f,
            -0.705f
        );

        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}


// ============================================================
// MURO DE PIEDRA
// ============================================================

void dibujarPiedra(float x, float y, float z, float variacion) {
    glPushMatrix();

    glTranslatef(x, y, z);
    glScalef(1.25f, 0.62f, 0.85f);

    glColor3f(
        0.40f + variacion,
        0.39f + variacion,
        0.36f + variacion
    );

    glutSolidCube(1.0f);

    glPopMatrix();
}

void dibujarMuroPiedra() {
    aplicarMaterial(0.05f, 8.0f);

    glPushMatrix();

    glTranslatef(-8.0f, 0.0f, 2.3f);

    for (int fila = 0; fila < 4; fila++) {
        for (int columna = 0; columna < 6; columna++) {
            float desplazamiento = (fila % 2 == 0) ? 0.0f : 0.55f;
            float variacion = ((fila + columna) % 3) * 0.025f;

            dibujarPiedra(
                columna * 1.12f + desplazamiento,
                0.36f + fila * 0.62f,
                0.0f,
                variacion
            );
        }
    }

    glPopMatrix();
}


// ============================================================
// BANCA
// ============================================================

void dibujarBanca(float x, float z, float angulo) {
    aplicarMaterial(0.18f, 22.0f);

    glPushMatrix();

    glTranslatef(x, 0.0f, z);
    glRotatef(angulo, 0.0f, 1.0f, 0.0f);

    dibujarCubo(
        0.0f, 0.75f, 0.0f,
        2.8f, 0.22f, 0.75f,
        0.42f, 0.22f, 0.09f
    );

    dibujarCubo(
        0.0f, 1.45f, 0.32f,
        2.8f, 1.0f, 0.18f,
        0.42f, 0.22f, 0.09f
    );

    dibujarCubo(
        -0.95f, 0.35f, 0.0f,
        0.20f, 0.7f, 0.20f,
        0.24f, 0.16f, 0.10f
    );

    dibujarCubo(
         0.95f, 0.35f, 0.0f,
         0.20f, 0.7f, 0.20f,
         0.24f, 0.16f, 0.10f
    );

    glPopMatrix();
}


// ============================================================
// ARBOL
// ============================================================

void dibujarArbol(float x, float z, float escala) {
    glPushMatrix();

    glTranslatef(x, 0.0f, z);
    glScalef(escala, escala, escala);

    aplicarMaterial(0.08f, 8.0f);

    dibujarCubo(
        0.0f, 1.3f, 0.0f,
        0.42f, 2.6f, 0.42f,
        0.33f, 0.18f, 0.07f
    );

    aplicarMaterial(0.03f, 4.0f);

    glColor3f(0.08f, 0.40f, 0.10f);

    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f);
    glutSolidSphere(1.15f, 18, 18);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.6f, 2.85f, 0.1f);
    glutSolidSphere(0.75f, 16, 16);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.55f, 2.85f, 0.15f);
    glutSolidSphere(0.72f, 16, 16);
    glPopMatrix();

    glPopMatrix();
}


// ============================================================
// MATE - SUPERFICIE DE REVOLUCION
// ============================================================

struct PuntoPerfil {
    float radio;
    float altura;
};

PuntoPerfil perfilMate[] = {
    {0.00f, 0.00f},
    {0.70f, 0.10f},
    {1.10f, 0.45f},
    {1.40f, 1.20f},
    {1.45f, 1.70f},
    {1.30f, 2.30f},
    {1.00f, 2.85f},
    {0.78f, 3.15f},
    {0.78f, 3.50f},
    {0.95f, 3.60f},
    {0.00f, 3.60f}
};

const int puntosPerfil = sizeof(perfilMate) / sizeof(perfilMate[0]);

void dibujarMateBurilado() {
    aplicarMaterial(0.45f, 55.0f);

    glPushMatrix();

    glTranslatef(-6.2f, 0.30f, -4.4f);
    glScalef(0.90f, 0.90f, 0.90f);

    const int segmentos = 32;

    if (modoWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    for (int i = 0; i < puntosPerfil - 1; i++) {
        glBegin(GL_QUAD_STRIP);

        for (int j = 0; j <= segmentos; j++) {
            float angulo = 2.0f * PI_ESCENA * j / segmentos;

            float cosA = cos(angulo);
            float sinA = sin(angulo);

            float r1 = perfilMate[i].radio;
            float y1 = perfilMate[i].altura;

            float r2 = perfilMate[i + 1].radio;
            float y2 = perfilMate[i + 1].altura;

            float tono = (i % 2 == 0) ? 0.0f : 0.04f;

            glColor3f(
                0.72f + tono,
                0.38f + tono,
                0.10f
            );

            glNormal3f(cosA, 0.0f, sinA);

            glVertex3f(r1 * cosA, y1, r1 * sinA);
            glVertex3f(r2 * cosA, y2, r2 * sinA);
        }

        glEnd();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPopMatrix();
}


// ============================================================
// MONUMENTO JERARQUICO
// ============================================================

void dibujarMonumentoJerarquico() {
    aplicarMaterial(0.20f, 28.0f);

    glPushMatrix();

    glTranslatef(5.7f, 0.0f, 5.0f);

    // Esta rotacion afecta al conjunto completo
    glRotatef(anguloMonumento, 0.0f, 1.0f, 0.0f);

    dibujarCubo(
        0.0f, 0.35f, 0.0f,
        4.0f, 0.7f, 3.3f,
        0.31f, 0.30f, 0.29f
    );

    dibujarCubo(
        0.0f, 1.05f, 0.0f,
        2.4f, 1.4f, 2.0f,
        0.47f, 0.44f, 0.40f
    );

    glPushMatrix();

    glTranslatef(0.0f, 2.25f, 0.0f);

    dibujarCubo(
        0.0f, 0.0f, 0.0f,
        1.0f, 2.6f, 0.9f,
        0.56f, 0.52f, 0.46f
    );

    dibujarCubo(
        0.75f, 0.45f, 0.0f,
        1.6f, 0.35f, 0.40f,
        0.62f, 0.45f, 0.16f
    );

    aplicarMaterial(0.60f, 80.0f);

    glPushMatrix();

    glTranslatef(0.20f, 1.75f, 0.0f);

    glColor3f(0.72f, 0.50f, 0.17f);
    glutSolidSphere(0.72f, 20, 20);

    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}


// ============================================================
// ESCALINATA
// ============================================================

void dibujarEscalinata() {
    aplicarMaterial(0.08f, 10.0f);

    glPushMatrix();

    glTranslatef(-7.0f, 0.0f, 7.5f);

    for (int i = 0; i < 5; i++) {
        float altura = 0.22f + i * 0.22f;

        dibujarCubo(
            0.0f,
            altura / 2.0f,
            i * 0.55f,
            3.2f,
            altura,
            1.0f,
            0.46f,
            0.43f,
            0.39f
        );
    }

    glPopMatrix();
}


// ============================================================
// FAROL
// ============================================================

void dibujarFarol(float x, float z) {
    glPushMatrix();

    glTranslatef(x, 0.0f, z);

    aplicarMaterial(0.30f, 40.0f);

    dibujarCubo(
        0.0f, 1.45f, 0.0f,
        0.13f, 2.9f, 0.13f,
        0.16f, 0.16f, 0.16f
    );

    aplicarMaterial(0.75f, 90.0f);

    glPushMatrix();

    glTranslatef(0.0f, 3.0f, 0.0f);

    glColor3f(1.0f, 0.78f, 0.20f);
    glutSolidSphere(0.32f, 16, 16);

    glPopMatrix();
    glPopMatrix();
}


// ============================================================
// JARDIN
// ============================================================

void dibujarJardin(float x, float z, float radio) {
    aplicarMaterial(0.05f, 6.0f);

    dibujarDisco(
        x, 0.035f, z, radio,
        0.16f, 0.40f, 0.13f
    );

    aplicarMaterial(0.18f, 20.0f);

    glColor3f(0.75f, 0.18f, 0.25f);

    for (int i = 0; i < 6; i++) {
        float angulo = 2.0f * PI_ESCENA * i / 6.0f;

        glPushMatrix();

        glTranslatef(
            x + cos(angulo) * radio * 0.55f,
            0.14f,
            z + sin(angulo) * radio * 0.55f
        );

        glutSolidSphere(0.14f, 10, 10);

        glPopMatrix();
    }
}


// ============================================================
// ESCENA COMPLETA
// ============================================================

void dibujarEscena() {
    glPushMatrix();

    glRotatef(rotacionEscena, 0.0f, 1.0f, 0.0f);

    dibujarTerreno();
    dibujarCamino();
    dibujarPortal();
    dibujarMuroPiedra();

    dibujarCanalBezier();

    dibujarMateBurilado();
    dibujarMonumentoJerarquico();
    dibujarEscalinata();

    // Bancas
    dibujarBanca(-8.0f, -2.8f, 25.0f);
    dibujarBanca(6.5f, 1.6f, -28.0f);

    // Arboles
    dibujarArbol(-10.0f, -5.0f, 1.0f);
    dibujarArbol(9.5f, -3.0f, 1.1f);
    dibujarArbol(-10.5f, 7.0f, 0.90f);
    dibujarArbol(9.3f, 8.0f, 1.0f);

    // Faroles
    dibujarFarol(-3.3f, -5.0f);
    dibujarFarol(3.3f, -3.6f);
    dibujarFarol(-3.3f, 3.0f);
    dibujarFarol(3.2f, 4.0f);

    // Jardines
    dibujarJardin(-9.0f, 4.5f, 1.7f);
    dibujarJardin(9.0f, 2.5f, 1.6f);

    glPopMatrix();
}