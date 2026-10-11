#include <GL/glut.h>
#include <cmath>

#include "Lighting.h"

const float PI_LUZ = 3.14159265f;

bool luzPuntualActiva = true;
bool luzDireccionalActiva = true;
bool animacionLuzActiva = false;

float intensidadLuzPrincipal = 1.0f;

float anguloLuz = 35.0f;
float velocidadLuz = 1.0f;

float posicionLuzX = 5.0f;
float posicionLuzY = 5.5f;
float posicionLuzZ = 4.0f;

void inicializarIluminacion() {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(
        GL_FRONT_AND_BACK,
        GL_AMBIENT_AND_DIFFUSE
    );

    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    GLfloat ambienteGlobal[] = {
        0.24f,
        0.24f,
        0.24f,
        1.0f
    };

    glLightModelfv(
        GL_LIGHT_MODEL_AMBIENT,
        ambienteGlobal
    );
}

void actualizarPosicionLuz() {
    float radianes =
        anguloLuz * PI_LUZ / 180.0f;

    const float radio = 8.0f;

    posicionLuzX =
        radio * cos(radianes);

    posicionLuzY = 5.5f;

    posicionLuzZ =
        radio * sin(radianes);
}

void configurarIluminacion() {
    actualizarPosicionLuz();

    if (luzPuntualActiva)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);

    if (luzDireccionalActiva)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);

    GLfloat posicionLuz0[] = {
        posicionLuzX,
        posicionLuzY,
        posicionLuzZ,
        1.0f
    };

    GLfloat ambienteLuz0[] = {
        0.10f * intensidadLuzPrincipal,
        0.10f * intensidadLuzPrincipal,
        0.08f * intensidadLuzPrincipal,
        1.0f
    };

    GLfloat difusaLuz0[] = {
        0.95f * intensidadLuzPrincipal,
        0.90f * intensidadLuzPrincipal,
        0.80f * intensidadLuzPrincipal,
        1.0f
    };

    GLfloat especularLuz0[] = {
        1.0f * intensidadLuzPrincipal,
        1.0f * intensidadLuzPrincipal,
        1.0f * intensidadLuzPrincipal,
        1.0f
    };

    glLightfv(
        GL_LIGHT0,
        GL_POSITION,
        posicionLuz0
    );

    glLightfv(
        GL_LIGHT0,
        GL_AMBIENT,
        ambienteLuz0
    );

    glLightfv(
        GL_LIGHT0,
        GL_DIFFUSE,
        difusaLuz0
    );

    glLightfv(
        GL_LIGHT0,
        GL_SPECULAR,
        especularLuz0
    );

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.04f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.004f);

    GLfloat direccionLuz1[] = {
        -1.0f,
        -1.0f,
        -0.5f,
        0.0f
    };

    GLfloat ambienteLuz1[] = {
        0.02f,
        0.02f,
        0.04f,
        1.0f
    };

    GLfloat difusaLuz1[] = {
        0.30f,
        0.35f,
        0.45f,
        1.0f
    };

    GLfloat especularLuz1[] = {
        0.35f,
        0.35f,
        0.40f,
        1.0f
    };

    glLightfv(
        GL_LIGHT1,
        GL_POSITION,
        direccionLuz1
    );

    glLightfv(
        GL_LIGHT1,
        GL_AMBIENT,
        ambienteLuz1
    );

    glLightfv(
        GL_LIGHT1,
        GL_DIFFUSE,
        difusaLuz1
    );

    glLightfv(
        GL_LIGHT1,
        GL_SPECULAR,
        especularLuz1
    );
}

void dibujarIndicadorLuz() {
    if (!luzPuntualActiva)
        return;

    glPushAttrib(
        GL_ENABLE_BIT |
        GL_CURRENT_BIT
    );

    glDisable(GL_LIGHTING);

    glColor3f(
        1.0f,
        0.90f,
        0.20f
    );

    glPushMatrix();

    glTranslatef(
        posicionLuzX,
        posicionLuzY,
        posicionLuzZ
    );

    glutSolidSphere(
        0.28f,
        16,
        16
    );

    glPopMatrix();

    glPopAttrib();
}

void aplicarMaterial(
    float especular,
    float brillo
) {
    GLfloat componenteEspecular[] = {
        especular,
        especular,
        especular,
        1.0f
    };

    glMaterialfv(
        GL_FRONT_AND_BACK,
        GL_SPECULAR,
        componenteEspecular
    );

    glMaterialf(
        GL_FRONT_AND_BACK,
        GL_SHININESS,
        brillo
    );
}