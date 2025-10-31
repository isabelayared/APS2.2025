/* main.c
 * APS - Sistema de Busca e Ordenação (Pará 2023/2024)
 * 
 * - Lê arquivos CSV (2023, 2024)
 * - Ordena por município, bioma ou data
 * - Usa 8 algoritmos (Bubble, Selection, Insertion, Shell, Merge, Quick, Heap e Bogo)
 * - Conta comparações, trocas e mede tempo total
 * - Exibe registros 200 por vez, com opção de continuar
 * 
 * Compatível com Dev-C++ (C89/C90)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LINE_SIZE 1024
#define INITIAL_CAPACITY 1000
#define MAX_FIELD 256
#define BOGO_MAX_ATTEMPTS 1000000L
#define PAGE_SIZE 200  /* número de registros por página */

typedef struct {
    char id[MAX_FIELD];
    char uuid[MAX_FIELD];
    char latitude[MAX_FIELD];
    char longitude[MAX_FIELD];
    char datahora[MAX_FIELD];
    char pais[MAX_FIELD];
    char estado[MAX_FIELD];
    char municipio[MAX_FIELD];
    char bioma[MAX_FIELD];
} Record;

/* Contadores globais */
long long comparisons;
long long swaps;

/* Tipo de função comparadora */
typedef int (*Comparator)(const Record*, const Record*);

/* ---------- Funções utilitárias ---------- */
Record* alloc_records(size_t n) {
    Record *r = (Record*) malloc(sizeof(Record) * n);
    if (!r) {
        fprintf(stderr, "Erro: memoria insuficiente\n");
        exit(EXIT_FAILURE);
    }
    return r;
}

void copy_records(Record *dest, const Record *src, size_t n) {
    size_t i;
    for (i = 0; i < n; ++i) dest[i] = src[i];
}

void swap_records(Record *a, Record *b) {
    Record temp = *a;
    *a = *b;
    *b = temp;
    swaps++;
}

void trim(char *s) {
    char *p = s, *end;
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
    if (p != s) memmove(s, p, strlen(p) + 1);
    end = s + strlen(s) - 1;
    while (end >= s && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) { *end = '\0'; end--; }
}

/* ---------- Comparadores ---------- */
int cmp_municipio(const Record *a, const Record *b) { comparisons++; return strcmp(a->municipio, b->municipio); }
int cmp_bioma(const Record *a, const Record *b) { comparisons++; return strcmp(a->bioma, b->bioma); }
int cmp_datahora(const Record *a, const Record *b) { comparisons++; return strcmp(a->datahora, b->datahora); }

/* ---------- Leitura do CSV ---------- */
Record* read_csv(const char *filename, size_t *out_n) {
    FILE *f = fopen(filename, "r");
    char line[LINE_SIZE];
    size_t capacity = INITIAL_CAPACITY, n = 0;
    Record *arr = alloc_records(capacity);

    if (!f) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, LINE_SIZE, f)) {
        Record rec;
        char *token;
        int field = 0;
        token = strtok(line, ",");
        while (token) {
            trim(token);
            switch (field) {
                case 0: strncpy(rec.id, token, MAX_FIELD); break;
                case 1: strncpy(rec.uuid, token, MAX_FIELD); break;
                case 2: strncpy(rec.latitude, token, MAX_FIELD); break;
                case 3: strncpy(rec.longitude, token, MAX_FIELD); break;
                case 4: strncpy(rec.datahora, token, MAX_FIELD); break;
                case 5: strncpy(rec.pais, token, MAX_FIELD); break;
                case 6: strncpy(rec.estado, token, MAX_FIELD); break;
                case 7: strncpy(rec.municipio, token, MAX_FIELD); break;
                case 8: strncpy(rec.bioma, token, MAX_FIELD); break;
            }
            field++;
            token = strtok(NULL, ",");
        }
        if (field < 9) continue;
        if (n >= capacity) {
            capacity *= 2;
            arr = (Record*) realloc(arr, capacity * sizeof(Record));
            if (!arr) { fprintf(stderr, "Erro: memoria insuficiente\n"); exit(EXIT_FAILURE); }
        }
        arr[n++] = rec;
    }

    fclose(f);
    *out_n = n;
    return arr;
}

/* ---------- Algoritmos de Ordenação ---------- */
int record_less(const Record *a, const Record *b, Comparator comp) { return comp(a, b) < 0; }

/* Bubble Sort */
void bubble_sort(Record *arr, size_t n, Comparator comp) {
    size_t i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j + 1 < n; j++)
            if (record_less(&arr[j+1], &arr[j], comp))
                swap_records(&arr[j], &arr[j+1]);
}

/* Selection Sort */
void selection_sort(Record *arr, size_t n, Comparator comp) {
    size_t i, j, min;
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++)
            if (record_less(&arr[j], &arr[min], comp))
                min = j;
        if (min != i)
            swap_records(&arr[i], &arr[min]);
    }
}

/* Insertion Sort */
void insertion_sort(Record *arr, size_t n, Comparator comp) {
    size_t i, j;
    for (i = 1; i < n; i++) {
        Record key = arr[i];
        j = i;
        while (j > 0 && record_less(&key, &arr[j - 1], comp)) {
            arr[j] = arr[j - 1];
            swaps++;
            j--;
        }
        arr[j] = key;
        swaps++;
    }
}

/* Shell Sort */
void shell_sort(Record *arr, size_t n, Comparator comp) {
    size_t gap, i, j;
    for (gap = n / 2; gap > 0; gap /= 2)
        for (i = gap; i < n; i++) {
            Record temp = arr[i];
            j = i;
            while (j >= gap && record_less(&temp, &arr[j - gap], comp)) {
                arr[j] = arr[j - gap];
                swaps++;
                j -= gap;
            }
            arr[j] = temp;
            swaps++;
        }
}

/* Merge Sort */
void merge(Record *arr, size_t l, size_t m, size_t r, Record *aux, Comparator comp) {
    size_t i = l, j = m, k = l;
    while (i < m && j < r)
        if (!record_less(&arr[j], &arr[i], comp))
            aux[k++] = arr[i++];
        else
            aux[k++] = arr[j++];
    while (i < m) aux[k++] = arr[i++];
    while (j < r) aux[k++] = arr[j++];
    for (k = l; k < r; k++) { arr[k] = aux[k]; swaps++; }
}
void merge_rec(Record *arr, size_t l, size_t r, Record *aux, Comparator comp) {
    if (r - l <= 1) return;
    size_t m = (l + r) / 2;
    merge_rec(arr, l, m, aux, comp);
    merge_rec(arr, m, r, aux, comp);
    merge(arr, l, m, r, aux, comp);
}
void merge_sort(Record *arr, size_t n, Comparator comp) {
    Record *aux = alloc_records(n);
    merge_rec(arr, 0, n, aux, comp);
    free(aux);
}

/* Quick Sort */
size_t partition(Record *arr, size_t low, size_t high, Comparator comp) {
    Record pivot = arr[high];
    size_t i = low, j;
    for (j = low; j < high; j++)
        if (record_less(&arr[j], &pivot, comp)) {
            swap_records(&arr[i], &arr[j]);
            i++;
        }
    swap_records(&arr[i], &arr[high]);
    return i;
}
void quick_rec(Record *arr, size_t low, size_t high, Comparator comp) {
    if (low < high) {
        size_t p = partition(arr, low, high, comp);
        if (p > 0) quick_rec(arr, low, p - 1, comp);
        quick_rec(arr, p + 1, high, comp);
    }
}
void quick_sort(Record *arr, size_t n, Comparator comp) { if (n) quick_rec(arr, 0, n - 1, comp); }

/* Heap Sort */
void heapify(Record *arr, size_t n, size_t i, Comparator comp) {
    size_t largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && record_less(&arr[largest], &arr[l], comp)) largest = l;
    if (r < n && record_less(&arr[largest], &arr[r], comp)) largest = r;
    if (largest != i) { swap_records(&arr[i], &arr[largest]); heapify(arr, n, largest, comp); }
}
void heap_sort(Record *arr, size_t n, Comparator comp) {
    size_t i;
    for (i = n / 2; i-- > 0; ) { heapify(arr, n, i, comp); if (i == 0) break; }
    for (i = n - 1; i > 0; i--) { swap_records(&arr[0], &arr[i]); heapify(arr, i, 0, comp); }
}

/* Bogo Sort */
int is_sorted(const Record *arr, size_t n, Comparator comp) {
    size_t i;
    for (i = 1; i < n; i++)
        if (record_less(&arr[i], &arr[i - 1], comp))
            return 0;
    return 1;
}
void shuffle(Record *arr, size_t n) {
    size_t i;
    for (i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        swap_records(&arr[i], &arr[j]);
    }
}
void bogo_sort(Record *arr, size_t n, Comparator comp) {
    long attempts = 0;
    while (!is_sorted(arr, n, comp)) {
        if (++attempts > BOGO_MAX_ATTEMPTS) {
            printf("Bogo Sort: limite de tentativas atingido.\n");
            return;
        }
        shuffle(arr, n);
    }
}

/* ---------- Exibição paginada ---------- */
void print_record(const Record *r) {
    printf("%s ,%s ,%s ,%s ,%s ,%s ,%s ,%s\n",
           r->id, r->uuid, r->latitude, r->longitude, r->datahora,
           r->pais, r->estado, r->municipio, r->bioma);
}

void run_algorithm(int algo, const Record *original, size_t n, Comparator comp) {
    Record *arr = alloc_records(n);
    copy_records(arr, original, n);
    comparisons = swaps = 0;
    srand((unsigned) time(NULL));
    clock_t start = clock();

    switch (algo) {
        case 1: bubble_sort(arr, n, comp); break;
        case 2: selection_sort(arr, n, comp); break;
        case 3: insertion_sort(arr, n, comp); break;
        case 4: shell_sort(arr, n, comp); break;
        case 5: merge_sort(arr, n, comp); break;
        case 6: quick_sort(arr, n, comp); break;
        case 7: heap_sort(arr, n, comp); break;
        case 8: bogo_sort(arr, n, comp); break;
    }

    clock_t end = clock();
    double total_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n--- Estatísticas ---\n");
    printf("Registros: %u\nComparações: %lld\nTrocas: %lld\nTempo total: %.6f s\n\n",
           (unsigned)n, comparisons, swaps, total_time);

    /* Exibição 200 por vez */
    size_t shown = 0;
    char resp[8];
    while (shown < n) {
        size_t i, limit = shown + PAGE_SIZE;
        if (limit > n) limit = n;

        clock_t block_start = clock();
        for (i = shown; i < limit; i++) print_record(&arr[i]);
        clock_t block_end = clock();

        double block_time = ((double)(block_end - start)) / CLOCKS_PER_SEC;
        printf("\n--- Mostrados %u/%u registros ---\n", (unsigned)limit, (unsigned)n);
        printf("Comparações: %lld | Trocas: %lld | Tempo acumulado: %.6f s\n",
               comparisons, swaps, block_time);

        shown = limit;
        if (shown >= n) break;

        printf("\nDeseja ver mais 200 registros? (S/N): ");
        scanf("%s", resp);
        if (resp[0] != 'S' && resp[0] != 's') break;
        system("cls");
    }

    printf("\nFim da exibição. Retornando ao menu...\n");
    free(arr);
}

/* ---------- Menu principal ---------- */
void menu_main() {
    int year, field, algo;
    char filename[128];
    size_t n;
    Record *data;

    while (1) {
        printf("\n=== APS - Ordenação de Dados (Pará) ===\n");
        printf("1 - Ano 2023\n2 - Ano 2024\n0 - Sair\nEscolha: ");
        scanf("%d", &year);
        if (year == 0) return;
        if (year == 1) strcpy(filename, "focos_br_pa_ref_2023.csv");
        else if (year == 2) strcpy(filename, "focos_br_pa_ref_2024.csv");
        else continue;

        printf("\nCarregando arquivo %s ...\n", filename);
        data = read_csv(filename, &n);
        printf("Registros carregados: %u\n", (unsigned)n);

        while (1) {
            printf("\nOrdenar por:\n1 - Município\n2 - Bioma\n3 - Data\n4 - Voltar\n0 - Sair\nEscolha: ");
            scanf("%d", &field);
            if (field == 0) { free(data); return; }
            if (field == 4) { free(data); break; }

            Comparator comp = NULL;
            if (field == 1) comp = cmp_municipio;
            else if (field == 2) comp = cmp_bioma;
            else if (field == 3) comp = cmp_datahora;
            else continue;

            printf("\nEscolha o algoritmo:\n");
            printf("1 - Bubble Sort\n2 - Selection Sort\n3 - Insertion Sort\n4 - Shell Sort\n");
            printf("5 - Merge Sort\n6 - Quick Sort\n7 - Heap Sort\n8 - Bogo Sort\n0 - Voltar\nEscolha: ");
            scanf("%d", &algo);
            if (algo == 0) continue;

            run_algorithm(algo, data, n, comp);
        }
    }
}

int main(void) {
    printf("APS - Estrutura de Dados (Pará 2023/2024)\n");
    printf("Certifique-se de que os arquivos CSV estão na mesma pasta do programa.\n");
    menu_main();
    return 0;
}

