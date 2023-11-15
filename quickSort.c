#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int quickSort(int arr[], int low, int high) {
    int comparacoes = 0;  // Inicializa o contador de comparações

    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            comparacoes++;  // Incrementa o contador de comparações
            if (arr[j] < pivot) {
                i++;
                trocar(&arr[i], &arr[j]);
            }
        }
        trocar(&arr[i + 1], &arr[high]);

        int comparacoes_esquerda = quickSort(arr, low, i);
        int comparacoes_direita = quickSort(arr, i + 2, high);

        comparacoes += comparacoes_esquerda + comparacoes_direita;
    }

    return comparacoes;
}

void processFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Não foi possível abrir o arquivo: %s\n", filename);
        return;
    }

    int *arr = NULL;
    int capacity = 10;
    int size = 0;
    int value;

    arr = (int *)malloc(capacity * sizeof(int));

    if (arr == NULL) {
        printf("Erro na alocação de memória.\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "%d", &value) != EOF) {
        if (size >= capacity) {
            capacity *= 2;
            int *temp = (int *)realloc(arr, capacity * sizeof(int));
            if (temp == NULL) {
                printf("Erro na realocação de memória.\n");
                free(arr);
                fclose(file);
                return;
            }
            arr = temp;
        }
        arr[size] = value;
        size++;
    }

    fclose(file);

    int comparacoes = quickSort(arr, 0, size - 1);
    free(arr);

    printf("Arquivo ordenado: %s\n", filename);
    printf("Número total de comparações: %d\n", comparacoes);
}

void processFilesInDirectory(const char *dirPath) {
    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        printf("Não foi possível abrir o diretório: %s\n", dirPath);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char filePath[PATH_MAX];
            snprintf(filePath, PATH_MAX, "%s/%s", dirPath, entry->d_name);
            processFile(filePath);
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char subDirPath[PATH_MAX];
            snprintf(subDirPath, PATH_MAX, "%s/%s", dirPath, entry->d_name);
            processFilesInDirectory(subDirPath);
        }
    }

    closedir(dir);
}

int main() {
    const char *folderName = "Dados";
    processFilesInDirectory(folderName);
    return 0;
}
