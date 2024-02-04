#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

typedef struct {
    char recipient[50];
    char city[50];
    float weight;
    int insurance;
} TParcel;

int compareWeight(const void *a, const void *b) {
    const TParcel *parcelA = (const TParcel *)a;
    const TParcel *parcelB = (const TParcel *)b;

    if (parcelA->weight < parcelB->weight) {
        return -1;
    } else if (parcelA->weight > parcelB->weight) {
        return 1;
    } else {
        return 0;
    }
}

int compareFee (const void *a, const void *b) {
    const TParcel *parcelA = (const TParcel *)a;
    const TParcel *parcelB = (const TParcel *)b;

    float feeA = parcelA->weight * 3.0 + (parcelA->insurance ? 1.5 : 0);
    float feeB = parcelB->weight * 3.0 + (parcelB->insurance ? 1.5 : 0);
    if (feeA < feeB) {
        return -1;
    } else if (feeA > feeB) {
        return 1;
    } else {
        return 0;
    }
}

int compareRecipient(const void *a, const void *b) {
    return strcmp(((TParcel *)a)->recipient, ((TParcel *)b)->recipient);
}

float sumOfWeights(TParcel *parcels, int numberOfParcels, const char *recipient) {
    qsort(parcels, numberOfParcels, sizeof(TParcel), compareRecipient);

    // find the recipient in a sorted array of parcels
    TParcel *result = (TParcel *)bsearch(recipient, parcels, numberOfParcels, sizeof(TParcel), compareRecipient);

    // sum of weights of the recipient
    float sum = 0.0;
    int firstOccurrence;

    if (result != NULL) {
        result++;
        if (strcmp(result->recipient, recipient) == 0) firstOccurrence = true;
        else firstOccurrence = false;
        result--;

        if (firstOccurrence) {
            while (result <= parcels + numberOfParcels && strcmp(result->recipient, recipient) == 0) {
                sum += result->weight;
                result++;
            }
        } else {
            while (result >= parcels && strcmp(result->recipient, recipient) == 0) {
                sum += result->weight;
                result--;
            }
        }
    } else {
        return -1.0;
    }

    return sum;
}

void writeWeights(TParcel *parcels, const char *fileName, const char *destinationCity) {
    FILE *fw = fopen(fileName, "w");
    if (fw == NULL) {
        printf("Error opening output file.\n");
        return;
    }

    int i = 0;

    while (strlen(parcels[i].city) > 0) {
        if (strcmp(parcels[i].city, destinationCity) == 0) {
            fprintf(fw, "%.2f\n", parcels[i].weight);
        }
        i++;
    }

    fclose(fw);
}

int main() {

    // TASK 1
    FILE *fr;
    char fileName[50];
    int numberOfParcels = 0;
    char row[110];

    printf("Enter file name:\n");
    scanf("%49s", fileName);

    char filePath[100];
    sprintf(filePath, "../data/%s", fileName);

    fr = fopen(filePath, "r");
    if (fr == NULL) {
        printf("Error opening file");
        return 1;
    }

    // read number of parcels in first row
    fscanf(fr, "%d", &numberOfParcels);
    while (fgetc(fr) != '\n'); // set pointer to next row

    TParcel* parcels;
    parcels = (TParcel *)malloc(numberOfParcels * sizeof(TParcel));

    // read the file and fill an array
    for (int i = 0; i < numberOfParcels; i++) {
        if (fgets(row, sizeof(row), fr) != NULL) {
            sscanf(row, "%s %s %f %d",
                   parcels[i].recipient,
                   parcels[i].city,
                   &parcels[i].weight,
                   &parcels[i].insurance);
        } else {
            printf("Error reading %d. row in the file.", i + 1);
            break;
        }
    }

    fclose(fr);

    for (int i = 0; i < numberOfParcels; i++) {
        printf("%s %s %.2f %d\n", parcels[i].recipient, parcels[i].city, parcels[i].weight, parcels[i].insurance);
    }

    // TASK 2
    qsort(parcels, numberOfParcels, sizeof(TParcel), compareWeight);
    printf("Sorted based on weight:\n");
    for (int i = 0; i < numberOfParcels; i++) {
        printf("%s %s %.2f %d\n", parcels[i].recipient, parcels[i].city, parcels[i].weight, parcels[i].insurance);
    }

    qsort(parcels, numberOfParcels, sizeof(TParcel), compareFee);
    printf("Sorted based on fee:\n");
    for (int i = 0; i < numberOfParcels; i++) {
        printf("%s %s %.2f %d\n", parcels[i].recipient, parcels[i].city, parcels[i].weight, parcels[i].insurance);
    }

    // TASK 3
    char recipient[50];
    printf("Enter a recipient to get a sum of parcel weights:");
    scanf("%49s", recipient);

    float sum = sumOfWeights(parcels, numberOfParcels, recipient);
    if (sum != -1) {
        printf("%.2f\n", sum);
    } else {
        printf("Recipient not found.\n");
    }

    // TASK 4
    char city[50];
    printf("Enter destination city:");
    scanf("%49s", city);

    int containsCity = false;
    for (int i = 0; i < numberOfParcels; i++) {
        if (strcmp(parcels[i].city,city) == 0) {
            containsCity = true;
        }
    }

    if (containsCity) {
        writeWeights(parcels, "../data/vystup3.txt", city);
    } else {
        printf("City not found.");
    }

    free(parcels);

    return 0;
}
