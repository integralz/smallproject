#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//O(N^6)인 알고리즘 구현
int firstalgorithm(int row, int col, int **array){
    int max, sum;
    max = array[0][0];
  
	//행, 열에서 각각 2개의 위치를 뽑아 직사각형을 만들어 
	//각각의 값들을 더하면서 최대값을 찾는다.
    for(int i = 0; i < row; i++)
        for(int j = i; j < row; j++)
            for(int a = 0; a < col; a++)
                for(int b = a; b < col; b++){
                    sum = 0;
                    for(int c = i; c <= j; c++)
                        for(int d = a; d <= b; d++){
                            sum += array[c][d];
                        }
                    if(sum > max)
                        max = sum;
                }
    
    return max;
}
//O(N^4)인 알고리즘 구현
int secondalgorithm(int row, int col, int **array, int *sumarray){
    int max, sum;
    max = array[0][0];
   
	//열에서 2개의 위치를 뽑아 그 사이에 존재하는 숫자들의 합을
	//행별로 만들어 더하면서 최댓값을 찾는다.
    for(int i = 0; i < col; i++){
        for(int clear = 0; clear < row; clear++)
            sumarray[clear] = 0;
        for(int j = i; j < col; j++){
            for(int k = 0; k < row; k++)
                sumarray[k] += array[k][j];
            for(int a = 0; a < row; a++){
                sum = 0;
                for(int b = a; b < row; b++){
                    sum += sumarray[b];
                    if(sum > max)
                        max = sum;
                }
            }
        }
    }
    
    return max;
}
//O(N^3)인 알고리즘 구현
int thirdalgorithm(int row, int col, int **array, int *sumarray){
    int max, sum;
    max = array[0][0];
  
	//열에서 2개의 위치를 뽑아 그 사이에 존재하는 숫자들의 합을
	//행별로 만들어서 더하는데 합이 음수가 되면 0으로 초기화 하면서 만든다.
    for(int i = 0; i < col; i++){
        for(int clear = 0; clear < row; clear++)
            sumarray[clear] = 0;
        for(int j = i; j < col; j++){
            for(int k = 0; k < row; k++)
                sumarray[k] += array[k][j];
            sum = 0;
            for(int a = 0; a < row; a++){
                sum += sumarray[a];
                if(sum > max)
                    max = sum;
                if(sum < 0)
                    sum = 0;
            }
        }
    }
    
    return max;
}

int main(int argc, char *argv[]){

	if (argc != 3)
		printf("usage: %s filename innput_filename number_of_algorithm\n", argv[0]);
	
	//리눅스에서 입력받은 값들을 변수로 저장
	//putputfilename는 추후에 저장
	char inputfilename[200], outputfilename[215]=  "result_";
	strcpy(inputfilename, argv[1]);
	strcat(outputfilename, inputfilename);
	int algorithm_num = atoi(argv[2]);

	//mp1_20141214.c에서 쓸 변수 저장
	clock_t start_time, end_time;
    int row, col, use, **array, max, *sumarray;

	//불러오는 파일 열기
    FILE *test = fopen(inputfilename, "r");
    
	//행, 열 읽기
    fscanf(test, "%d", &row);
    fscanf(test, "%d", &col);
    
	//데이터 값을 저장하기 위한 공간 만들기
    array = (int**)malloc(sizeof(int*)*row);
    for(int i = 0; i < row; i++)
        array[i] = (int*)malloc(sizeof(int)*col);
    
	//알고리즘2,3에서 고정된 사이 열의 합을 행별로 
	//저장하기 위한 공간 만들기
    sumarray = (int*)malloc(sizeof(int)*row);
    
	//데이터 값 저장
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++){
            fscanf(test,"%d", &use);
            array[i][j] = use;
        }

	//입력 받은 알고리즘 번호 호출
	//순수 알고리즘 동작 시간 측정
	switch (algorithm_num) {
	case 1:
		start_time = clock();
		max = firstalgorithm(row, col, array);
		end_time = clock();
		break;
	case 2:
		start_time = clock();
		max = secondalgorithm(row, col, array, sumarray);
		end_time = clock();
		break;
		
	case 3:
		start_time = clock();
		max = thirdalgorithm(row, col, array, sumarray);
		end_time = clock();
		break;
	}

	//input 파일 닫기
	fclose(test);

	//output파일 생성
	test = fopen(outputfilename, "w");
	
	//조건에서 요구하는 값 파일에 기입
	fprintf(test, "%s\n", inputfilename);
	fprintf(test, "%d\n%d\n%d\n%d\n", algorithm_num, row, col, max);
	fprintf(test, "%.3f\n", (double)(end_time - start_time)*1000 / CLOCKS_PER_SEC);

	//output 파일 닫기
	fclose(test);

	//메모리 해제
	free(sumarray);
	for (int i = 0; i < row; i++) {
		free(array[i]);
	}
	free(array);

    return 0;
}
