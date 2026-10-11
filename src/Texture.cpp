#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>

#include "Texture.h"

bool texturaActiva = true;
GLuint texturaPiedra = 0;

bool cargarTexturaBMP(const char* ruta, GLuint& textura) {
    FILE* archivo = nullptr;

    if (fopen_s(&archivo, ruta, "rb") != 0 || archivo == nullptr) {
        printf("No se encontro el archivo: %s\n", ruta);
        return false;
    }

    unsigned char cabecera[54];

    if (fread(cabecera, 1, 54, archivo) != 54) {
        printf("Cabecera BMP no valida.\n");
        fclose(archivo);
        return false;
    }

    if (cabecera[0] != 'B' || cabecera[1] != 'M') {
        printf("El archivo no es BMP.\n");
        fclose(archivo);
        return false;
    }

    int ancho = *(int*)&cabecera[18];
    int alto = *(int*)&cabecera[22];
    int offset = *(int*)&cabecera[10];
    short bits = *(short*)&cabecera[28];
    int compresion = *(int*)&cabecera[30];

    printf("BMP detectado: %d x %d - %d bits\n", ancho, alto, bits);

    if (bits != 24 && bits != 32) {
        printf("Formato BMP no soportado: %d bits.\n", bits);
        fclose(archivo);
        return false;
    }

    if (compresion != 0) {
        printf("BMP comprimido no soportado.\n");
        fclose(archivo);
        return false;
    }

    bool invertido = alto > 0;

    if (alto < 0)
        alto = -alto;

    int bytesPixel = bits / 8;
    int filaOriginal = ancho * bytesPixel;
    int padding = (4 - (filaOriginal % 4)) % 4;
    int filaBMP = filaOriginal + padding;

    unsigned char* datos = new unsigned char[ancho * alto * 3];
    unsigned char* fila = new unsigned char[filaBMP];

    fseek(archivo, offset, SEEK_SET);

    for (int y = 0; y < alto; y++) {
        if (fread(fila, 1, filaBMP, archivo) != filaBMP) {
            printf("Error leyendo los pixeles del BMP.\n");

            delete[] fila;
            delete[] datos;
            fclose(archivo);

            return false;
        }

        int destinoY = invertido ? y : alto - 1 - y;

        for (int x = 0; x < ancho; x++) {
            int origen = x * bytesPixel;
            int destino = (destinoY * ancho + x) * 3;

            datos[destino] = fila[origen + 2];
            datos[destino + 1] = fila[origen + 1];
            datos[destino + 2] = fila[origen];
        }
    }

    delete[] fila;
    fclose(archivo);

    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        ancho,
        alto,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        datos
    );

    delete[] datos;

    return true;
}

void inicializarTexturas() {
    if (cargarTexturaBMP("textures/piedra.bmp", texturaPiedra)) {
        printf("Textura cargada correctamente. ID: %u\n", texturaPiedra);
    } else {
        printf("ERROR: No se pudo cargar textures/piedra.bmp\n");
    }
}