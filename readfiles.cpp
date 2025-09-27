#include <iostream>
#include <cstdio>
using namespace std;


char* leerArchivoCompleto(const char *nombreArchivo, long &tamano) {
    FILE *archivo = fopen(nombreArchivo, "rb");  
    if (archivo == NULL) {
        cout << "Error: no se pudo abrir el archivo " << nombreArchivo << endl;
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);     // Mover al final
    tamano = ftell(archivo);         // Tamaño del archivo en bytes
    rewind(archivo);                 // Volver al inicio

    // Reservar memoria dinámica
    char *buffer = new char[tamano + 1]; 
    if (buffer == NULL) {
        cout << "Error: no se pudo asignar memoria\n";
        fclose(archivo);
        return NULL;
    }

    // Leer todo el archivo en memoria
    fread(buffer, 1, tamano, archivo);
    buffer[tamano] = '\0';  

    fclose(archivo);
    return buffer;
}

int main() {
    long tamano;
    char *contenido = leerArchivoCompleto("Encriptado1.txt", tamano);

    if (contenido != NULL) {
        cout << "Archivo (" << tamano << " bytes):\n\n";
        cout << contenido << endl;

        delete[] contenido;  // Liberar memoria
    }

    return 0;
}
