# Editor de Escenas 3D - Examen Final

Proyecto desarrollado para el curso de Computación Gráfica de la Universidad Continental.

El sistema implementa un Editor de Escenas 3D en C++ utilizando OpenGL, FreeGLUT y Dear ImGui. Permite crear y modificar escenas durante la ejecución, sin depender de una escena fija programada previamente.

## Funcionalidades principales

- Creación de cubos, esferas, cilindros y planos.
- Selección, duplicado y eliminación de objetos.
- Modificación de posición, rotación y escala en X, Y y Z.
- Cámara interactiva con zoom y cambio de altura.
- Proyección perspectiva y ortogonal.
- Visualización sólida y wireframe.
- Materiales con color, componente especular y brillo.
- Luz puntual y direccional configurables.
- Aplicación de texturas BMP desde la carpeta `textures`.
- Importación de modelos OBJ.
- Animación basada en `deltaTime`.
- Controles de iniciar, pausar y reiniciar animaciones.
- Jerarquía padre-hijo.
- Cinemática directa mediante un brazo robótico.
- Guardado y carga de escenas.

## Estructura principal

```text
EF_Codigo/
├── include/
├── src/
├── imgui/
├── freeglut/
├── models/
├── textures/
├── EF.exe
└── escena.txt
