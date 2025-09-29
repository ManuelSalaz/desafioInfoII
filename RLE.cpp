#include <iostream>
#include <cstring>   // strlen, sprintf
#include <cstdlib>   // atoi
using namespace std;

// ------------------ COMPRESIÓN ------------------
char* RLE_Compress(const char* texto) {
    int n = strlen(texto);

    // Reservamos memoria dinámica para salida (n*2 por si todos son diferentes)
    char* salida = new char[n * 2 + 1];
    char* pSalida = salida;

    for (int i = 0; i < n; i++) {
        char letra = texto[i];
        int count = 1;

        // Contar repeticiones consecutivas
        while (i + 1 < n && texto[i + 1] == letra) {
            count++;
            i++;
        }

        // Convertir número a string (en numStr)
        char numStr[20];
        sprintf(numStr, "%d", count);

        // Guardar letra
        *pSalida++ = letra;

        // Guardar número como caracteres
        for (int k = 0; numStr[k] != '\0'; k++) {
            *pSalida++ = numStr[k];
        }
    }

    *pSalida = '\0';  
    return salida;   
}


char* RLE_Decompress(const char* texto) {
    int n = strlen(texto);

    // Reservamos un buffer grande 
    char* salida = new char[n * 10 + 1];
    char* pSalida = salida;

    for (int i = 0; i < n; i++) {
        char letra = texto[i];
        char numStr[20];
        int j = 0;

        // Leer el número que sigue a la letra
        while (i + 1 < n && texto[i + 1] >= '0' && texto[i + 1] <= '9') {
            numStr[j++] = texto[++i];
        }
        numStr[j] = '\0';

        int count = atoi(numStr);  // convertir string → entero

        // Escribir la letra "count" veces
        for (int k = 0; k < count; k++) {
            *pSalida++ = letra;
        }
    }

    *pSalida = '\0';  
    return salida;   
}

int main() {
    const char* textoOriginal = "aaaaabbbccd";
    char* comprimido = RLE_Compress(textoOriginal);
    char* descomprimido = RLE_Decompress(comprimido);

    cout << "Texto original : " << textoOriginal << endl;
    cout << "Comprimido RLE : " << comprimido << endl;
    cout << "Descomprimido  : " << descomprimido << endl;

    // Liberar memoria dinámica
    delete[] comprimido;
    delete[] descomprimido;

    return 0;
}