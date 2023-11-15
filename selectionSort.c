#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// Estrutura para manter as contagens de comparações e trocas
typedef struct {
    int comparisons;
    int swaps;
} SortMetrics;

SortMetrics selectionSort(int arr[], int n) {
    int i, j, minIndex, temp;
    SortMetrics metrics = {0, 0}; // Inicializa as métricas de ordenação

    for (i = 0; i < n - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < n; j++) {
            metrics.comparisons++;
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) { // Checa se uma troca é necessária
            temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
            metrics.swaps++; // Incrementa a contagem de trocas
        }
    }

    return metrics; // Retorna as métricas de ordenação
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
        arr[size++] = value;
    }

    fclose(file);

    SortMetrics metrics = selectionSort(arr, size); // Ordena o array e obtém as métricas

    printf("Arquivo ordenado: %s\n", filename);
    printf("Número de comparações realizadas: %d\n", metrics.comparisons);
    printf("Número de trocas realizadas: %d\n", metrics.swaps);

    free(arr);
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
    const char *folderName = "Dados/5k/Sem Duplicidade"; // Caminho para a pasta com os dados
    processFilesInDirectory(folderName);
    return 0;
}
