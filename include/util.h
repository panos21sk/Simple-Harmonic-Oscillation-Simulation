#include <math.h>

//when not over anything -> crash
int valueAt(int array[], int size, int value){
    for(int i = 0; i < size; i++){
        if (array[i] == value) return i;
    }
}

int findMinIndex(int array[], int size){
    int min;
    for(int i = 0; i < size; i++){ if (array[i] > 0) {min = array[i]; break; } }
    for(int i = 0; i < size; i++){
    //printf("%i,", array[i]);
        if (array[i] < min && array[i] >= 0) min = array[i];
    }
    
    //printf("\nminval: %i\n", min);
    return valueAt(array, size, min);
}

