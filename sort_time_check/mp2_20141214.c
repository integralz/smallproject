#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>


// bubble sort
void algorithm1(int array[], int num) {
	int change;
	for (int a = 0; a < num - 1; a++)
		for (int b = 1; b < num - a; b++) {
			if (array[b] < array[b - 1]) {
				change = array[b];
				array[b] = array[b - 1];
				array[b - 1] = change;
			}
		}


}
//quick sort
//partition 함수는 주어진 수 중에서 마지막에 위치한 수와 나머지 수들을 비교하여 작은 수들은 앞에 
//큰 수들은 뒤에 위치시키고 마지막에 있던 수가 몇번째로 작은지에 대한 정보 호출
int partition(int low, int high, int array[]) {
	int last_num = array[high];
	int pivot = low;
	int change;

	for (int a = low; a < high; a++) {
		if (array[a] <= last_num) {
			change = array[pivot];
			array[pivot++] = array[a];
			array[a] = change;
		}
	}
	change = array[pivot];
	array[pivot] = array[high];
	array[high] = change;

	return pivot;
}
//partiton 함수에서 만들어진 정보를 바탕으로 작은수들의 모임, 큰 수들의 모임 계속해서 재귀함수로 분할
void divide(int low, int high, int array[]) {
	int divide_num;

	if (low >= high);
	else {
		divide_num = partition(low, high, array);
		divide(low, divide_num - 1, array);
		divide(divide_num + 1, high, array);
	}
}
//quick sort함수 호출
void algorithm2(int array[], int num) {
	divide(0, num - 1, array);

}

// merge sort
// merge함수롤 인해 쪼개진 인접한 배열을 다시 순서에 맞게 재편성하여 붙이는 함수
void combine_merge(int low, int high, int array[]) {
	int first_comparison = low, second_comparision = (low + high) / 2 + 1;
	int *copy = (int*)malloc(sizeof(int) * (high - low + 1));
	int num = 0;


	while (first_comparison <= (low + high) / 2 && second_comparision <= high) {
		if (array[first_comparison] < array[second_comparision])
			copy[num++] = array[first_comparison++];
		else if (array[first_comparison] > array[second_comparision])
			copy[num++] = array[second_comparision++];
		else {
			copy[num++] = array[first_comparison++];
			copy[num++] = array[second_comparision++];
		}
	}
	while (first_comparison <= (low + high) / 2)
		copy[num++] = array[first_comparison++];
	while (second_comparision <= high)
		copy[num++] = array[second_comparision++];

	for (int a = low; a <= high; a++)
		array[a] = copy[a - low];
	free(copy);
}
//주어진 배열을 최소단위까지 쪼갠 후 순서에 맞게 다시 붙이는 재귀함수
void merge(int low, int high, int array[]) {
	if (low == high);
	else {
		int mid = (low + high) / 2;

		merge(low, mid, array);
		merge(mid + 1, high, array);

		combine_merge(low, high, array);
	}

}
//merge sort 호출 함수
void algorithm3(int array[], int num) {
	merge(0, num - 1, array);
}

//own algorithm(intro sort)
//insertion sort함수
void insertion(int array[], int left, int right) {
	int temp, b;
	for (int a = left + 1; a <= right; a++) {
		temp = array[a];
		b = a;
		while (b > 0 && temp < array[b - 1]) {
			array[b] = array[b - 1];
			b--;
		}
		array[b] = temp;
	}
}
//num개의 수로 이루어진 이진트리를 받아 a번째에 속한 수를 max heap로 정렬하는 함수 
void adjust(int copy[], int a, int num) {
	int temp = copy[a];
	int child = 2 * a;
	while (child <= num) {
		if (child == num)
			if (copy[child] > temp) {
				copy[child / 2] = copy[child];
				child *= 2;
			}
			else break;
		else {
			if (copy[child] < copy[child + 1]) {
				child++;
				if (copy[child] > temp) {
					copy[child / 2] = copy[child];
					child *= 2;
				}
				else break;
			}
			else {
				if (copy[child] > temp) {
					copy[child / 2] = copy[child];
					child *= 2;
				}
				else break;
			}
		}
	}
	copy[child / 2] = temp;
}
//heap sort 호출 함수 처음 array를 받으면 max heap로 정렬 후 max 값을 하나 씩 빼냄
void heapsort(int array[], int left, int right) {
	int change;
	int num = right - left + 1;
	int *copy = (int*)malloc(sizeof(int)*(num + 1));
	for (int a = 0; a < num; a++)
		copy[a + 1] = array[a + left];
	
	for (int a = num / 2; a >= 1; a--)
		adjust(copy, a, num);
	for (int a = num - 1; a >= 1; a--) {
		change = copy[1];
		copy[1] = copy[a + 1];
		copy[a + 1] = change;
		adjust(copy, 1, a);
	}
	for (int a = 0; a < num; a++)
		array[a + left] = copy[a + 1];
	free(copy);
}

//2번 함수인 quick sort에서 깊이를 추가하여 하나씩 줄여나가면서 quick sort를 진행하다가
//depth가 0이 되면 insertion 혹은 heap sort로 전환
void countdivide(int low, int high, int array[], int depth) {
	int divide_num;

	if (low >= high);
	else {
		if (depth > 0) {
			divide_num = partition(low, high, array);
			countdivide(low, divide_num - 1, array, depth - 1);
			countdivide(divide_num + 1, high, array, depth -1);
		}
		else {
			if (high - low + 1 <= 16)
				insertion(array, low, high);
			else
				heapsort(array, low, high);
		}
	}
}
//intro sort를 구동하는 함수
//숫자 갯수가 16개 이하면 insertion으로, 초과면 quick sort로 정렬하되 depth를 
//2*log2(num)으로 지정하여 시간복잡도를 최적화
void algorithm4(int array[], int num) {
	int max_depth = 2 * log2(num);
	if (num <= 16)
		insertion(array, 0, num - 1);
	else {
		countdivide(0, num - 1, array, max_depth);
	}
	
}


int main(int argc, char *argv[]) {
	int num, use;
	int *array;
	// ./mp2_20141214 input00001.txt 2 와 같은 꼴이 아닐시 오류메세지 호출
	if (argc != 3)
		printf("usage: %s filename input_filename number_of_algorithm\n", argv[0]);
	char inputfilename[200], outputfilename[215] = "result_";
	//호출 받은 input파일 이름 복사
	strcpy(inputfilename, argv[1]);
	// 실행할 번째 알고리즘 받기
	int algorithm_num = atoi(argv[2]);
	// 시간을 재기 위한 시작시간, 종료시간 변수 생성
	clock_t start_time, end_time;
	
	//input파일 읽고 정보 빼오기 및 정보를 담을 배열 공간 만들어 정보 담기
	FILE *test = fopen(inputfilename, "r");
	fscanf(test, "%d", &num);

	array = (int*)malloc(sizeof(int)*num);

	for (int a = 0; a < num; a++) {
		fscanf(test, "%d", &use);
		array[a] = use;
	}
	//입력 받은 숫자에 해당하는 알고리즘 호출 및 알고리즘 구동 시간 재기
	//outputfilename 실행하는 알고리즘 순번 덧붙이기
	switch (algorithm_num) {
	case 1:
		strcat(outputfilename, "1_");
		start_time = clock();
		algorithm1(array, num); 
		end_time = clock();
		break;
	case 2:
		strcat(outputfilename, "2_");
		start_time = clock();
		algorithm2(array, num);
		end_time = clock();
		break;
	case 3: 
		strcat(outputfilename, "3_");
		start_time = clock();
		algorithm3(array, num);
		end_time = clock();
		break;
	case 4: 
		strcat(outputfilename, "4_");
		start_time = clock();
		algorithm4(array, num);
		end_time = clock();
		break;
	default: printf("error\n");
	}

	fclose(test);
	//outputfilename에 inputfilename 덧붙이기
	 strcat(outputfilename, inputfilename);
	//output 파일 생성 및 정보 담기
	test = fopen(outputfilename, "w");

	fprintf(test, "%s\n", inputfilename);
	fprintf(test, "%d\n", algorithm_num);
	fprintf(test, "%d\n", num);
	fprintf(test, "%.6f\n", (double)(end_time - start_time) * 1000 / CLOCKS_PER_SEC);
	for (int a = 0; a < num; a++)
		fprintf(test, "%d ", array[a]);
	fprintf(test, "\n");
	
	fclose(test);
	free(array);
	
	return 0;

}