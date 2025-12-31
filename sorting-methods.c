#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // For sleep() function
#include <string.h>  // For string manipulation

// ANSI color codes for better visualization
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"
#define RESET   "\x1b[0m"
#define BOLD    "\x1b[1m"
#define CLEAR   "\x1b[2J\x1b[H"

// Function prototypes
void selectionSort(int arr[], int n, int visualize);
void bubbleSort(int arr[], int n, int visualize);
void mergeSort(int arr[], int left, int right, int visualize);
void merge(int arr[], int left, int mid, int right, int visualize);
void quickSort(int arr[], int low, int high, int visualize);
int partition(int arr[], int low, int high, int visualize);
void printArray(int arr[], int n);
void copyArray(int source[], int dest[], int n);
void visualizeSort(int arr[], int n, int index1, int index2, const char* sortName, int color);
void printProgressBar(float progress, int width);
void printMenu();
void printHeader();
void printComparisonChart(double times[], const char* names[], int num_sorts);
void printLegend();

// Global variables for visualization
int visualization_speed = 50000; // microseconds delay
int visualize_enabled = 1;

int main() {
    int n, choice;
    char input[100];

    // Clear screen and print header
    printf(CLEAR);
    printHeader();

    printf("\n\n" BOLD "SORTING ALGORITHM VISUALIZER AND COMPARATOR\n" RESET);
    printf("============================================\n\n");

    // Main menu
    printf("1. Compare all sorting algorithms\n");
    printf("2. Visualize individual algorithm\n");
    printf("3. Performance analysis mode\n");
    printf("4. Configure settings\n");
    printf("5. Exit\n");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    if (choice == 5) {
        return 0;
    }

    if (choice == 4) {
        printf("\n" BOLD "Configuration Menu\n" RESET);
        printf("===================\n");
        printf("1. Enable/Disable Visualization (Currently: %s)\n",
               visualize_enabled ? "Enabled" : "Disabled");
        printf("2. Set Visualization Speed (Currently: %d)\n", visualization_speed);
        printf("3. Return to main menu\n");
        printf("Enter choice: ");
        int config_choice;
        scanf("%d", &config_choice);

        if (config_choice == 1) {
            visualize_enabled = !visualize_enabled;
            printf("Visualization %s\n", visualize_enabled ? "Enabled" : "Disabled");
            sleep(1);
        } else if (config_choice == 2) {
            printf("Enter visualization speed (1-100000 microseconds): ");
            scanf("%d", &visualization_speed);
        }

        // Restart program
        main();
        return 0;
    }

    printf("\nEnter the number of elements (10-50 for visualization, 1000+ for comparison): ");
    scanf("%d", &n);

    // Create and initialize the original array
    int *original = (int*)malloc(n * sizeof(int));
    int *arr = (int*)malloc(n * sizeof(int));

    // Generate random numbers
    srand(time(0));
    printf("\nGenerating array...\n");
    for(int i = 0; i < n; i++) {
        original[i] = rand() % 100 + 1; // Limit to 100 for better visualization
    }

    if (choice == 1) {
        // Clear screen for comparison mode
        printf(CLEAR);
        printHeader();

        // Variables to store execution times
        clock_t start, end;
        double selection_time, bubble_time, merge_time, quick_time;

        printf("\n" BOLD CYAN "COMPARING ALL SORTING ALGORITHMS\n" RESET);
        printf("==================================\n\n");

        // Prepare array for small visualization
        int *small_arr = (int*)malloc(20 * sizeof(int));
        copyArray(original, small_arr, n > 20 ? 20 : n);

        printf("Sample array (first %d elements):\n", n > 20 ? 20 : n);
        for(int i = 0; i < (n > 20 ? 20 : n); i++) {
            printf("[" GREEN "%3d" RESET "] ", small_arr[i]);
        }
        printf("\n\n");

        // 1. Selection Sort
        printf(YELLOW "➤ Selection Sort:\n" RESET);
        printProgressBar(0.1, 50);
        copyArray(original, arr, n);
        start = clock();
        selectionSort(arr, n, 0);
        end = clock();
        selection_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf(" Completed in " GREEN "%.6f" RESET " seconds\n", selection_time);

        // 2. Bubble Sort
        printf(YELLOW "➤ Bubble Sort:\n" RESET);
        printProgressBar(0.3, 50);
        copyArray(original, arr, n);
        start = clock();
        bubbleSort(arr, n, 0);
        end = clock();
        bubble_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf(" Completed in " GREEN "%.6f" RESET " seconds\n", bubble_time);

        // 3. Merge Sort
        printf(YELLOW "➤ Merge Sort:\n" RESET);
        printProgressBar(0.6, 50);
        copyArray(original, arr, n);
        start = clock();
        mergeSort(arr, 0, n - 1, 0);
        end = clock();
        merge_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf(" Completed in " GREEN "%.6f" RESET " seconds\n", merge_time);

        // 4. Quick Sort
        printf(YELLOW "➤ Quick Sort:\n" RESET);
        printProgressBar(0.9, 50);
        copyArray(original, arr, n);
        start = clock();
        quickSort(arr, 0, n - 1, 0);
        end = clock();
        quick_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf(" Completed in " GREEN "%.6f" RESET " seconds\n", quick_time);

        // Display results in a table
        printf("\n" BOLD "RESULTS SUMMARY\n" RESET);
        printf("+-----------------+-----------------+\n");
        printf("| Algorithm       | Time (seconds)  |\n");
        printf("+-----------------+-----------------+\n");
        printf("| Selection Sort  | %15.6f |\n", selection_time);
        printf("| Bubble Sort     | %15.6f |\n", bubble_time);
        printf("| Merge Sort      | %15.6f |\n", merge_time);
        printf("| Quick Sort      | %15.6f |\n", quick_time);
        printf("+-----------------+-----------------+\n\n");

        // Performance comparison with chart
        double times[] = {selection_time, bubble_time, merge_time, quick_time};
        const char* names[] = {"Selection", "Bubble", "Merge", "Quick"};
        printComparisonChart(times, names, 4);

        free(small_arr);

    } else if (choice == 2) {
        // Individual algorithm visualization
        printf(CLEAR);
        printHeader();

        printf("\n" BOLD CYAN "ALGORITHM VISUALIZATION MODE\n" RESET);
        printf("================================\n\n");

        printf("Select algorithm to visualize:\n");
        printf("1. " YELLOW "Selection Sort\n" RESET);
        printf("2. " GREEN "Bubble Sort\n" RESET);
        printf("3. " BLUE "Merge Sort\n" RESET);
        printf("4. " MAGENTA "Quick Sort\n" RESET);
        printf("5. View all algorithms\n");
        printf("\nEnter choice: ");
        int algo_choice;
        scanf("%d", &algo_choice);

        if (algo_choice >= 1 && algo_choice <= 5) {
            printf(CLEAR);
            printHeader();

            // Limit n for visualization
            if (n > 30) n = 30;

            // Regenerate array with smaller values for visualization
            for(int i = 0; i < n; i++) {
                original[i] = rand() % 50 + 1;
            }

            printf("\nInitial array (%d elements):\n", n);
            printArray(original, n);
            printf("\n");

            copyArray(original, arr, n);

            switch(algo_choice) {
                case 1:
                    printf(BOLD YELLOW "VISUALIZING SELECTION SORT\n" RESET);
                    printf("===========================\n\n");
                    selectionSort(arr, n, 1);
                    break;
                case 2:
                    printf(BOLD GREEN "VISUALIZING BUBBLE SORT\n" RESET);
                    printf("=======================\n\n");
                    bubbleSort(arr, n, 1);
                    break;
                case 3:
                    printf(BOLD BLUE "VISUALIZING MERGE SORT\n" RESET);
                    printf("======================\n\n");
                    mergeSort(arr, 0, n - 1, 1);
                    break;
                case 4:
                    printf(BOLD MAGENTA "VISUALIZING QUICK SORT\n" RESET);
                    printf("======================\n\n");
                    quickSort(arr, 0, n - 1, 1);
                    break;
                case 5:
                    // Show all algorithms in sequence
                    printf(BOLD "VISUALIZING ALL ALGORITHMS\n" RESET);
                    printf("===========================\n\n");

                    // Selection Sort
                    printf(YELLOW "\n● SELECTION SORT:\n" RESET);
                    copyArray(original, arr, n);
                    selectionSort(arr, n, 1);
                    sleep(1);

                    // Bubble Sort
                    printf(GREEN "\n● BUBBLE SORT:\n" RESET);
                    copyArray(original, arr, n);
                    bubbleSort(arr, n, 1);
                    sleep(1);

                    // Merge Sort
                    printf(BLUE "\n● MERGE SORT:\n" RESET);
                    copyArray(original, arr, n);
                    mergeSort(arr, 0, n - 1, 1);
                    sleep(1);

                    // Quick Sort
                    printf(MAGENTA "\n● QUICK SORT:\n" RESET);
                    copyArray(original, arr, n);
                    quickSort(arr, 0, n - 1, 1);
                    break;
            }

            printf("\n\n" BOLD GREEN "✓ Sorting completed!\n" RESET);
            printf("\nSorted array:\n");
            printArray(arr, n);
        }
    } else if (choice == 3) {
        // Performance analysis mode
        printf(CLEAR);
        printHeader();

        printf("\n" BOLD CYAN "PERFORMANCE ANALYSIS MODE\n" RESET);
        printf("============================\n\n");

        int sizes[] = {100, 500, 1000, 5000, 10000};
        int num_sizes = 5;

        printf("Testing with different array sizes:\n");
        printf("+----------+---------------+---------------+---------------+---------------++\n");
        printf("| Size     | Selection     | Bubble        | Merge         | Quick         |\n");
        printf("+----------+---------------+---------------+---------------+---------------++\n");

        for (int i = 0; i < num_sizes; i++) {
            int test_size = sizes[i];
            if (test_size > n && i > 0) break;

            int *test_arr = (int*)malloc(test_size * sizeof(int));
            for (int j = 0; j < test_size; j++) {
                test_arr[j] = rand() % 10000;
            }

            clock_t start, end;
            double times[4];

            // Time each algorithm
            int *temp = (int*)malloc(test_size * sizeof(int));

            copyArray(test_arr, temp, test_size);
            start = clock();
            selectionSort(temp, test_size, 0);
            end = clock();
            times[0] = ((double)(end - start)) / CLOCKS_PER_SEC;

            copyArray(test_arr, temp, test_size);
            start = clock();
            bubbleSort(temp, test_size, 0);
            end = clock();
            times[1] = ((double)(end - start)) / CLOCKS_PER_SEC;

            copyArray(test_arr, temp, test_size);
            start = clock();
            mergeSort(temp, 0, test_size - 1, 0);
            end = clock();
            times[2] = ((double)(end - start)) / CLOCKS_PER_SEC;

            copyArray(test_arr, temp, test_size);
            start = clock();
            quickSort(temp, 0, test_size - 1, 0);
            end = clock();
            times[3] = ((double)(end - start)) / CLOCKS_PER_SEC;

            printf("| %8d | %13.6f | %13.6f | %13.6f | %13.6f |\n",
                   test_size, times[0], times[1], times[2], times[3]);

            free(test_arr);
            free(temp);
        }
        printf("+----------+---------------+---------------+---------------+---------------++\n");
    }

    printf("\n\n" BOLD "Press Enter to continue..." RESET);
    getchar(); // Clear buffer
    getchar(); // Wait for Enter

    // Free allocated memory
    free(original);
    free(arr);

    // Restart
    main();
    return 0;
}

// Print header with ASCII art
void printHeader() {
    printf(CYAN BOLD);
    printf("╔══════════════════════════════════════════════════════╗\n");
    printf("║                                                      ║\n");
    printf("║      SORTING ALGORITHM VISUALIZER                    ║\n");
    printf("║      ===========================                    ║\n");
    printf("║                                                      ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n");
    printf(RESET);
}

// Print progress bar
void printProgressBar(float progress, int width) {
    printf("[");
    int pos = width * progress;
    for (int i = 0; i < width; i++) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %d%%\r", (int)(progress * 100.0));
    fflush(stdout);
}

// Print comparison chart
void printComparisonChart(double times[], const char* names[], int num_sorts) {
    printf(BOLD "\nPERFORMANCE COMPARISON CHART:\n" RESET);
    printf("(Bar length represents relative speed)\n\n");

    // Find maximum time for scaling
    double max_time = 0;
    for (int i = 0; i < num_sorts; i++) {
        if (times[i] > max_time) max_time = times[i];
    }

    // Print bars
    for (int i = 0; i < num_sorts; i++) {
        printf("%-12s: ", names[i]);

        // Choose color based on algorithm
        if (strcmp(names[i], "Selection") == 0) printf(YELLOW);
        else if (strcmp(names[i], "Bubble") == 0) printf(GREEN);
        else if (strcmp(names[i], "Merge") == 0) printf(BLUE);
        else if (strcmp(names[i], "Quick") == 0) printf(MAGENTA);

        int bar_length = 50 - (int)((times[i] / max_time) * 50);
        if (bar_length < 1) bar_length = 1;

        for (int j = 0; j < bar_length; j++) {
            printf("█");
        }
        printf(" %12.6f seconds\n" RESET, times[i]);
    }

    // Determine fastest and slowest
    double fastest = times[0];
    double slowest = times[0];
    int fastest_idx = 0, slowest_idx = 0;

    for (int i = 1; i < num_sorts; i++) {
        if (times[i] < fastest) {
            fastest = times[i];
            fastest_idx = i;
        }
        if (times[i] > slowest) {
            slowest = times[i];
            slowest_idx = i;
        }
    }

    printf("\n" BOLD "Analysis:\n" RESET);
    printf("• Fastest: %s Sort (%.6f seconds)\n", names[fastest_idx], fastest);
    printf("• Slowest: %s Sort (%.6f seconds)\n", names[slowest_idx], slowest);
    printf("• Speed ratio (Slowest/Fastest): %.2fx\n", slowest/fastest);
}

// Visualization function
void visualizeSort(int arr[], int n, int index1, int index2, const char* sortName, int color) {
    if (!visualize_enabled) return;

    printf(CLEAR);
    printHeader();

    printf("\n" BOLD);
    switch(color) {
        case 1: printf(YELLOW); break;
        case 2: printf(GREEN); break;
        case 3: printf(BLUE); break;
        case 4: printf(MAGENTA); break;
    }
    printf("%s IN PROGRESS\n" RESET, sortName);
    printf("====================================\n\n");

    // Calculate maximum value for scaling
    int max_val = arr[0];
    for(int i = 1; i < n; i++) {
        if(arr[i] > max_val) max_val = arr[i];
    }

    // Print array as bars
    for(int i = 0; i < n; i++) {
        int bar_length = (arr[i] * 50) / max_val;
        if (bar_length < 1) bar_length = 1;

        // Highlight the elements being compared/swapped
        if (i == index1 || i == index2) {
            printf(RED BOLD);
        } else {
            switch(color) {
                case 1: printf(YELLOW); break;
                case 2: printf(GREEN); break;
                case 3: printf(BLUE); break;
                case 4: printf(MAGENTA); break;
            }
        }

        // Print the bar
        for(int j = 0; j < bar_length; j++) {
            printf("█");
        }
        printf(" %3d\n" RESET, arr[i]);
    }

    usleep(visualization_speed); // Delay for visualization
}

// Modified sorting functions with visualization
void selectionSort(int arr[], int n, int visualize) {
    int i, j, min_idx, temp;

    for(i = 0; i < n - 1; i++) {
        min_idx = i;
        for(j = i + 1; j < n; j++) {
            if(visualize) visualizeSort(arr, n, j, min_idx, "SELECTION SORT", 1);
            if(arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        // Swap
        temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
        if(visualize) visualizeSort(arr, n, i, min_idx, "SELECTION SORT", 1);
    }
    if(visualize) visualizeSort(arr, n, -1, -1, "SELECTION SORT - COMPLETED", 1);
}

void bubbleSort(int arr[], int n, int visualize) {
    int i, j, temp;
    for(i = 0; i < n - 1; i++) {
        for(j = 0; j < n - i - 1; j++) {
            if(visualize) visualizeSort(arr, n, j, j+1, "BUBBLE SORT", 2);
            if(arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                if(visualize) visualizeSort(arr, n, j, j+1, "BUBBLE SORT", 2);
            }
        }
    }
    if(visualize) visualizeSort(arr, n, -1, -1, "BUBBLE SORT - COMPLETED", 2);
}

void merge(int arr[], int left, int mid, int right, int visualize) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));

    for(i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for(j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while(i < n1 && j < n2) {
        if(visualize) visualizeSort(arr, right+1, left+i, mid+1+j, "MERGE SORT", 3);
        if(L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while(i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while(j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int left, int right, int visualize) {
    if(left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid, visualize);
        mergeSort(arr, mid + 1, right, visualize);
        if(visualize) visualizeSort(arr, right+1, left, right, "MERGE SORT", 3);
        merge(arr, left, mid, right, visualize);
    }
}

int partition(int arr[], int low, int high, int visualize) {
    int pivot = arr[high];
    int i = (low - 1);
    int temp;

    for(int j = low; j <= high - 1; j++) {
        if(visualize) visualizeSort(arr, high+1, j, high, "QUICK SORT", 4);
        if(arr[j] < pivot) {
            i++;
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            if(visualize) visualizeSort(arr, high+1, i, j, "QUICK SORT", 4);
        }
    }

    temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return (i + 1);
}

void quickSort(int arr[], int low, int high, int visualize) {
    if(low < high) {
        int pi = partition(arr, low, high, visualize);

        quickSort(arr, low, pi - 1, visualize);
        quickSort(arr, pi + 1, high, visualize);
    }
    if(visualize && low == 0 && high > low)
        visualizeSort(arr, high+1, -1, -1, "QUICK SORT - PARTITION", 4);
}

void printArray(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        if(i % 10 == 0) printf("\n");
        printf("[%3d] ", arr[i]);
    }
    printf("\n");
}

void copyArray(int source[], int dest[], int n) {
    for(int i = 0; i < n; i++) {
        dest[i] = source[i];
    }
}
