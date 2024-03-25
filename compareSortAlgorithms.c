/*
	Edited by Daniel Nunez
	3/12/2024
	CS1 Bonus Assignment
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int extraMemoryAllocated;

void *Alloc(size_t sz) {
  extraMemoryAllocated += sz;
  size_t *ret = malloc(sizeof(size_t) + sz);
  *ret = sz;
	if (ret == NULL) exit(EXIT_FAILURE);	// Checks if 'ret' returns NULL
  printf("Extra memory allocated, size: %ld\n", sz);
  return &ret[1];
}

void DeAlloc(void *ptr) {
  size_t *pSz = (size_t *)ptr - 1;
  extraMemoryAllocated -= *pSz;
  printf("Extra memory deallocated, size: %ld\n", *pSz);
  free((size_t *)ptr - 1);
}

//size_t represents size of an object in bytes
size_t Size(void *ptr) { return ((size_t *)ptr)[-1]; }

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz) {
  int i, sz = dataSz - 100;
  printf("\tData:\n\t");
	
  for (i = 0; i < 100; ++i)	printf("%d ", pData[i]);
  printf("\n\t");

  for (i = sz; i < dataSz; ++i)	printf("%d ", pData[i]);
  printf("\n\n");
}

// helper function for heapSort
void heapify(int array[], int n, int i) {
  int max, temp, leftIndex, rightIndex;
  max = i;                // assuming 'i' position is largest value
  leftIndex = 2 * i + 1;  // left child index
  rightIndex = 2 * i + 2; // right child index

  if (leftIndex < n && (array[leftIndex] > array[max]))
    max = leftIndex;
  if (rightIndex < n && array[rightIndex] > array[max])
    max = rightIndex;
  if (max != i) {
    temp = array[i];
    array[i] = array[max];
    array[max] = temp;
    heapify(array, n, max);
  }	//Ends if statement
} // end heapify

//Heap sort
void heapSort(int array[], int n) {
  int temp, i;
  for (i = n / 2 - 1; i >= 0; i--)
    heapify(array, n, i);
  for (i = n - 1; i > 0; i--) {
    temp = array[0];
    array[0] = array[i];
    array[i] = temp;
    heapify(array, i, 0);
  }
} // end heapSort

// Helper function for mergeSort
void merge(int array[], int left, int middle, int right) {
  int i, j, k;
  int n1 = middle - left + 1;
  int n2 = right - middle;
  int *leftPoint = (int *)Alloc(sizeof(int) * n1);
  int *rightPoint = (int *)Alloc(sizeof(int) * n2);

  for (i = 0; i < n1; i++)
    leftPoint[i] = array[left + i];
  for (j = 0; j < n2; j++)
    rightPoint[j] = array[middle + 1 + j];
  i = 0;
  j = 0;
  k = left;
  while (i < n1 && j < n2) {
    if (leftPoint[i] <= rightPoint[j]) {
      array[k] = leftPoint[i];
      i++;
    } else {
      array[k] = rightPoint[j];
      j++;
    }
    k++;
  } // ends while loop
  while (i < n1) {
    array[k] = leftPoint[i];
    i++;
    k++;
  } // ends while loop
  while (j < n2) {
    array[k] = rightPoint[j];
    j++;
    k++;
  } // ends while loop
  DeAlloc(leftPoint);
  DeAlloc(rightPoint);
} // Ends merge function

//Merge sort
void mergeSort(int pData[], int l, int r) {
  if (l < r) {
    int middle = (l + r) / 2;
    mergeSort(pData, l, middle);
    mergeSort(pData, middle + 1, r);

    merge(pData, l, middle, r);
  }	// Ends if statement
} // Ends mergeSort

//Insertion sort
void insertionSort(int *pData, int n) {
  int i, j, temp;
  for (i = 1; i < n; i++) {
    temp = pData[i];
    for (j = i - 1; j >= 0; j--) {
      if (pData[j] > temp)
        pData[j + 1] = pData[j];
      else
        break;
    } // Ends j for loop
    pData[j + 1] = temp;
  } // Ends i for loop
}	// Ends insertionSort

//Bubble sort
void bubbleSort(int *pData, int n) {
  int temp;
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (pData[j] > pData[j + 1]) { // Swap values
        temp = pData[j];
        pData[j] = pData[j + 1];
        pData[j + 1] = temp;
      } // Ends if statement
    }   // Ends j for loop
  }     // Ends i for loop
  printArray(pData, n);
}	// Ends bubbleSort

//Selection sort
void selectionSort(int *pData, int n) {
  int i, j, temp, minIndex;
  for (i = 0; i < n - 1; i++) {
    minIndex = i; // finds min. element in unsorted array
    for (j = i + 1; j < n; j++) {
      if (pData[j] < pData[minIndex])
        minIndex = j;
    } // Ends j for loop
    // now swaps found min. element w/ first element
    temp = pData[i];
    pData[i] = pData[minIndex];
    pData[minIndex] = temp;
  } // Ends i for loop
  printArray(pData, n);
}	// Ends selectionSort

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData) {
  FILE *inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  int i, n, *data;
  *ppData = NULL;

  if (inFile) {
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = (int *)Alloc(sizeof(int) * dataSz);
    if (*ppData == NULL) {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i = 0; i < dataSz; ++i) {
      fscanf(inFile, "%d ", &n);
      data = *ppData + i;
      *data = n;
    }
    fclose(inFile);
  }
  return dataSz;
}

int main(void) {
  clock_t start, end;
  int i;
  double cpu_time_used;
  char *fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};

  for (i = 0; i < 3; ++i) {
    int *pDataSrc, *pDataCopy;
    int dataSz = parseData(fileNames[i], &pDataSrc);

    if (dataSz <= 0) continue;

    pDataCopy = (int *)Alloc(sizeof(int) * dataSz);

    printf("---------------------------\n");
    printf("Dataset Size : %d\n", dataSz);
    printf("---------------------------\n");

    printf("Selection Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    selectionSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
    printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Insertion Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    insertionSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
    printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    printf("Bubble Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    bubbleSort(pDataCopy, dataSz);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
    printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz); // edited to heapSort(pDataCopy, dataSz); instead of HeapSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
    printf("Merge Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    mergeSort(pDataCopy, 0, dataSz - 1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
    printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    DeAlloc(pDataCopy);
    DeAlloc(pDataSrc);
  }
}