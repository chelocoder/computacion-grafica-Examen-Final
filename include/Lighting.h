#pragma once

extern bool luzPuntualActiva;
extern bool luzDireccionalActiva;
extern bool animacionLuzActiva;

extern float intensidadLuzPrincipal;
extern float anguloLuz;
extern float velocidadLuz;

extern float posicionLuzX;
extern float posicionLuzY;
extern float posicionLuzZ;

void inicializarIluminacion();
void configurarIluminacion();
void aplicarMaterial(float especular, float brillo);

void actualizarPosicionLuz();
void dibujarIndicadorLuz();