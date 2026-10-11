#include <GL/freeglut.h>

#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <vector>
#include <string>

#include "Camera.h"
#include "Scene.h"
#include "Curves.h"
#include "Lighting.h"
#include "Texture.h"
#include "SceneManager.h"

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

// ============================================================
// EXAMEN FINAL - COMPUTACION GRAFICA
// Editor de Escenas 3D
// ============================================================

SceneManager editor;

int tiempoAnterior = 0;

// ============================================================
// BUSCAR TEXTURAS BMP AUTOMATICAMENTE
// ============================================================

std::vector<std::string> obtenerTexturasDisponibles() {
    std::vector<std::string> texturas;

    WIN32_FIND_DATAA datos;

    HANDLE busqueda = FindFirstFileA(
        "textures\\*.bmp",
        &datos
    );

    if (busqueda == INVALID_HANDLE_VALUE)
        return texturas;

    do {
        if (!(datos.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            texturas.push_back(
                "textures/" + std::string(datos.cFileName)
            );
        }
    }
    while (FindNextFileA(busqueda, &datos));

    FindClose(busqueda);

    return texturas;
}

// ============================================================
// INTERFAZ
// ============================================================

void dibujarInterfaz() {
    ImGuiIO& io = ImGui::GetIO();

    float ancho = io.DisplaySize.x;
    float alto = io.DisplaySize.y;

    float anchoIzquierdo = 230.0f;
    float anchoDerecho = 280.0f;

    // ========================================================
    // PANEL IZQUIERDO - ESCENA
    // ========================================================

    ImGui::SetNextWindowPos(
        ImVec2(0.0f, 0.0f),
        ImGuiCond_Always
    );

    ImGui::SetNextWindowSize(
        ImVec2(anchoIzquierdo, alto),
        ImGuiCond_Always
    );

    ImGui::Begin(
        "Escena",
        nullptr,
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse
    );

    ImGui::Text("Objetos");
    ImGui::Separator();

    const auto& objetos = editor.obtenerObjetos();

    int seleccionado =
        editor.obtenerIndiceSeleccionado();

    for (int i = 0; i < (int)objetos.size(); i++) {
        bool activo =
            (i == seleccionado);

        if (ImGui::Selectable(
            objetos[i].nombre.c_str(),
            activo
        )) {
            editor.seleccionarIndice(i);
        }
    }

    ImGui::Separator();

    if (ImGui::Button("Cubo"))
        editor.agregarObjeto(CUBO);

    ImGui::SameLine();

    if (ImGui::Button("Esfera"))
        editor.agregarObjeto(ESFERA);

    if (ImGui::Button("Cilindro"))
        editor.agregarObjeto(CILINDRO);

    ImGui::SameLine();

    if (ImGui::Button("Plano"))
        editor.agregarObjeto(PLANO);

    if (ImGui::Button("Importar OBJ")) {
        editor.agregarModeloOBJ(
            "models/modelo.obj"
        );
    }

    if (ImGui::Button("Crear brazo FK")) {
        editor.crearBrazoRobot();
    }

    ImGui::Separator();

    if (ImGui::Button("Duplicar"))
        editor.duplicarSeleccionado();

    ImGui::SameLine();

    if (ImGui::Button("Eliminar"))
        editor.eliminarSeleccionado();

    ImGui::Separator();

    if (ImGui::Button("Guardar escena")) {
        editor.guardarEscena(
            "escena.txt"
        );
    }

    ImGui::SameLine();

    if (ImGui::Button("Cargar escena")) {
        editor.cargarEscena(
            "escena.txt"
        );
    }

    ImGui::End();

    // ========================================================
    // PANEL DERECHO - PROPIEDADES
    // ========================================================

    ImGui::SetNextWindowPos(
        ImVec2(
            ancho - anchoDerecho,
            0.0f
        ),
        ImGuiCond_Always
    );

    ImGui::SetNextWindowSize(
        ImVec2(
            anchoDerecho,
            alto
        ),
        ImGuiCond_Always
    );

    ImGui::Begin(
        "Propiedades",
        nullptr,
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse
    );

    Objeto3D* objeto =
        editor.obtenerSeleccionado();

    if (objeto) {
        ImGui::Text(
            "Objeto: %s",
            objeto->nombre.c_str()
        );

        ImGui::Separator();

        // ====================================================
        // TRANSFORMACION
        // ====================================================

        ImGui::Text("Transformacion");

        float posicion[3] = {
            objeto->transformacion.x,
            objeto->transformacion.y,
            objeto->transformacion.z
        };

        if (ImGui::DragFloat3(
            "Posicion",
            posicion,
            0.1f
        )) {
            objeto->transformacion.x =
                posicion[0];

            objeto->transformacion.y =
                posicion[1];

            objeto->transformacion.z =
                posicion[2];
        }

        float rotacion[3] = {
            objeto->transformacion.rotX,
            objeto->transformacion.rotY,
            objeto->transformacion.rotZ
        };

        if (ImGui::DragFloat3(
            "Rotacion",
            rotacion,
            1.0f
        )) {
            objeto->transformacion.rotX =
                rotacion[0];

            objeto->transformacion.rotY =
                rotacion[1];

            objeto->transformacion.rotZ =
                rotacion[2];
        }

        float escala[3] = {
            objeto->transformacion.escalaX,
            objeto->transformacion.escalaY,
            objeto->transformacion.escalaZ
        };

        if (ImGui::DragFloat3(
            "Escala",
            escala,
            0.05f,
            0.05f,
            20.0f
        )) {
            objeto->transformacion.escalaX =
                escala[0];

            objeto->transformacion.escalaY =
                escala[1];

            objeto->transformacion.escalaZ =
                escala[2];
        }

        // ====================================================
        // MATERIAL
        // ====================================================

        ImGui::Separator();

        ImGui::Text("Material");

        float color[3] = {
            objeto->material.r,
            objeto->material.g,
            objeto->material.b
        };

        if (ImGui::ColorEdit3(
            "Color",
            color
        )) {
            objeto->material.r =
                color[0];

            objeto->material.g =
                color[1];

            objeto->material.b =
                color[2];
        }

        ImGui::SliderFloat(
            "Especular",
            &objeto->material.especular,
            0.0f,
            1.0f
        );

        ImGui::SliderFloat(
            "Brillo",
            &objeto->material.brillo,
            1.0f,
            128.0f
        );

        // ====================================================
        // TEXTURA
        // ====================================================

        ImGui::Separator();

        ImGui::Text("Textura");

        static std::vector<std::string> texturas =
            obtenerTexturasDisponibles();

        static int texturaSeleccionada = 0;

        if (!texturas.empty()) {

            if (
                texturaSeleccionada >=
                (int)texturas.size()
            ) {
                texturaSeleccionada = 0;
            }

            const char* texturaActual =
                texturas[
                    texturaSeleccionada
                ].c_str();

            if (ImGui::BeginCombo(
                "Archivo",
                texturaActual
            )) {

                for (
                    int i = 0;
                    i < (int)texturas.size();
                    i++
                ) {
                    bool seleccionada =
                        (
                            texturaSeleccionada
                            == i
                        );

                    if (ImGui::Selectable(
                        texturas[i].c_str(),
                        seleccionada
                    )) {
                        texturaSeleccionada =
                            i;
                    }

                    if (seleccionada) {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            if (ImGui::Button(
                "Aplicar textura"
            )) {
                editor.asignarTexturaSeleccionado(
                    texturas[
                        texturaSeleccionada
                    ]
                );
            }

            ImGui::SameLine();

            if (ImGui::Button(
                "Quitar textura"
            )) {
                editor.quitarTexturaSeleccionado();
            }
        }
        else {
            ImGui::Text(
                "No hay archivos BMP en textures/"
            );
        }

        // ====================================================
        // ANIMACION
        // ====================================================

        ImGui::Separator();

        ImGui::Text("Animacion");

        if (ImGui::Button("Iniciar"))
            editor.iniciarAnimacionSeleccionado();

        ImGui::SameLine();

        if (ImGui::Button("Pausar"))
            editor.pausarAnimacionSeleccionado();

        ImGui::SameLine();

        if (ImGui::Button("Reset"))
            editor.resetAnimacionSeleccionado();

        // ====================================================
        // ILUMINACION
        // ====================================================

        ImGui::Separator();

        ImGui::Text("Iluminacion");

        ImGui::Checkbox(
            "Luz puntual",
            &luzPuntualActiva
        );

        ImGui::Checkbox(
            "Luz direccional",
            &luzDireccionalActiva
        );

        ImGui::SliderFloat(
            "Intensidad luz",
            &intensidadLuzPrincipal,
            0.0f,
            2.0f
        );

        ImGui::SliderFloat(
            "Angulo luz",
            &anguloLuz,
            0.0f,
            360.0f
        );

        ImGui::Checkbox(
            "Animar luz",
            &animacionLuzActiva
        );

        ImGui::SliderFloat(
            "Velocidad luz",
            &velocidadLuz,
            0.25f,
            4.0f
        );
    }
    else {
        ImGui::Text(
            "No hay objeto seleccionado."
        );
    }

    ImGui::End();
}

// ============================================================
// GRID
// ============================================================

void dibujarGrid() {
    glPushAttrib(
        GL_ENABLE_BIT |
        GL_CURRENT_BIT
    );

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glColor3f(
        0.35f,
        0.35f,
        0.35f
    );

    glBegin(GL_LINES);

    for (int i = -10; i <= 10; i++) {
        glVertex3f(
            (float)i,
            0.0f,
            -10.0f
        );

        glVertex3f(
            (float)i,
            0.0f,
            10.0f
        );

        glVertex3f(
            -10.0f,
            0.0f,
            (float)i
        );

        glVertex3f(
            10.0f,
            0.0f,
            (float)i
        );
    }

    glEnd();

    glPopAttrib();
}

// ============================================================
// DISPLAY
// ============================================================

void display() {
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );

    int anchoCentral =
        anchoVentana -
        230 -
        280;

    if (anchoCentral < 1)
        anchoCentral = 1;

    glViewport(
        230,
        0,
        anchoCentral,
        altoVentana
    );

    configurarProyeccion(
        anchoCentral,
        altoVentana
    );

    glMatrixMode(
        GL_MODELVIEW
    );

    glLoadIdentity();

    aplicarCamara();

    configurarIluminacion();

    int tiempoActual =
        glutGet(
            GLUT_ELAPSED_TIME
        );

    float deltaTime = 0.0f;

    if (tiempoAnterior != 0) {
        deltaTime =
            (
                tiempoActual -
                tiempoAnterior
            ) /
            1000.0f;
    }

    tiempoAnterior =
        tiempoActual;

    editor.actualizarAnimaciones(
        deltaTime
    );

    dibujarGrid();

    if (modoWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    editor.dibujarEscena();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    dibujarIndicadorLuz();

    // ========================================================
    // IMGUI
    // ========================================================

    ImGui_ImplOpenGL2_NewFrame();

    ImGui_ImplGLUT_NewFrame();

    ImGui::NewFrame();

    dibujarInterfaz();

    ImGui::Render();

    ImGui_ImplOpenGL2_RenderDrawData(
        ImGui::GetDrawData()
    );

    glutSwapBuffers();

    glutPostRedisplay();
}

// ============================================================
// RESHAPE
// ============================================================

void reshape(
    int width,
    int height
) {
    ImGui_ImplGLUT_ReshapeFunc(
        width,
        height
    );

    redimensionarCamara(
        width,
        height
    );

    glutPostRedisplay();
}

// ============================================================
// TIMER
// ============================================================

void timer(int value) {
    if (animacionLuzActiva) {
        anguloLuz +=
            velocidadLuz;

        if (anguloLuz >= 360.0f)
            anguloLuz -= 360.0f;

        glutPostRedisplay();
    }

    glutTimerFunc(
        16,
        timer,
        0
    );
}

// ============================================================
// TECLADO
// ============================================================

void teclado(
    unsigned char tecla,
    int x,
    int y
) {
    ImGui_ImplGLUT_KeyboardFunc(
        tecla,
        x,
        y
    );

    if (
        ImGui::GetIO().WantCaptureKeyboard
    ) {
        return;
    }

    switch (tecla) {

        // ====================================================
        // VISUALIZACION
        // ====================================================

        case 'm':
        case 'M':
            modoWireframe =
                !modoWireframe;
            break;

        case 'b':
        case 'B':
            mostrarPuntosControl =
                !mostrarPuntosControl;
            break;

        case 'p':
        case 'P':
            perspectiva =
                !perspectiva;
            break;

        // ====================================================
        // ESCENA ORIGINAL
        // ====================================================

        case 'a':
        case 'A':
            anguloMonumento +=
                5.0f;
            break;

        case 'd':
        case 'D':
            anguloMonumento -=
                5.0f;
            break;

        case 'q':
        case 'Q':
            rotacionEscena +=
                5.0f;
            break;

        case 'e':
        case 'E':
            rotacionEscena -=
                5.0f;
            break;

        // ====================================================
        // CREACION
        // ====================================================

        case 'c':
        case 'C':
            editor.agregarObjeto(
                CUBO
            );
            break;

        case 'v':
        case 'V':
            editor.agregarObjeto(
                ESFERA
            );
            break;

        case 'n':
        case 'N':
            editor.agregarObjeto(
                CILINDRO
            );
            break;

        case 'f':
        case 'F':
            editor.agregarObjeto(
                PLANO
            );
            break;

        // ====================================================
        // GESTION
        // ====================================================

        case 9:
            editor.seleccionarSiguiente();
            break;

        case 'g':
        case 'G':
            editor.duplicarSeleccionado();
            break;

        case 'k':
        case 'K':
            editor.eliminarSeleccionado();
            break;

        case 127:
            editor.eliminarSeleccionado();
            break;

        // ====================================================
        // POSICION
        // ====================================================

        case 'h':
        case 'H':
            editor.moverSeleccionado(
                -0.5f,
                0.0f,
                0.0f
            );
            break;

        case 'l':
        case 'L':
            editor.moverSeleccionado(
                0.5f,
                0.0f,
                0.0f
            );
            break;

        case 'y':
        case 'Y':
            editor.moverSeleccionado(
                0.0f,
                0.5f,
                0.0f
            );
            break;

        case 'u':
        case 'U':
            editor.moverSeleccionado(
                0.0f,
                -0.5f,
                0.0f
            );
            break;

        case 'i':
        case 'I':
            editor.moverSeleccionado(
                0.0f,
                0.0f,
                -0.5f
            );
            break;

        case 'o':
        case 'O':
            editor.moverSeleccionado(
                0.0f,
                0.0f,
                0.5f
            );
            break;

        // ====================================================
        // ROTACION
        // ====================================================

        case 'j':
        case 'J':
            editor.rotarSeleccionado(
                0.0f,
                -5.0f,
                0.0f
            );
            break;

        case ';':
            editor.rotarSeleccionado(
                0.0f,
                5.0f,
                0.0f
            );
            break;

        // ====================================================
        // ESCALA
        // ====================================================

        case '+':
        case '=':
            editor.escalarSeleccionado(
                1.10f
            );
            break;

        case '-':
        case '_':
            editor.escalarSeleccionado(
                0.90f
            );
            break;

        // ====================================================
        // ILUMINACION
        // ====================================================

        case '1':
            luzPuntualActiva =
                !luzPuntualActiva;
            break;

        case '2':
            luzDireccionalActiva =
                !luzDireccionalActiva;
            break;

        case '3':
            intensidadLuzPrincipal +=
                0.20f;

            if (
                intensidadLuzPrincipal >
                1.60f
            ) {
                intensidadLuzPrincipal =
                    0.40f;
            }

            printf(
                "Intensidad luz principal: %.2f\n",
                intensidadLuzPrincipal
            );

            break;

        // ====================================================
        // TEXTURA
        // ====================================================

        case 't':
        case 'T':
            texturaActiva =
                !texturaActiva;

            printf(
                "Textura: %s\n",
                texturaActiva
                    ? "ACTIVADA"
                    : "DESACTIVADA"
            );

            break;

        // ====================================================
        // ZOOM
        // ====================================================

        case 'z':
        case 'Z':
            distanciaCamara -=
                1.0f;

            if (
                distanciaCamara <
                10.0f
            ) {
                distanciaCamara =
                    10.0f;
            }

            break;

        case 'x':
        case 'X':
            distanciaCamara +=
                1.0f;

            if (
                distanciaCamara >
                50.0f
            ) {
                distanciaCamara =
                    50.0f;
            }

            break;

        // ====================================================
        // ANIMACION DE LUZ
        // ====================================================

        case 'r':
        case 'R':
            animacionLuzActiva =
                !animacionLuzActiva;

            printf(
                "Animacion de luz: %s\n",
                animacionLuzActiva
                    ? "ACTIVADA"
                    : "PAUSADA"
            );

            break;

        case '4':
            velocidadLuz -=
                0.25f;

            if (velocidadLuz < 0.25f)
                velocidadLuz = 0.25f;

            printf(
                "Velocidad luz: %.2f\n",
                velocidadLuz
            );

            break;

        case '5':
            velocidadLuz +=
                0.25f;

            if (velocidadLuz > 4.0f)
                velocidadLuz = 4.0f;

            printf(
                "Velocidad luz: %.2f\n",
                velocidadLuz
            );

            break;

        // ====================================================
        // POSICIONES LUZ
        // ====================================================

        case '6':
            animacionLuzActiva =
                false;

            anguloLuz =
                0.0f;

            printf(
                "Luz fija: 0 grados\n"
            );

            break;

        case '7':
            animacionLuzActiva =
                false;

            anguloLuz =
                90.0f;

            printf(
                "Luz fija: 90 grados\n"
            );

            break;

        case '8':
            animacionLuzActiva =
                false;

            anguloLuz =
                180.0f;

            printf(
                "Luz fija: 180 grados\n"
            );

            break;

        case '9':
            animacionLuzActiva =
                false;

            anguloLuz =
                270.0f;

            printf(
                "Luz fija: 270 grados\n"
            );

            break;

        // ====================================================
        // SALIR
        // ====================================================

        case 27:
            exit(0);
            break;
    }

    glutPostRedisplay();
}

// ============================================================
// TECLAS ESPECIALES
// ============================================================

void teclasEspeciales(
    int tecla,
    int x,
    int y
) {
    ImGui_ImplGLUT_SpecialFunc(
        tecla,
        x,
        y
    );

    if (
        ImGui::GetIO().WantCaptureKeyboard
    ) {
        return;
    }

    switch (tecla) {

        case GLUT_KEY_LEFT:
            anguloCamara -=
                4.0f;
            break;

        case GLUT_KEY_RIGHT:
            anguloCamara +=
                4.0f;
            break;

        case GLUT_KEY_UP:
            alturaCamara +=
                0.5f;

            if (
                alturaCamara >
                25.0f
            ) {
                alturaCamara =
                    25.0f;
            }

            break;

        case GLUT_KEY_DOWN:
            alturaCamara -=
                0.5f;

            if (
                alturaCamara <
                2.0f
            ) {
                alturaCamara =
                    2.0f;
            }

            break;
    }

    glutPostRedisplay();
}

// ============================================================
// MOUSE
// ============================================================

void mouse(
    int boton,
    int estado,
    int x,
    int y
) {
    ImGui_ImplGLUT_MouseFunc(
        boton,
        estado,
        x,
        y
    );

    glutPostRedisplay();
}

void movimientoMouse(
    int x,
    int y
) {
    ImGui_ImplGLUT_MotionFunc(
        x,
        y
    );

    glutPostRedisplay();
}

void movimientoPasivoMouse(
    int x,
    int y
) {
    ImGui_ImplGLUT_MotionFunc(
        x,
        y
    );

    glutPostRedisplay();
}

void ruedaMouse(
    int boton,
    int direccion,
    int x,
    int y
) {
    ImGui_ImplGLUT_MouseWheelFunc(
        boton,
        direccion,
        x,
        y
    );

    glutPostRedisplay();
}

// ============================================================
// INICIALIZACION
// ============================================================

void inicializar() {
    glClearColor(
        0.55f,
        0.78f,
        0.92f,
        1.0f
    );

    glEnable(
        GL_DEPTH_TEST
    );

    inicializarIluminacion();

    inicializarTexturas();

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGLUT_Init();

    ImGui_ImplOpenGL2_Init();

    printf("\n");
    printf("=============================================\n");
    printf(" EDITOR DE ESCENAS 3D - EXAMEN FINAL\n");
    printf("=============================================\n");

    printf("\nCREACION\n");
    printf("C       : Crear cubo\n");
    printf("V       : Crear esfera\n");
    printf("N       : Crear cilindro\n");
    printf("F       : Crear plano\n");

    printf("\nGESTION\n");
    printf("TAB     : Seleccionar siguiente objeto\n");
    printf("G       : Duplicar objeto seleccionado\n");
    printf("K       : Eliminar objeto seleccionado\n");

    printf("\nPOSICION DEL OBJETO\n");
    printf("H / L   : Mover en X\n");
    printf("Y / U   : Mover en Y\n");
    printf("I / O   : Mover en Z\n");

    printf("\nROTACION\n");
    printf("J / ;   : Rotar sobre eje Y\n");

    printf("\nESCALA\n");
    printf("+ / -   : Aumentar / reducir escala\n");

    printf("\nCAMARA\n");
    printf("Flechas : Orbitar / cambiar altura\n");
    printf("Z / X   : Zoom\n");
    printf("P       : Perspectiva / ortogonal\n");

    printf("\nOTROS\n");
    printf("M       : Solido / wireframe\n");
    printf("ESC     : Salir\n");

    printf("=============================================\n\n");
}

// ============================================================
// MAIN
// ============================================================

int main(
    int argc,
    char** argv
) {
    glutInit(
        &argc,
        argv
    );

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_DEPTH
    );

    glutInitWindowSize(
        anchoVentana,
        altoVentana
    );

    glutInitWindowPosition(
        100,
        50
    );

    glutCreateWindow(
        "EF - Editor de Escenas 3D - Juan Marcelo Chamorro"
    );

    inicializar();

    glutDisplayFunc(
        display
    );

    glutReshapeFunc(
        reshape
    );

    glutKeyboardFunc(
        teclado
    );

    glutSpecialFunc(
        teclasEspeciales
    );

    glutMouseFunc(
        mouse
    );

    glutMotionFunc(
        movimientoMouse
    );

    glutPassiveMotionFunc(
        movimientoPasivoMouse
    );

    glutMouseWheelFunc(
        ruedaMouse
    );

    glutTimerFunc(
        16,
        timer,
        0
    );

    glutMainLoop();

    return 0;
}