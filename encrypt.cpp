#include <iostream>
#include <cstring>
#include <cstdlib>

// --- Rotaciones en 8 bits ---
unsigned char rotl(unsigned char x, int k) {
    return (unsigned char)((x << k) | (x >> (8 - k)));
}

unsigned char rotr(unsigned char x, int k) {
    return (unsigned char)((x >> k) | (x << (8 - k)));
}

// --- Encriptar: reserva memoria dinámica para salida ---
char* encrypt(const char* mensaje, unsigned char key, int rot) {
    int n = strlen(mensaje);
    char* salida = new char[n + 1]; // reservar memoria dinámica

    for (int i = 0; i < n; i++) {
        unsigned char c = (unsigned char)*(mensaje + i);
        unsigned char r = rotl(c, rot);
        unsigned char e = r ^ key;
        *(salida + i) = (char)e;
    }
    *(salida + n) = '\0';
    return salida;
}

// --- Desencriptar: reserva memoria dinámica para salida ---
char* decrypt(const char* mensaje, unsigned char key, int rot) {
    int n = strlen(mensaje);
    char* salida = new char[n + 1];

    for (int i = 0; i < n; i++) {
        unsigned char e = (unsigned char)*(mensaje + i);
        unsigned char r = e ^ key;
        unsigned char d = rotr(r, rot);
        *(salida + i) = (char)d;
    }
    *(salida + n) = '\0';
    return salida;
}

int main() {
    const char* original = "HOLA MUNDO";

    unsigned char clave = 0x5A; // clave XOR
    int rot = 3;                // número de bits a rotar

    // Encriptar
    char* encriptado = encrypt(original, clave, rot);

    std::cout << "Original:    " << original << std::endl;
    std::cout << "Encriptado:  ";
    for (int i = 0; i < strlen(original); i++) {
        printf("%02X ", (unsigned char)*(encriptado + i));
    }
    std::cout << std::endl;

    // Desencriptar
    char* desencriptado = decrypt(encriptado, clave, rot);
    std::cout << "Desencriptado: " << desencriptado << std::endl;

    // Liberar memoria
    delete[] encriptado;
    delete[] desencriptado;

    return 0;
}
