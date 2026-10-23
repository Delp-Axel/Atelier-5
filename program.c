#include <stdio.h>
#include <stdbool.h>

// Étape 1 : Dimensions du monde
#define WIDTH 10
#define HEIGHT 10

int world[HEIGHT][WIDTH] = {0};

// Étape 2 : Fonctions d'état
bool cell_cur_state(int grid[HEIGHT][WIDTH], int y, int x) {
    // L'état actuel correspond aux unités : modulo 10
    return (grid[y][x] % 10) == 1;
}

bool cell_next_state(int grid[HEIGHT][WIDTH], int y, int x) {
    // L'état futur correspond aux dizaines : division entière par 10
    return (grid[y][x] / 10) == 1;
}

// Étape 1 : Affichage du monde (état actuel)
void print_world(int grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (cell_cur_state(grid, i, j)) {
                printf("O "); // Cellule vivante
            } else {
                printf(". "); // Cellule morte
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Étape 3 : Calcul du futur état pour une cellule (Monde torique)
void cell_compute_next_state(int grid[HEIGHT][WIDTH], int y, int x) {
    int alive_neighbors = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue; // On ignore la cellule elle-même

            // Gestion du monde torique avec l'opérateur modulo
            int ny = (y + dy + HEIGHT) % HEIGHT;
            int nx = (x + dx + WIDTH) % WIDTH;

            if (cell_cur_state(grid, ny, nx)) {
                alive_neighbors++;
            }
        }
    }

    bool current = cell_cur_state(grid, y, x);
    bool next = false;

    // Règles du Jeu de la vie de Conway
    if (current && (alive_neighbors == 2 || alive_neighbors == 3)) {
        next = true;
    } else if (!current && alive_neighbors == 3) {
        next = true;
    }

    // Codage sur 2 chiffres : dizaine (futur), unité (actuel)
    int current_val = current ? 1 : 0;
    int next_val = next ? 1 : 0;
    grid[y][x] = (next_val * 10) + current_val;
}

// Initialisation des motifs
void init_patterns(int grid[HEIGHT][WIDTH]) {
    // Stable : Carré (2x2) en haut à gauche
    grid[1][1] = 1; grid[1][2] = 1;
    grid[2][1] = 1; grid[2][2] = 1;

    // Oscillateur : Clignotant (3x1) au centre
    grid[5][4] = 1; grid[5][5] = 1; grid[5][6] = 1;

    // Vaisseau : Planeur (Glider) en bas à droite
    grid[7][8] = 1;
    grid[8][9] = 1;
    grid[9][7] = 1; grid[9][8] = 1; grid[9][9] = 1;
}

int main() {
    init_patterns(world);

    printf("--- État initial ---\n");
    print_world(world);

    // Simulation de 5 générations
    for (int gen = 1; gen <= 5; gen++) {
        // Phase 1 : Calcul de l'état futur pour chaque cellule
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                cell_compute_next_state(world, i, j);
            }
        }

        // Phase 2 : Application du nouvel état (le futur devient l'actuel)
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                // Remplacement de l'unité par la dizaine
                bool next = cell_next_state(world, i, j);
                world[i][j] = next ? 1 : 0;
            }
        }

        printf("--- Génération %d ---\n", gen);
        print_world(world);
    }

    return 0;
}