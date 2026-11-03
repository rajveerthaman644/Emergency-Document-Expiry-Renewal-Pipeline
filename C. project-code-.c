#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 64

// Converts a Gregorian date (DD, MM, YYYY) to Julian Day Number
long dateToJulianDays(int d, int m, int y) {
    if (m < 3) {
        y--;
        m += 12;
    }
    return 365L * y + y / 4 - y / 100 + y / 400 + (153 * (m - 3) + 2) / 5 + d;
}

// Calculates exact difference in days between two dates
long getDaysRemaining(int curD, int curM, int curY, int expD, int expM, int expY) {
    long currentJDN = dateToJulianDays(curD, curM, curY);
    long expiryJDN = dateToJulianDays(expD, expM, expY);
    return expiryJDN - currentJDN;
}

// Swaps string pointers
void swapStrings(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

// Swaps long integers
void swapLongs(long *a, long *b) {
    long temp = *a;
    *a = *b;
    *b = temp;
}

// Swaps standard integers
void swapInts(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Swaps all parallel fields for index i and j
void swapRecords(char **docNames, char **owners, char **expiryDates, 
                 int *leadDays, long *remainingDays, int i, int j) {
    swapStrings(&docNames[i], &docNames[j]);
    swapStrings(&owners[i], &owners[j]);
    swapStrings(&expiryDates[i], &expiryDates[j]);
    swapInts(&leadDays[i], &leadDays[j]);
    swapLongs(&remainingDays[i], &remainingDays[j]);
}

// Partition step for QuickSort
int partition(char **docNames, char **owners, char **expiryDates, 
              int *leadDays, long *remainingDays, int low, int high) {
    long pivot = remainingDays[high];
    int i = (low - 1);

    for (int j = low; j < high; j++) {
        // Ascending order: lowest days remaining float to the top
        if (remainingDays[j] <= pivot) {
            i++;
            swapRecords(docNames, owners, expiryDates, leadDays, remainingDays, i, j);
        }
    }
    swapRecords(docNames, owners, expiryDates, leadDays, remainingDays, i + 1, high);
    return (i + 1);
}

// Recursive QuickSort to order documents by urgency
void recursiveSortByUrgency(char **docNames, char **owners, char **expiryDates, 
                            int *leadDays, long *remainingDays, int low, int high) {
    if (low < high) {
        int pi = partition(docNames, owners, expiryDates, leadDays, remainingDays, low, high);
        recursiveSortByUrgency(docNames, owners, expiryDates, leadDays, remainingDays, low, pi - 1);
        recursiveSortByUrgency(docNames, owners, expiryDates, leadDays, remainingDays, pi + 1, high);
    }
}

// Prompts and registers a new document
void addDocument(char ***docNames, char ***owners, char ***expiryDates, 
                 int **leadDays, long **remainingDays, int *count,
                 int curD, int curM, int curY) {
    int n = *count;

    // Dynamically resize parallel arrays
    *docNames = (char **)realloc(*docNames, (n + 1) * sizeof(char *));
    *owners = (char **)realloc(*owners, (n + 1) * sizeof(char *));
    *expiryDates = (char **)realloc(*expiryDates, (n + 1) * sizeof(char *));
    *leadDays = (int *)realloc(*leadDays, (n + 1) * sizeof(int));
    *remainingDays = (long *)realloc(*remainingDays, (n + 1) * sizeof(long));

    (*docNames)[n] = (char *)malloc(MAX_STR_LEN * sizeof(char));
    (*owners)[n] = (char *)malloc(MAX_STR_LEN * sizeof(char));
    (*expiryDates)[n] = (char *)malloc(MAX_STR_LEN * sizeof(char));

    printf("\n--- Register Document ---\n");
    printf("Document Name (e.g., Car_Insurance, Passport): ");
    scanf("%63s", (*docNames)[n]);

    printf("Owner Name (e.g., Self, Dad): ");
    scanf("%63s", (*owners)[n]);

    int expD, expM, expY;
    printf("Expiry Date (DD MM YYYY): ");
    scanf("%d %d %d", &expD, &expM, &expY);
    snprintf((*expiryDates)[n], MAX_STR_LEN, "%02d/%02d/%04d", expD, expM, expY);

    printf("Required Renewal Lead Time (in days, e.g., 7, 30): ");
    scanf("%d", &(*leadDays)[n]);

    (*remainingDays)[n] = getDaysRemaining(curD, curM, curY, expD, expM, expY);

    (*count)++;
    printf("Document registered successfully.\n");
}

// Displays all records sorted by urgency
void displayPipeline(char **docNames, char **owners, char **expiryDates, 
                     int *leadDays, long *remainingDays, int count) {
    if (count == 0) {
        printf("\nNo active documents in the pipeline.\n");
        return;
    }

    // Sort by urgency before rendering
    recursiveSortByUrgency(docNames, owners, expiryDates, leadDays, remainingDays, 0, count - 1);

    printf("\n========================================================================================\n");
    printf("%-4s | %-16s | %-10s | %-10s | %-10s | %-18s\n", 
           "No.", "Document", "Owner", "Expiry", "Days Left", "Pipeline Status");
    printf("========================================================================================\n");

    for (int i = 0; i < count; i++) {
        char status[32];
        if (remainingDays[i] < 0) {
            snprintf(status, sizeof(status), "[EXPIRED / BREACH]");
        } else if (remainingDays[i] <= leadDays[i]) {
            snprintf(status, sizeof(status), "[ACTION REQUIRED]");
        } else {
            snprintf(status, sizeof(status), "[VALID / SAFE]");
        }

        printf("%-4d | %-16s | %-10s | %-10s | %-10ld | %-18s\n", 
               i + 1, docNames[i], owners[i], expiryDates[i], remainingDays[i], status);
    }
    printf("========================================================================================\n");
}

// Frees all heap allocations
void freeMemory(char **docNames, char **owners, char **expiryDates, 
                int *leadDays, long *remainingDays, int count) {
    for (int i = 0; i < count; i++) {
        free(docNames[i]);
        free(owners[i]);
        free(expiryDates[i]);
    }
    free(docNames);
    free(owners);
    free(expiryDates);
    free(leadDays);
    free(remainingDays);
}

int main() {
    char **docNames = NULL;
    char **owners = NULL;
    char **expiryDates = NULL;
    int *leadDays = NULL;
    long *remainingDays = NULL;
    int count = 0;

    int curD, curM, curY;
    printf("====================================================\n");
    printf("       EMERGENCY DOCUMENT & RENEWAL PIPELINE         \n");
    printf("====================================================\n");
    printf("Enter Today's Date (DD MM YYYY): ");
    if (scanf("%d %d %d", &curD, &curM, &curY) != 3) {
        printf("Invalid date input.\n");
        return 1;
    }

    int choice;
    do {
        printf("\n--- Navigation Menu ---\n");
        printf("1. Register New Document\n");
        printf("2. View Expiry & Renewal Pipeline (Sorted)\n");
        printf("3. Exit\n");
        printf("Selection: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                addDocument(&docNames, &owners, &expiryDates, 
                            &leadDays, &remainingDays, &count, 
                            curD, curM, curY);
                break;
            case 2:
                displayPipeline(docNames, owners, expiryDates, 
                                leadDays, remainingDays, count);
                break;
            case 3:
                printf("\nFreeing allocated heap memory and exiting...\n");
                break;
            default:
                printf("Invalid selection. Try again.\n");
        }
    } while (choice != 3);

    freeMemory(docNames, owners, expiryDates, leadDays, remainingDays, count);
    return 0;
}