#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int insertionSort(int arr[], int n) {
    int i, key, j, count = 0;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        // A comparação é feita aqui, então o contador deve ser incrementado
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
            count++;
        }
        arr[j + 1] = key;
    }

    return count;
}

void processDirectory(const char *dirname) {
    struct dirent *de;
    DIR *dir = opendir(dirname);

    if (dir == NULL) {
        printf("Não foi possível abrir a pasta '%s'.\n", dirname);
        return;
    }

    while ((de = readdir(dir)) != NULL) {
        if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
            char filepath[256];
            snprintf(filepath, sizeof(filepath), "%s/%s", dirname, de->d_name);

            if (de->d_type == DT_DIR) {
                processDirectory(filepath);
            } else if (de->d_type == DT_REG) {
                FILE *file = fopen(filepath, "r");
                if (file == NULL) {
                    printf("Não foi possível abrir o arquivo %s.\n", filepath);
                    continue;
                }

                int *arr = (int *)malloc(10 * sizeof(int));
                if (arr == NULL) {
                    printf("Erro na alocação de memória.\n");
                    fclose(file);
                    continue;
                }

                int size = 0;
                int capacity = 10;
                int value;

                while (fscanf(file, "%d", &value) != EOF) {
                    if (size >= capacity) {
                        capacity *= 2;
                        int *temp = (int *)realloc(arr, capacity * sizeof(int));

                        if (temp == NULL) {
                            printf("Erro na realocação de memória.\n");
                            free(arr);
                            fclose(file);
                            arr = NULL;
                            break;
                        }

                        arr = temp;
                    }

                    arr[size++] = value;
                }

                if (arr) {
                    int count = insertionSort(arr, size);
                    printf("Número do loop '%s': %d\n", filepath, count);
                    free(arr);
                }

                fclose(file);
            }
        }
    }

    closedir(dir);
}

int main() {
    processDirectory("Dados");
    return 0;
}
