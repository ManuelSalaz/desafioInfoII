#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

// rotar a la derecha n bits
static inline unsigned char rotarDerecha(unsigned char valor, int n) {
    return (unsigned char)((valor >> n) | (valor << (8 - n)));
}

// desencriptar un byte
static inline unsigned char desencriptarByte(unsigned char byteEnc, unsigned char K, int n) {
    unsigned char tmp = byteEnc;
    tmp ^= K;
    tmp = rotarDerecha(tmp, n);
    return tmp;
}

int main(void) {
    const char* archivoEnc = "Encriptado3.txt";
    const char* archivoPista = "pista3.txt";

    // Leer pista
    FILE* fP = fopen(archivoPista, "rb");
    if (!fP) { std::printf("Error: no se pudo abrir %s\n", archivoPista); return 1; }
    if (fseek(fP, 0, SEEK_END) != 0) { fclose(fP); std::printf("Error fseek pista\n"); return 1; }
    long pista_len_long = ftell(fP);
    if (pista_len_long < 0) { fclose(fP); std::printf("Error ftell pista\n"); return 1; }
    if (fseek(fP, 0, SEEK_SET) != 0) { fclose(fP); std::printf("Error fseek pista start\n"); return 1; }

    int pista_len = (int)pista_len_long;
    if (pista_len == 0) { fclose(fP); std::printf("Error: pista vacía\n"); return 1; }

    unsigned char* pista = (unsigned char*)malloc(pista_len);
    if (!pista) { fclose(fP); std::printf("Error malloc pista\n"); return 1; }
    if ((int)fread(pista, 1, pista_len, fP) != pista_len) { fclose(fP); free(pista); std::printf("Error leyendo pista\n"); return 1; }
    fclose(fP);

    int encontrado = 0;

    for (int n = 1; n <= 7 && !encontrado; ++n) {
        for (int K = 0; K <= 255 && !encontrado; ++K) {
            FILE* fEnc = fopen(archivoEnc, "rb");
            if (!fEnc) { std::printf("Error: no se pudo abrir %s\n", archivoEnc); free(pista); return 1; }

            // buffer circular para los últimos pista_len bytes
            unsigned char* buffer = (unsigned char*)malloc(pista_len);
            if (!buffer) { std::printf("Error malloc buffer\n"); fclose(fEnc); free(pista); return 1; }
            int buf_pos = 0;
            int buf_count = 0;
            unsigned long long decompressed_offset = 0ULL;

            // mantener todos los bytes descomprimidos solo si encontramos la coincidencia
            unsigned char* full_output = (unsigned char*)malloc(1); // tamaño inicial 1
            size_t output_size = 0;

            unsigned char t[3];
            int match_this_combo = 0;

            while (1) {
                size_t read = fread(t, 1, 3, fEnc);
                if (read != 3) break;

                unsigned char b0 = desencriptarByte(t[0], (unsigned char)K, n);
                unsigned char b1 = desencriptarByte(t[1], (unsigned char)K, n);
                unsigned char b2 = desencriptarByte(t[2], (unsigned char)K, n);

                unsigned int count = (unsigned int)b1;
                unsigned char symbol = b2;

                for (unsigned int j = 0; j < count; ++j) {
                    // almacenar en buffer circular
                    buffer[buf_pos] = symbol;
                    buf_pos = (buf_pos + 1) % pista_len;
                    if (buf_count < pista_len) buf_count++;

                    // agregar al output temporal
                    full_output = (unsigned char*)realloc(full_output, output_size + 1);
                    full_output[output_size++] = symbol;

                    // verificar coincidencia
                    if (buf_count == pista_len) {
                        int match = 1;
                        for (int k = 0; k < pista_len; ++k) {
                            int idx = (buf_pos + k) % pista_len;
                            if (buffer[idx] != pista[k]) {
                                match = 0;
                                break;
                            }
                        }
                        if (match) {
                            unsigned long long match_pos = decompressed_offset + 1 - (unsigned long long)pista_len;
                            std::printf("Coincidencia encontrada!\n");
                            std::printf("K = 0x%02X, n = %d\n", K, n);

                            // escribir salida completa solo ahora
                            FILE* fOut = fopen("salida.txt", "wb");
                            if (fOut) {
                                fwrite(full_output, 1, output_size, fOut);
                                fclose(fOut);
                                std::printf("Archivo descomprimido completo escrito en 'salida.txt'\n");
                            } else {
                                std::printf("Error: no se pudo crear salida.txt\n");
                            }

                            match_this_combo = 1;
                            encontrado = 1;
                            break;
                        }
                    }
                    decompressed_offset++;
                }
                if (match_this_combo) break;
            }

            free(buffer);
            free(full_output);
            fclose(fEnc);
        }
    }

    if (!encontrado) {
        std::printf("No se encontró la pista con las combinaciones probadas (K=0..255, n=1..7).\n");
    }

    free(pista);
    return 0;
}
