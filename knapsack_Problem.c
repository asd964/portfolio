#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>

typedef struct{
	int benefit;
	int weight;
}Item;//하나의 item을 표현하기 위한 구조체
typedef struct{
	double Processing_time;
	int Maximun_benefit_value;
}Data_of_alg;//각각 알고리즘의 수행 결과를 반환하기 위한 구조체
typedef struct{
	int Number_of_items;
	Data_of_alg Data[4];
}Data_number;//아이템의 갯수마다 정해지는 알고리즘의 수행 결과
typedef struct Node{
	Item item;
	int level;
	int tot_weight;
	double bound;
	struct Node * Next;
}Tree_node;

void Print_output(Data_number Data);//결과를 파일로 출력하는 함수
Item * Make_random(int size);//정해진 갯수에 따라 random number를 만드는 함수
void Four_algrithm_in_one(int size);//네가지 알고리즘을 한곳에 묶는 함수 -> 크기를 전달해주면 모든 출력 값을 내보낸다
void Print_output_init(void);//초기화 하는 함수


Data_of_alg Brute_force(Item * items,int size);
Data_of_alg Gready(Item * items, int size);
Data_of_alg Dynamic_programming(Item * items, int size);
Data_of_alg Branch_and_bound(Item * items, int size);

//실패를 체크하기 위한 전역변수
int Brute_force_fail=0;
int Gready_fail=0;
int Dynamic_programming_fail=0;
int Branch_and_bound_fail=0;


int main(void) //실행하는 main
{
	printf("output.txt 파일 생성 중입니다. 잠시만 기다려주세요\n");
	Print_output_init();
	Four_algrithm_in_one(10);
	Four_algrithm_in_one(20);
	Four_algrithm_in_one(30);
	Four_algrithm_in_one(40);
	Four_algrithm_in_one(50);
	Four_algrithm_in_one(100);
	Four_algrithm_in_one(500);
	Four_algrithm_in_one(1000);
	Four_algrithm_in_one(5000);
	Four_algrithm_in_one(10000);

	printf("output.txt 파일이 생성되었습니다\n");

	return 0;
}

//정해진 갯수에 따라 random number를 만드는 함수
Item * Make_random(int size)
{
	Item * new_item;
	int i = 0;//반복문을 위한 변수

	srand((unsigned)time(NULL));

	new_item = malloc(size*sizeof(Item));//사이즈 만큼 메모리 동적할당
	if (new_item == NULL){
		printf("메모리 할당이 실패하였습니다\n");
		exit(-1);
	}
	for(i = 0; i < size; i++)
	{
		new_item[i].benefit = rand() % 500 + 1;
		new_item[i].weight = rand() % 100 + 1;
	}


	/*//랜덤 변수의 출력 보기
	printf("랜덤 변수\n");
	for (i = 0; i < size; i++)
	printf("%d번째 benefit: %d weight: %d\n", i, new_item[i].benefit, new_item[i].weight);
	*/
	return new_item;
}

//파일출력할때 form을 만드는 함수
void Print_output_init(void)
{
	FILE * file;
	fopen_s(&file, "output.txt", "wt");
	if (file == NULL)
	{
		printf("file open fail!\n");
		exit(-1);
	}
	fprintf(file, "Number of			processing time in seconds / Maximum benefit value\n");
	fprintf(file, "  Items			Brute force		Greedy		D.P.		B. & B.\n");
	fclose(file);


}
//결과를 파일로 출력하는 함수
void Print_output(Data_number Data)
{
	FILE * file;
	fopen_s(&file,"output.txt", "at");
	if (file == NULL)
	{
		printf("file open fail!\n");
		exit(-1);
	}


	fprintf(file, "   %-5d		", Data.Number_of_items);
	if (Brute_force_fail == 1)
	{
		fprintf(file, "20miniute over		");
	}
	else
	{
		fprintf(file, "%.2f/%d		", Data.Data[0].Processing_time, Data.Data[0].Maximun_benefit_value);
	}
	if (Gready_fail == 1)
	{
		fprintf(file, "20miniute over	");
	}
	else
	{
		fprintf(file, "%.2f/%d	", Data.Data[1].Processing_time, Data.Data[1].Maximun_benefit_value);
	}
	if (Dynamic_programming_fail == 1)
	{
		fprintf(file, "20miniute over	");
	}
	else
	{
		fprintf(file, "%.2f/%d	", Data.Data[2].Processing_time, Data.Data[2].Maximun_benefit_value);
	}
	if (Branch_and_bound_fail == 1)
	{
		fprintf(file, "20miniute over		\n");
	}
	else
	{
		fprintf(file, "%.2f/%d		\n", Data.Data[3].Processing_time, Data.Data[3].Maximun_benefit_value);
	}
	fclose(file);
}

//네가지 알고리즘을 한곳에 묶는 함수
void Four_algrithm_in_one(int size)
{
	Item * items;
	int d = 0, c = 0;
	Data_of_alg  temp_data;
	Data_number datas;
	
	
	items = Make_random(size);//랜덤 함수를 만들고
	

	//네가지 알고리즘에 돌린다
	if (Brute_force_fail == 0)//전에 오버 타임으로 실패한 적이 없다면
	{
		temp_data=Brute_force(items, size);
		datas.Data[0] = temp_data;
	}
	if (Gready_fail == 0)
	{
		temp_data = Gready(items, size);
		datas.Data[1] = temp_data;
	}
	if (Dynamic_programming_fail == 0)
	{
		temp_data=Dynamic_programming(items, size);
		datas.Data[2] = temp_data;
	}
	if (Branch_and_bound_fail == 0)
	{
		temp_data=Branch_and_bound(items, size);
		datas.Data[3] = temp_data;
	}
	

	

	//출력한다
	datas.Number_of_items = size;
	Print_output(datas);

	free(items);



}

Data_of_alg Brute_force(Item * items, int size)
{
	int * binary_num;
	double com = 0;
	int max_benefit = 0;
	int max_weight = size * 25;
	int compare_benefit = 0;
	int compare_weight = 0;
	int i = 0,j = 0; //반복문
	Data_of_alg result;
	Data_of_alg fail;
	clock_t start=0, end = 0;

	start = clock();

	//실패시 -1을 반환
	fail.Maximun_benefit_value = -1;
	fail.Processing_time = -1;


	//이진수를 만들 배열을 동적메모리 할당
	binary_num = (int*)malloc(sizeof(int)*size);
	if (binary_num == NULL){
		printf("메모리 할당이 실패하였습니다\n");
		exit(-1);
	}
	//0으로 초기화
	for (i = 0; i < size-1; i++)
	{
		binary_num[i] = 0;
	}

	
	while(1)
	{
		//모두가 1로 채워지면 반복문을 빠져나감
		if (binary_num[i] == 1)
		{
			if (i == size - 1)
				break;
			else
			{
				i++;
				continue;
			}
		}

		j = size-1;
		while (1)//이진수의 숫자를 1씩 증가시킨다
		{
			if (binary_num[j] == 1)
				binary_num[j--] = 0;
			else{
				binary_num[j] = 1;
				break;
			}
		}

		compare_benefit = 0;
		compare_weight = 0;
		//이진수로 compare_benefit을 만든다
		for (j = 0; j < size; j++)
		{
			if (binary_num[j] == 1)
			{
				compare_benefit += items[j].benefit;
				compare_weight += items[j].weight;
			}
		}
		if (compare_benefit > max_benefit && compare_weight <=max_weight)
			max_benefit = compare_benefit;

		i = 0;

		//20분이 넘으면 프로그램 종료
		end = clock();
		if (end - start > 1200000)
		{
			Brute_force_fail = 1;
			return fail;
		}

	}

	end = clock();
	result.Maximun_benefit_value = max_benefit;	
	result.Processing_time = ((double)(end - start))/CLOCKS_PER_SEC;
	free(binary_num);
	
	return result;
}

Data_of_alg Gready(Item * items, int size)
{
	int i = 0;//반복문
	int num = 0;
	int remain_w = size * 25;
	double max_benefit = 0;
	Item temp;
	Data_of_alg result;
	Data_of_alg fail;
	clock_t start = 0, end = 0;

	start = clock();

	fail.Maximun_benefit_value = -1;
	fail.Processing_time = -1;


	//insertion sort를 이용하여 sort
	for (i = 1; i < size; i++)
	{
		num = i;
		while ((num > 0) && (( (double)items[num - 1].benefit / items[num - 1].weight) < ((double)items[num].benefit / items[num].weight)))
		{
			temp = items[num];
			items[num] = items[num - 1];
			items[num-1] = temp;
			num--;
		}
	}
	//greedy 알고리즘
	i = 0;
	while (i < size)
	{
		if (items[i].weight <= remain_w)
		{
			max_benefit += items[i].benefit;
			remain_w -= items[i].weight;
		}
		else
		{
			max_benefit += (double)items[i].benefit / items[i].weight * remain_w;
			break;
		}
		i++;
		end = clock();
		if (end - start > 1200000)
		{
			Gready_fail = 1;
			return fail;
		}
	}
	end = clock();


	result.Maximun_benefit_value = max_benefit;
	result.Processing_time = ((double)(end - start)) / CLOCKS_PER_SEC;

	return result;
}

Data_of_alg Dynamic_programming(Item * items,int size)
{
	int W = size * 25; //최대 무게
	int i = 0,j=0;//반복문
	int **B;//2차원 배열
	Data_of_alg result;
	Data_of_alg fail;
	clock_t start = 0, end = 0;

	start = clock();

	fail.Maximun_benefit_value = -1;
	fail.Processing_time = -1;


	//2차원 배열 B 선언
	B = (int **)malloc(sizeof(int *) * (W+1) );
	B[0] = (int *)malloc(sizeof(int) * 2 * (W+1) );
	if (B == NULL || B[0] == NULL){
		printf("메모리 할당이 실패하였습니다\n");
		exit(-1);
	}
	for (int i = 1; i<W+1 ; i++)
	{
		B[i] = B[i - 1] + 2 ;
	}

	//첫칸을 0으로 초기화
	for (i = 0; i < 2; i++)
		B[0][i] = 0;
	for (i = 1; i < W + 1; i++)
		B[i][0] = 0;
	

	//dynamic programming
	for (i = 1; i < size + 1; i++)//가로
	{
		for (j = 1; j < W + 1; j++)//세로
		{
			if (items[i - 1].weight <= j)
			{
				if (items[i - 1].benefit + B[j - items[i - 1].weight][(i - 1)%2]>B[j][(i - 1)%2])
					B[j][i%2] = items[i - 1].benefit + B[j - items[i - 1].weight][(i - 1)%2];
				else
					B[j][i % 2] = B[j][(i - 1) % 2];
			}
			else
				B[j][i % 2] = B[j][(i - 1) % 2];
		}
		end = clock();
		if (end - start > 1200000)
		{
			Dynamic_programming_fail = 1;
			return fail;
		}
	}


	end = clock();

	result.Maximun_benefit_value = B[W][size % 2];
	result.Processing_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	
	free(B[0]);
	free(B);

	return result;
}

Data_of_alg Branch_and_bound(Item * items, int size)
{
	Tree_node * root = NULL, *first = NULL;
	Tree_node * left = NULL, *right = NULL;
	Tree_node * temp, *temp1, *temp2;
	int max_benefit = 0;
	int W = size * 25;
	int i = 0, num = 0;
	int tot_weight = 0;
	int tot_benefit = 0;
	Item item_temp;
	Data_of_alg result;
	Data_of_alg fail;
	clock_t start = 0, end = 0;

	start = clock();

	fail.Maximun_benefit_value = -1;
	fail.Processing_time = -1;


	//insertion sort를 이용하여 sort
	for (i = 1; i < size; i++)
	{
		num = i;
		while ((num > 0) && (((double)items[num - 1].benefit / items[num - 1].weight) < ((double)items[num].benefit / items[num].weight)))
		{
			item_temp = items[num];
			items[num] = items[num - 1];
			items[num - 1] = item_temp;
			num--;
		}
	}


	//제일 먼저 first를 만든다
	first = (Tree_node *)malloc(sizeof(Tree_node));
	if (first == NULL){
		printf("메모리 할당이 실패하였습니다\n");
		exit(-1);
	}
	first->item.benefit = 0;
	first->item.weight = 0;
	first->level = 0;
	first->bound = 0;
	first->Next = NULL;
	//boud구하는 식
	for (tot_weight = first->item.weight, tot_benefit = first->item.benefit, i = first->level; tot_weight < W && i < size; i++)
	{
		tot_benefit += items[i].benefit;
		tot_weight += items[i].weight;
	}
	if (i != size)
	{
		tot_weight -= items[--i].weight;
		tot_benefit -= items[i].benefit;
		first->bound = (double)first->item.benefit + tot_benefit + (W - tot_weight)*items[i].benefit / items[i].weight;
	}
	else
	{
		first->bound = (double)first->item.benefit + tot_benefit;
	}
	///////////여기까지


	root = first;//linked list를 만든다
	max_benefit = first->item.benefit;



	while (root != NULL)//linked list 에 있을때 까지 계속한다
	{
		//노드를 하나 지운다
		temp = root;
		root = root->Next;
		if (temp->bound > max_benefit && temp->level < size)//만약 promising하다면 왼쪽과 오른쪽 node를 만든다
		{
			///먼저 왼쪽 node를 만든다//왼쪽 노드는 그 level의 items을 추가한다
			left = (Tree_node *)malloc(sizeof(Tree_node));
			if (left == NULL){
				printf("메모리 할당이 실패하였습니다\n");
				exit(-1);
			}
			left->Next = NULL;
			left->bound = 0;
			left->level = temp->level + 1;
			left->item.benefit = temp->item.benefit + items[left->level - 1].benefit;
			left->item.weight = temp->item.weight + items[left->level - 1].weight;
			//boud구하는 식
			tot_weight = left->item.weight;
			tot_benefit = left->item.benefit;
			i = left->level;
			while (1){
				if (tot_weight > W || i == size)
					break;
				tot_benefit += items[i].benefit;
				tot_weight += items[i].weight;
				i++;
			}
			if (tot_weight > W)
			{
				tot_weight -= items[--i].weight;
				tot_benefit -= items[i].benefit;
				left->bound = (double)tot_benefit + (W - tot_weight)*items[i].benefit / items[i].weight;
			}
			else
			{
				left->bound = (double)tot_benefit;
			}
			//////////////////////여기까지
			if (left->item.benefit > max_benefit && left->item.weight <= W)//왼쪽의 benefit이 max이면 바꾸어 놓는다
				max_benefit = left->item.benefit;

			if (left->bound > max_benefit && left->item.weight<W)//promisiing하다면 큐에 넣는다
			{
				temp2 = root;
				do{
					if (temp2 == NULL)//아무것도 없으면 그냥 넣는다
					{
						root = left;
						break;
					}
					else if (temp2->bound > left->bound && temp2->Next == NULL)//두번째 자리가 바른 자리이면 넣는다
					{
						temp2->Next = left;
						break;
					}
					while (1)//바른 자리를 찾고
					{
						if (temp2->Next == NULL)
							break;
						else if (temp2->Next->bound < left->bound)
							break;
						temp2 = temp2->Next;
					}
					//넣는다
					temp1 = temp2->Next;
					temp2->Next = left;
					left->Next = temp1;

				} while (0);
			}
			//다음으로 오른쪽 node를 만든다, 오른쪽 노드는 item을 추가하지 않는 것이다
			right = (Tree_node *)malloc(sizeof(Tree_node));
			if (right == NULL){
				printf("메모리 할당이 실패하였습니다\n");
				exit(-1);
			}
			right->level = temp->level + 1;
			right->bound = 0;
			right->Next = NULL;
			right->item.benefit = temp->item.benefit;
			right->item.weight = temp->item.weight;
			//boud구하는 식
			tot_weight = right->item.weight;
			tot_benefit = right->item.benefit;
			i = right->level;
			while (1){
				if (tot_weight > W || i == size)
					break;
				tot_benefit += items[i].benefit;
				tot_weight += items[i].weight;
				i++;
			}
			if (tot_weight > W)
			{
				tot_weight -= items[--i].weight;
				tot_benefit -= items[i].benefit;
				right->bound = (double)tot_benefit + (W - tot_weight)*items[i].benefit / items[i].weight;
			}
			else
			{
				right->bound = (double)tot_benefit;
			}
			///////////여기까지

			if (right->bound > max_benefit && right->item.weight < W)//promising 하다면 큐에 넣는다
			{
				temp2 = root;
				do{
					if (temp2 == NULL)//아무것도 없으면 그냥 넣는다
					{
						root = right;
						break;
					}
					else if (temp2->bound > right->bound && temp2->Next == NULL)//두번째 자리가 바른 자리이면 넣는다
					{
						temp2->Next = right;
						break;
					}
					while (1)//바른 자리를 찾고
					{
						if (temp2->Next == NULL)
							break;
						else if (temp2->Next->bound < right->bound)
							break;
						temp2 = temp2->Next;
					}
					//넣는다
					temp1 = temp2->Next;
					temp2->Next = right;
					right->Next = temp1;

				} while (0);
			}
		}
		free(temp);

		end = clock();
		if (end - start > 1200000)
		{
			Branch_and_bound_fail = 1;
			return fail;
		}
	}

	end = clock();

	result.Maximun_benefit_value = max_benefit;
	result.Processing_time = ((double)(end - start)) / CLOCKS_PER_SEC;

	return result;
}
