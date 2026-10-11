#pragma once

#include <GL/glut.h>

extern bool texturaActiva;
extern GLuint texturaPiedra;

bool cargarTexturaBMP(const char* ruta, GLuint& textura);
void inicializarTexturas();