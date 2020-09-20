#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//O(N^6)�� �˰��� ����
int firstalgorithm(int row, int col, int **array){
    int max, sum;
    max = array[0][0];
  
	//��, ������ ���� 2���� ��ġ�� �̾� ���簢���� ����� 
	//������ ������ ���ϸ鼭 �ִ밪�� ã�´�.
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
//O(N^4)�� �˰��� ����
int secondalgorithm(int row, int col, int **array, int *sumarray){
    int max, sum;
    max = array[0][0];
   
	//������ 2���� ��ġ�� �̾� �� ���̿� �����ϴ� ���ڵ��� ����
	//�ະ�� ����� ���ϸ鼭 �ִ��� ã�´�.
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
//O(N^3)�� �˰��� ����
int thirdalgorithm(int row, int col, int **array, int *sumarray){
    int max, sum;
    max = array[0][0];
  
	//������ 2���� ��ġ�� �̾� �� ���̿� �����ϴ� ���ڵ��� ����
	//�ະ�� ���� ���ϴµ� ���� ������ �Ǹ� 0���� �ʱ�ȭ �ϸ鼭 �����.
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
	
	//���������� �Է¹��� ������ ������ ����
	//putputfilename�� ���Ŀ� ����
	char inputfilename[200], outputfilename[215]=  "result_";
	strcpy(inputfilename, argv[1]);
	strcat(outputfilename, inputfilename);
	int algorithm_num = atoi(argv[2]);

	//mp1_20141214.c���� �� ���� ����
	clock_t start_time, end_time;
    int row, col, use, **array, max, *sumarray;

	//�ҷ����� ���� ����
    FILE *test = fopen(inputfilename, "r");
    
	//��, �� �б�
    fscanf(test, "%d", &row);
    fscanf(test, "%d", &col);
    
	//������ ���� �����ϱ� ���� ���� �����
    array = (int**)malloc(sizeof(int*)*row);
    for(int i = 0; i < row; i++)
        array[i] = (int*)malloc(sizeof(int)*col);
    
	//�˰���2,3���� ������ ���� ���� ���� �ະ�� 
	//�����ϱ� ���� ���� �����
    sumarray = (int*)malloc(sizeof(int)*row);
    
	//������ �� ����
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++){
            fscanf(test,"%d", &use);
            array[i][j] = use;
        }

	//�Է� ���� �˰��� ��ȣ ȣ��
	//���� �˰��� ���� �ð� ����
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

	//input ���� �ݱ�
	fclose(test);

	//output���� ����
	test = fopen(outputfilename, "w");
	
	//���ǿ��� �䱸�ϴ� �� ���Ͽ� ����
	fprintf(test, "%s\n", inputfilename);
	fprintf(test, "%d\n%d\n%d\n%d\n", algorithm_num, row, col, max);
	fprintf(test, "%.3f\n", (double)(end_time - start_time)*1000 / CLOCKS_PER_SEC);

	//output ���� �ݱ�
	fclose(test);

	//�޸� ����
	free(sumarray);
	for (int i = 0; i < row; i++) {
		free(array[i]);
	}
	free(array);

    return 0;
}
