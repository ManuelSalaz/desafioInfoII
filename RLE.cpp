#include <iostream>
#include <cstring>   // strlen, strcpy, strcat, strcmp, sprintf
using namespace std;


char* RLE_Compress(const char* texto) {
    int n = strlen(texto);

    // Reservamos memoria dinámica para salida (n*2 por si todos son diferentes)
    char* salida = new char[n * 2 + 1];
    *salida = '\0'; 

    char* pSalida = salida;

    for (int i = 0; i < n; i++) {
        char letra = *(texto + i);
        int count = 1;

        // Contar repeticiones consecutivas
        while (i + 1 < n && *(texto + i + 1) == letra) {
            count++;
            i++;
        }

        // Convertir número a string
        char numStr[20];
        sprintf(numStr, "%d", count);

        // Escribir letra
        *pSalida = letra;
        pSalida++;
        *pSalida = '\0';

        // Escribir número
        for (int k = 0; numStr[k] != '\0'; k++) {
            *pSalida = numStr[k];
            pSalida++;
        }
        *pSalida = '\0';
    }

    return salida; // devolver puntero dinámico
}
int main() {
    const char* textoRLE = "aaaaabbbccd";
    char* salidaRLE = RLE_Compress(textoRLE);

    cout << "Original RLE: " << textoRLE << endl;
    cout << "Comprimido RLE: " << salidaRLE << endl;
}
