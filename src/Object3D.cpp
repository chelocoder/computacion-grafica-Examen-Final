#include <GL/freeglut.h>
#include "Object3D.h"
#include "Lighting.h"

Objeto3D::Objeto3D(int id, const std::string& nombre, TipoObjeto tipo)
    : id(id), nombre(nombre), tipo(tipo) {
}

void Objeto3D::dibujar() const {
    glPushMatrix();

    aplicarTransformacion();
    dibujarGeometria();

    glPopMatrix();
}

void Objeto3D::dibujarSeleccion() const {
    glPushMatrix();

    aplicarTransformacion();
    dibujarGeometriaSeleccion();

    glPopMatrix();
}

void Objeto3D::aplicarTransformacion() const {
    glTranslatef(
        transformacion.x,
        transformacion.y,
        transformacion.z
    );

    glRotatef(transformacion.rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(transformacion.rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(transformacion.rotZ, 0.0f, 0.0f, 1.0f);

    glScalef(
        transformacion.escalaX,
        transformacion.escalaY,
        transformacion.escalaZ
    );
}

void Objeto3D::dibujarGeometria() const {
    glColor3f(
        material.r,
        material.g,
        material.b
    );

    aplicarMaterial(material.especular, material.brillo);

    switch (tipo) {
        case CUBO:
            glutSolidCube(1.0);
            break;

        case ESFERA:
            glutSolidSphere(0.6, 24, 24);
            break;

        case CILINDRO:
            glutSolidCylinder(0.5, 1.2, 24, 8);
            break;

        case PLANO:
            if (usaTextura && texturaId != 0) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texturaId);
                glColor3f(1.0f, 1.0f, 1.0f);
            }

            glBegin(GL_QUADS);

            glNormal3f(0.0f, 1.0f, 0.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f, 0.0f, -1.0f);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1.0f, 0.0f, 1.0f);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(1.0f, 0.0f, 1.0f);

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1.0f, 0.0f, -1.0f);

            glEnd();

            if (usaTextura) {
                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_TEXTURE_2D);
            }

            break;

        case MODELO_OBJ:
            modelo.dibujar();
            break;
    }
}

void Objeto3D::dibujarGeometriaSeleccion() const {
    glPushAttrib(
        GL_ENABLE_BIT |
        GL_POLYGON_BIT |
        GL_LINE_BIT |
        GL_CURRENT_BIT
    );

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(3.0f);
    glColor3f(1.0f, 1.0f, 0.0f);

    glScalef(1.03f, 1.03f, 1.03f);

    switch (tipo) {
        case CUBO:
            glutWireCube(1.0);
            break;

        case ESFERA:
            glutWireSphere(0.6, 24, 24);
            break;

        case CILINDRO:
            glutWireCylinder(0.5, 1.2, 24, 8);
            break;

        case PLANO:
            glBegin(GL_LINE_LOOP);
            glVertex3f(-1.0f, 0.01f, -1.0f);
            glVertex3f(-1.0f, 0.01f,  1.0f);
            glVertex3f( 1.0f, 0.01f,  1.0f);
            glVertex3f( 1.0f, 0.01f, -1.0f);
            glEnd();
            break;

        case MODELO_OBJ:
            modelo.dibujar();
            break;
    }

    glPopAttrib();
}

