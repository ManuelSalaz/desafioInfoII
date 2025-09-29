
#include <iostream>
#include <cstdio>

// ------------------ ROTACIÓN ------------------
// Rotar un byte a la derecha n bits
unsigned char rotarDerecha(unsigned char valor, int n) {
    return (valor >> n) | (valor << (8 - n));
}

// ------------------ DESENCRIPTAR ------------------
unsigned char desencriptarByte(unsigned char byteEnc, unsigned char K, int n) {
    unsigned char temp = byteEnc;
    temp ^= K;                     // revertir XOR
    temp = rotarDerecha(temp, n);  // revertir rotación izquierda
    return temp;
}

// ------------------ DECOMPRESIÓN RLE ------------------
void RLE_Decompress(unsigned char* bufferIn, int len, FILE* fOut) {
    for (int i = 0; i + 2 < len; i += 3) {
        unsigned char count  = bufferIn[i + 1];  // segundo byte = repeticiones
        unsigned char symbol = bufferIn[i + 2];  // tercer byte = símbolo

        for (int j = 0; j < count; j++) {
            fputc(symbol, fOut);
        }
    }
}

// ------------------ MAIN ------------------
int main() {
    const char* archivoEntrada = "Encriptado1.txt";   // archivo encriptado
    const char* archivoSalida  = "salida.txt";        // salida final

    unsigned char K = 0x5A; // clave XOR
    int n = 3;              // bits de rotación usados en la encriptación

    FILE* fIn = fopen(archivoEntrada, "rb");
    if (!fIn) {
        std::cout << "Error: no se pudo abrir " << archivoEntrada << "\n";
        return 1;
    }

    unsigned char* buffer = new unsigned char[100000]; // máx. 100 KB
    int pos = 0;

    while (true) {
        char t[4];
        if (fread(t, 1, 3, fIn) != 3) break;  // leer terna
        t[3] = '\0';

        // Desencriptar los 3 bytes de la terna
        unsigned char b0 = desencriptarByte((unsigned char)t[0], K, n);
        unsigned char b1 = desencriptarByte((unsigned char)t[1], K, n);
        unsigned char b2 = desencriptarByte((unsigned char)t[2], K, n);

        buffer[pos++] = b0;
        buffer[pos++] = b1;
        buffer[pos++] = b2;
    }
    fclose(fIn);

    // Decompresión RLE
    FILE* fOut = fopen(archivoSalida, "wb");
    if (!fOut) {
        std::cout << "Error: no se pudo crear " << archivoSalida << "\n";
        delete[] buffer;
        return 1;
    }

    RLE_Decompress(buffer, pos, fOut);

    fclose(fOut);
    delete[] buffer;

    std::cout << "Proceso terminado. Revisa " << archivoSalida << "\n";
    return 0;
}
