#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//트리를 만들기 위한 구조체 생성
//symbol에서는 아스키의 값을, freq에서는 쓰인 횟수, left, right는 각각 자식 노드의 주소값을 받음
typedef struct node {
	char symbol;
	int freq;
	struct node *left;
	struct node *right;
}node;
//quick sort 함수 구현 시작
//partition 함수는 주어진 수 중에서 마지막에 위치한 수와 나머지 수들을 비교하여 작은 수들은 앞에 
//큰 수들은 뒤에 위치시키고 마지막에 있던 수가 몇번째로 작은지에 대한 정보 호출
int partition(int low, int high, int **array) {
	int last_num = array[1][high];
	int pivot = low;
	int change;

	for (int a = low; a < high; a++) {
		if (array[1][a] <= last_num) {
			change = array[0][pivot];
			array[0][pivot] = array[0][a];
			array[0][a] = change;
			change = array[1][pivot];
			array[1][pivot++] = array[1][a];
			array[1][a] = change;
		}
	}
	change = array[1][pivot];
	array[1][pivot] = array[1][high];
	array[1][high] = change;
	change = array[0][pivot];
	array[0][pivot] = array[0][high];
	array[0][high] = change;

	return pivot;
}
//partiton 함수에서 만들어진 정보를 바탕으로 작은수들의 모임, 큰 수들의 모임 계속해서 재귀함수로 분할
void divide(int low, int high, int **array) {
	int divide_num;

	if (low >= high);
	else {
		divide_num = partition(low, high, array);
		divide(low, divide_num - 1, array);
		divide(divide_num + 1, high, array);
	}
}
//quick sort함수 호출
void quicksort(int **array, int num) {
	divide(0, num - 1, array);

}

//최소힙을 구현할 때 새로운 값이 들어왔을때 처리하는 함수
//마지막 노드에 삽입하고 부모노드와 비교하여 작으면 바꾸고, 작지 않으면 그대로 두는 형태로
//최대 root까지 올라간다.
void insertheap(node **letternode,int num) {
	int nodenum = num;
	node *change;
	if (nodenum != 0) {
		while (letternode[nodenum]->freq < letternode[(nodenum + 1) / 2 - 1]->freq) {
			change = letternode[nodenum];
			letternode[nodenum] = letternode[(nodenum + 1) / 2 - 1];
			letternode[(nodenum + 1) / 2 - 1] = change;

			nodenum = (nodenum + 1) / 2 - 1;
			if (nodenum == 0)
				break;
		}		
	}
}
//최소 힙을 구현할 때 제일 작은 값인 root를 지우는 함수
//root 노드를 제일 끝 노드로 초기화한 후 최소힙이 되도록 자식 노드들과 비교하여 내린다.
void deleteheap(node **letternode, int num) {
	int nodenum = 0, changenum;
	node *change;
	letternode[0] = letternode[num - 1];
	while ((nodenum + 1) * 2  <= num) {
		if ((nodenum + 1) * 2 <= num - 1) {
			if (letternode[(nodenum + 1) * 2]->freq < letternode[(nodenum + 1) * 2 - 1]->freq){
				changenum = (nodenum + 1) * 2;
			}
			else {
				changenum = (nodenum + 1) * 2 - 1;
			}
		}
		else {
			changenum = (nodenum + 1) * 2 - 1;
		}
		if (letternode[changenum]->freq < letternode[nodenum]->freq) {
			change = letternode[nodenum];
			letternode[nodenum] = letternode[changenum];
			letternode[changenum] = change;
			nodenum = changenum;
		}
		else break;
	}
}
// 노드를 만드는 함수
node* makenode() {
	node *newnode = (node*)malloc(sizeof(node));

	newnode->left = NULL;
	newnode->right = NULL;

	return newnode;
}
// 허프만 알고리즘을 이용하여 트리를 구현하는 함수
// 제일 작은 두 값을 묶어 이 둘의 부모노드를 생성하는 과정을 반복하여 트리를 만들고
// root에 존재하는 노드의 주소값을 반환한다.
node* makehuffman(node ** letternode, int existnum) {
	node *newnode;
	if (existnum == 1) {
		newnode = makenode();
		newnode->symbol = 0;
		newnode->freq = letternode[0]->freq;
		newnode->right = letternode[0];
	}
	else {
		for (int i = 0; i < existnum - 1; i++) {
			newnode = makenode();
			newnode->symbol = -1;
			newnode->freq = letternode[0]->freq;
			newnode->left = letternode[0];
			deleteheap(letternode, existnum - i);
			newnode->freq += letternode[0]->freq;
			newnode->right = letternode[0];
			deleteheap(letternode, existnum - i - 1);
			letternode[existnum - 2 - i] = newnode;
			insertheap(letternode, existnum - 2 - i);
		}
	}
	return newnode;
}

//인코딩하는 함수
//주어진 트리를 확인하여 모든 끝지점의 노드들에 도착한후의 symbol과 자취를 code에 저장한다.
void encode(node *location, int **code, int length, int *footprint) {
	int ascii;
	if (location->left == NULL && location->right == NULL) {
		ascii = location->symbol;
		for (int i = 0; i < length; i++)
			code[ascii][i] = footprint[i];
		code[ascii][length] = 2;
		return;
	}
		if (location->left != NULL) {
		footprint[length] = 0;
		encode(location->left, code, length + 1, footprint);
	}
	if (location->right != NULL) {
		footprint[length] = 1;
		encode(location->right, code, length + 1, footprint);
	}
	 
}
//decode하는 함수
//받은 일렬로 된 코드들을 분석하여 받은 트리에 대입하여 계속해서 진행하다가
// 끝지점에 도달하면 출력하고 이어서 root로 돌아가 이어지는 코드들을 분석한다.
void decode(int *decodecode, node *root, int start, int end, FILE *output) {
	node* now = root;
	for (int i = start; i <= end; i++) {
		if (decodecode[i] == 0) {
			if (now->left != NULL) {
				now = now->left;
			}
			else {
				fprintf(output, "%c", now -> symbol);
				now = root->left;
			}
		}
		else if (decodecode[i] == 1) {
			if (now->right != NULL) {
				now = now->right;
			}
			else {
				fprintf(output, "%c", now -> symbol);
				now = root->right;
			}
		}
	}
	fprintf(output, "%c", now -> symbol);
}
int main(int argc, char *argv[]) {
	//input과 output 파일을 저장
	FILE *input, *output;
	// 파일에서 받은 char형을 저장하는 변수
	char letter;
	// asciinum은 letter의 값을 int형으로 변환하는데에 사용, change는 어떠한 수를 바꿀데에 사용
	// check는 조건문을 진행할 때 조건을 체크하기 위한 매개체로 활용
	int asciinum, change, check = 0;
	//lettercount는 파일에서 letter가 쓰인 횟수를 저장
	//code는 허프만 알고리즘을 진행했을때 나오는 이진 코드를 저장
	//footprint는 code에 저장하기 위해 트리에서 어디를 지나왔는지 자취를 저장
	//decodecode는 디코딩을 하기 위한 일련의 이진 코드들을 저장하는 함수
	int **lettercount, **code, *footprint, *decodecode;
	//existnum은 파일에 쓰인 letter의 종류의 갯수
	//bitnum은 인코딩을 하였을 때 할당 되어야하는 bit의 갯수를 나타냄
	//bytenum은 인코딩을 하였을 때 할당 되어야하는 byte의 갯수를 나타냄
	//데이터는 byte에 저장되는 값을 나타냄
	int existnum = 0, bitnum, bytenum, data;
	//letternode는 초기에 파일에 쓰인 문자들의 정보를 지닌 노드들을 저장하였다가
	// 호프만 알고리즘을 진행할때 트리를 만드는데에 쓰임
	//root는 트리의 root 주소값을 나타냄
	node **letternode, *root;
	
	//만약 리눅스에서 받은 값이 3개가 아닐경우 에러메세지 호출
	if (argc != 3)
		printf("usage: %s filename    -c or -d   coding or decoding file\n", argv[0]);
	
	//inputfilename에서는 받은 파일의 이름을 저장
	//outputfilename에서는 생성할 파일의 이름을 저장
	//how에서는 인코딩 하는지 디코딩하는지에 대한 방식을 저장
	char inputfilename[200], outputfilename[215], how[22];
	strcpy(inputfilename, argv[2]);
	strcpy(how, argv[1]);

	//동적할당을 하여 배열을 만듬
	code = (int**)malloc(sizeof(int*) * 128);
	for (int i = 0; i < 128; i++)
		code[i] = (int*)malloc(sizeof(int) * 1000);
	for (int i = 0; i < 128; i++)
		code[i][0] = 2;
	footprint = (int*)malloc(sizeof(int) * 1000);
	lettercount = (int**)malloc(sizeof(int) * 2);
	for (int i = 0; i < 2; i++)
		lettercount[i] = (int*)malloc(sizeof(int) * 128);
	
	//파일을 코딩할때의 알고리즘 구현
	if (how[0] == '-' && how[1] == 'c' && how[2] == '\0') {
		//output파일 이름 생성
		strcpy(outputfilename, argv[2]);
		strcat(outputfilename, ".zz");

		//input파일을 받음 만약 없는 파일을 받을시 에러메세지 생성
		input = fopen(inputfilename, "r");
		if(input == NULL){
			printf("there is a no file\n");
			return 0;
		}
		//output파일 생성
		output = fopen(outputfilename, "w");

		//lettercount 배열 초기화 첫번째 행에는 아스키 코드 번호를, 두번째 행에는 0을 넣음
		for (int i = 0; i < 128; i++) {
			lettercount[0][i] = i;
			lettercount[1][i] = 0;
		}
		//파일에 있는 글들을 하나씩 읽어 lettercount에 몇개씩 쓰였는지 저장
		while (fscanf(input, "%c", &letter) != EOF) {
			asciinum = letter;
			lettercount[1][asciinum]++;
		}
		//lettercount를 확인하여 몇종류의 글자가 쓰였는지 확인
		for (int i = 0; i < 128; i++) {
			if (lettercount[1][i] != 0)
				existnum++;
		}
		//output 파일에 몇종류의 글자를 가졌는지 저장
		fprintf(output, "%d ", existnum);
		existnum = 0;
		//output 파일에 글자의 종류 및 쓰인 횟수를 각각 저장하고,
		//쓰인 적인 있는 글자들을 lettercount배열 내에서 앞으로 떙긴다.
		for (int i = 0; i < 128; i++) {
			if (lettercount[1][i] != 0) {
				fprintf(output, "%c ", i);
				fprintf(output, "%d ", lettercount[1][i]);
				lettercount[1][existnum] = lettercount[1][i];
				lettercount[0][existnum] = lettercount[0][i];
				existnum++;
			}
		}
		//lettercount 배열내에서 쓰인 글자들만 판단하여, 횟수에 대한 퀵소드 진행
		quicksort(lettercount, existnum);
		
		//letternode에 각각의 노드를 생성하여 정보를 전달한다
		letternode = (node**)malloc(sizeof(node*)*existnum);
		for (int i = 0; i < existnum; i++) {
			letternode[i] = makenode();
			letternode[i]->symbol = lettercount[0][i];
			letternode[i]->freq = lettercount[1][i];
		}

		//허프만 알고리즘을 진행 후 트리의 root의 정보를 받는다.
		root = makehuffman(letternode, existnum);
		
		//인코딩하여 쓰인 글자 각각의 이진코들를 얻는다.
		encode(root, code, 0, footprint);
		
		fclose(input);
		bytenum = 0;
		bitnum = 0;
		data = 0;
	
		input = fopen(inputfilename, "r");
		//input 파일의 글자들을 인코딩을 하여 나온 이진코드로 읽었을 때 
		//bit의 갯수와 byte의 갯수 확인
		//만약 bit의 갯수가 7이되면 byte의 갯수를 증가시키고, bit의 값은 0이된다.
		//(엄밀히 말하면 bit의 갯수가 8개여야지 byte이지만 아스키 값이 0-127까지 이므로
		//2^7까지 정보를 받게 이렇게 설정하였다)
		while (fscanf(input, "%c", &letter) != EOF) {
			asciinum = letter;
			while (code[asciinum][check] != 2) {
				bitnum++;
				if (bitnum == 7) {
					bitnum = 0;
					bytenum++;
				}
				check++;
			}
			check = 0;
		}
		bytenum++;
		//bytenum과 bitnum을 output 파일에 기록
		fprintf(output, " ");
		fprintf(output, "%d ", bytenum);
		fprintf(output, "%d ", bitnum);
		fclose(input);
		bitnum = 0;
		input = fopen(inputfilename, "r");
		
		//이진코드를 7개씩 묵어 아스키코드로 만들어 output 파일에 char형으로 저장한다.
		while (fscanf(input, "%c", &letter) != EOF) {
			asciinum = letter;
			while (code[asciinum][check] != 2) {
				data *= 2;
				data += code[asciinum][check];
				bitnum++;
				if (bitnum == 7) {
					fprintf(output, "%c", data);
					bitnum = 0;
					data = 0;
				}
				check++;
			}
			check = 0;
		}
		if (bitnum != 0) {
			fprintf(output, "%c", data);
		}
		fclose(input);
		fclose(output);
		//동적할당 해제
		free(letternode);
	}
	//주어진 input파일을 디코딩을 하는 알고리즘 구현
	else if (how[0] == '-' && how[1] == 'd' && how[2] == '\0') {
		//output파일 이름 생성
		strcpy(outputfilename, argv[2]);
		strcat(outputfilename, ".yy");
		
		//input파일을 연다. 만약 파일이 존재하지 않으면 에러 메세지 출력
		input = fopen(inputfilename, "r");
		if(input == NULL){
			printf("there is a no file\n");
			return 0;
		}
		
		//쓰인 letter의 종류및 횟수를 알기위해 배열 생성
		for (int a = 0; a < 128; a++) {
			lettercount[0][a] = a;
			lettercount[1][a] = 0;
		}
		//output파일 생성
		output = fopen(outputfilename, "w");
		//input파일을 처음 읽어 쓰인 글자의 종류의 갯수를 확인
		fscanf(input, "%d", &asciinum);
		change = asciinum;
	
		//종류의 갯수만큼 input파일에서 정보를 읽어 무슨 글자가 몇번 쓰였는지를
		//lettercount에 저장
		for (int i = 0; i < change; i++) {
			fscanf(input, "%c", &letter);
			fscanf(input, "%c", &letter);
			fscanf(input, "%d", &asciinum);
			lettercount[1][(int)letter] = asciinum;
		}
		
		//lettercount에서 내에서 쓰인 글자들은 앞으로 땡긴다
		existnum = 0;
		for (int i = 0; i < 128; i++) {
			if (lettercount[1][i] != 0) {
				lettercount[1][existnum] = lettercount[1][i];
				lettercount[0][existnum] = lettercount[0][i];
				existnum++;
			}
		}
		//쓰인 글자들의 횟수에 대해 퀵소트로 정렬
		quicksort(lettercount, existnum);
		
		//받은 정보들을 바탕으로 node를 생성
		letternode = (node**)malloc(sizeof(node*)*existnum);
		for (int i = 0; i < existnum; i++) {
			letternode[i] = makenode();
			letternode[i]->symbol = lettercount[0][i];
			letternode[i]->freq = lettercount[1][i];
		}
		//허프만 알고리즘 진행하여 트리를 생성
		root = makehuffman(letternode, existnum);
		
		//input 파일에 저장된 bytenum과 bitnum 호출
		fscanf(input, "%d", &bytenum);
		fscanf(input, "%d", &bitnum);
		fscanf(input, "%c", &letter);
		
		//bytenum과 bitnum을 바탕으로 decodecode배열의 갯수를 확정하여 동적할당
		decodecode = (int*)malloc(sizeof(int)*((bytenum - 1) * 7 + bitnum));
		//이후의 input파일의 정보를 읽어 각각을 이진수 코드로 변이 후 
		//decodecode에 저장
		for (int i = 0; i < bytenum; i++) {
			fscanf(input, "%c", &letter);
			asciinum = letter;
			if (i == bytenum - 1) {
				for (int a = 0; a < bitnum; a++) {
					decodecode[7 * i + bitnum - a - 1] = asciinum % 2;
					asciinum /= 2;
				}
			}
			else
				for (int a = 0; a < 7; a++) {
					decodecode[7 * (i + 1) - a - 1] = asciinum % 2;
					asciinum /= 2;
				}
		}
		//디코딩을 진행
		decode(decodecode, root, 0, (bytenum - 1) * 7 + bitnum - 1, output);
		//동적할당 해제
		free(letternode);
		free(decodecode);
	}
	
	//만약 -c 혹은 -d 이외에 다른 값이 들어가면 에러 메세지를 생성한다.
	else printf("error\n");
	
	//동적할당 해제
	for(int i = 0; i <128; i++)
		free(code[i]);
	free(code);
	free(footprint);
	for(int i = 0; i < 2; i++)
		free(lettercount[i]);
	free(lettercount);
	
	
	return 0;
}