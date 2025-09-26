#include <iostream>
#include <cstring>
using namespace std;

#define MAX_TEXT 1000
#define MAX_DICT 65535
#define MAX_ENTRY 100

// ------------------- COMPRESIÓN LZ78 -------------------
int LZ78_Compress(const char* texto, unsigned char* salida) {
    char* diccionario[MAX_DICT];
    int dicSize = 0;
    int len = strlen(texto);
    int outPos = 0;

    char prefijo[MAX_ENTRY];

    for (int i = 0; i < len;) {
        prefijo[0] = '\0';
        int indice = 0;
        int j = i;

        while (j < len) {
            char temp[MAX_ENTRY];
            strcpy(temp, prefijo);
            int l = strlen(temp);
            temp[l] = texto[j];
            temp[l+1] = '\0';

            int encontrado = -1;
            for (int k = 0; k < dicSize; k++) {
                if (strcmp(diccionario[k], temp) == 0) {
                    encontrado = k + 1; // índice 1-based
                    break;
                }
            }

            if (encontrado != -1) {
                strcpy(prefijo, temp);
                indice = encontrado;
                j++;
            } else {
                // Guardar par (indice, simbolo)
                salida[outPos++] = (indice >> 8) & 0xFF;
                salida[outPos++] = indice & 0xFF;
                salida[outPos++] = texto[j];

                // Guardar nueva entrada en diccionario
                diccionario[dicSize] = new char[strlen(temp)+1];
                strcpy(diccionario[dicSize], temp);
                dicSize++;

                j++;
                break;
            }
        }


        i = j;
    }

    for (int i = 0; i < dicSize; i++) {
        delete[] diccionario[i];
    }

    return outPos; 
}

// ------------------- DESCOMPRESIÓN LZ78 -------------------
void LZ78_Decompress(const unsigned char* comp, int compLen, char* salida) {
    char* diccionario[MAX_DICT];
    int dicSize = 0;
    int outPos = 0;

    for (int i = 0; i < compLen; i += 3) {
        int indice = (comp[i] << 8) | comp[i+1];
        char simbolo = comp[i+2];

        char nueva[MAX_ENTRY];
        if (indice == 0) {
            nueva[0] = simbolo;
            nueva[1] = '\0';
        } else {
            strcpy(nueva, diccionario[indice-1]);
            int l = strlen(nueva);
            nueva[l] = simbolo;
            nueva[l+1] = '\0';
        }

        if (simbolo != 0) {
            int l2 = strlen(nueva);
            for (int k = 0; k < l2; k++) salida[outPos++] = nueva[k];
        }

        diccionario[dicSize] = new char[strlen(nueva)+1];
        strcpy(diccionario[dicSize], nueva);
        dicSize++;
    }

    salida[outPos] = '\0';

    // liberar memoria
    for (int i = 0; i < dicSize; i++) delete[] diccionario[i];
}

int main() {
    const char texto[] = "ABAABABA";
    unsigned char comp[MAX_TEXT];
    char descomp[MAX_TEXT];

    int compLen = LZ78_Compress(texto, comp);

    cout << "Texto original: " << texto << endl;
    cout << "Comprimido (bytes): ";
    for (int i = 0; i < compLen; i++) {
        if (i % 3 == 0) cout << " | ";
        if (i % 3 == 2) cout << "'" << comp[i] << "'";
        else printf("0x%02X ", comp[i]);
    }
    cout << endl;

    LZ78_Decompress(comp, compLen, descomp);
    cout << "Descomprimido: " << descomp << endl;

    return 0;
}
