#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX 100 


typedef struct Node {
    int x, y;              
    int g, h, f;           
    struct Node* parent;   
} Node;


typedef struct {
    Node* nodes[MAX * MAX];
    int size;
} MinHeap;


int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};


int grid[MAX][MAX];
int rows, cols;
Node* came_from[MAX][MAX]; //  backtracking

// create_node
Node* create_node(int x, int y, int g, int h, Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->x = x;
    node->y = y;
    node->g = g;
    node->h = h;
    node->f = g + h;
    node->parent = parent;
    return node;
}

//  Manhattan
int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// init_heap
void init_heap(MinHeap* heap) {
    heap->size = 0;
}

// heap_not_empty
int heap_not_empty(MinHeap* heap) {
    return heap->size > 0;
}

// heap
void heap_insert(MinHeap* heap, Node* node) {
    int i = heap->size++;
    while (i > 0 && node->f < heap->nodes[(i - 1) / 2]->f) {
        heap->nodes[i] = heap->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->nodes[i] = node;
}

// extract
Node* heap_extract(MinHeap* heap) {
    if (heap->size == 0) return NULL;
    Node* min_node = heap->nodes[0];
    Node* last_node = heap->nodes[--heap->size];

    int i = 0;
    while (2 * i + 1 < heap->size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = left;

        if (right < heap->size && heap->nodes[right]->f < heap->nodes[left]->f) {
            smallest = right;
        }

        if (last_node->f <= heap->nodes[smallest]->f) break;

        heap->nodes[i] = heap->nodes[smallest];
        i = smallest;
    }
    heap->nodes[i] = last_node;

    return min_node;
}

// 
void print_grid(const char* message) {
    printf("\n%s\n", message);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 1)
                printf("# "); //empodio
            else if (grid[i][j] == 2)
                printf("x "); // diadromi
            else
                printf(". "); //free scpace
        }
        printf("\n");
    }
}

//  A*
void a_star(int start_x, int start_y, int goal_x, int goal_y) {
    MinHeap open_heap;
    init_heap(&open_heap);

    int closed[MAX][MAX] = {0};

    Node* start_node = create_node(start_x, start_y, 0, manhattan_distance(start_x, start_y, goal_x, goal_y), NULL);
    heap_insert(&open_heap, start_node);

    while (heap_not_empty(&open_heap)) {
        Node* current = heap_extract(&open_heap);

        if (closed[current->x][current->y]) {
            free(current);
            continue;
        }
        closed[current->x][current->y] = 1;
        came_from[current->x][current->y] = current->parent;

        if (current->x == goal_x && current->y == goal_y) {
            Node* path_node = current;
            while (path_node) {
                grid[path_node->x][path_node->y] = 2;
                path_node = path_node->parent;
            }
            print_grid("Grid with path:");
            return;
        }

        for (int i = 0; i < 4; i++) {
            int neighbor_x = current->x + directions[i][0];
            int neighbor_y = current->y + directions[i][1];

            if (neighbor_x < 0 || neighbor_x >= rows || neighbor_y < 0 || neighbor_y >= cols || grid[neighbor_x][neighbor_y] == 1 || closed[neighbor_x][neighbor_y]) {
                continue;
            }

            int g = current->g + 1;
            int h = manhattan_distance(neighbor_x, neighbor_y, goal_x, goal_y);
            Node* neighbor = create_node(neighbor_x, neighbor_y, g, h, current);
            printf("Inserting neighbor (%d, %d) with g=%d, h=%d, f=%d\n", neighbor_x, neighbor_y, g, h, g + h); // Debug print
            heap_insert(&open_heap, neighbor);
        }

        free(current);
    }

    printf("\nNo path found\n");
    print_grid("Final grid (no path):");
}

int main() {
    rows = 10;
    cols = 10;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = (rand() % 10 < 2) ? 1 : 0;
        }
    }

    print_grid("Initial grid:");
    int start_x = 0, start_y = 0;
    int goal_x = rows - 1, goal_y = cols - 1;
    printf("Starting A* algorithm from (%d, %d) to (%d, %d)\n", start_x, start_y, goal_x, goal_y);
    a_star(start_x, start_y, goal_x, goal_y);
    printf("A* algorithm completed\n");
    return 0;
}