#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>

typedef struct{
	int benefit;
	int weight;
}Item;//�ϳ��� item�� ǥ���ϱ� ���� ����ü
typedef struct{
	double Processing_time;
	int Maximun_benefit_value;
}Data_of_alg;//���� �˰����� ���� ����� ��ȯ�ϱ� ���� ����ü
typedef struct{
	int Number_of_items;
	Data_of_alg Data[4];
}Data_number;//�������� �������� �������� �˰����� ���� ���
typedef struct Node{
	Item item;
	int level;
	int tot_weight;
	double bound;
	struct Node * Next;
}Tree_node;

void Print_output(Data_number Data);//����� ���Ϸ� ����ϴ� �Լ�
Item * Make_random(int size);//������ ������ ���� random number�� ����� �Լ�
void Four_algrithm_in_one(int size);//�װ��� �˰����� �Ѱ��� ���� �Լ� -> ũ�⸦ �������ָ� ��� ��� ���� ��������
void Print_output_init(void);//�ʱ�ȭ �ϴ� �Լ�


Data_of_alg Brute_force(Item * items,int size);
Data_of_alg Gready(Item * items, int size);
Data_of_alg Dynamic_programming(Item * items, int size);
Data_of_alg Branch_and_bound(Item * items, int size);

//���и� üũ�ϱ� ���� ��������
int Brute_force_fail=0;
int Gready_fail=0;
int Dynamic_programming_fail=0;
int Branch_and_bound_fail=0;


int main(void) //�����ϴ� main
{
	printf("output.txt ���� ���� ���Դϴ�. ��ø� ��ٷ��ּ���\n");
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

	printf("output.txt ������ �����Ǿ����ϴ�\n");

	return 0;
}

//������ ������ ���� random number�� ����� �Լ�
Item * Make_random(int size)
{
	Item * new_item;
	int i = 0;//�ݺ����� ���� ����

	srand((unsigned)time(NULL));

	new_item = malloc(size*sizeof(Item));//������ ��ŭ �޸� �����Ҵ�
	if (new_item == NULL){
		printf("�޸� �Ҵ��� �����Ͽ����ϴ�\n");
		exit(-1);
	}
	for(i = 0; i < size; i++)
	{
		new_item[i].benefit = rand() % 500 + 1;
		new_item[i].weight = rand() % 100 + 1;
	}


	/*//���� ������ ��� ����
	printf("���� ����\n");
	for (i = 0; i < size; i++)
	printf("%d��° benefit: %d weight: %d\n", i, new_item[i].benefit, new_item[i].weight);
	*/
	return new_item;
}

//��������Ҷ� form�� ����� �Լ�
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
//����� ���Ϸ� ����ϴ� �Լ�
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

//�װ��� �˰����� �Ѱ��� ���� �Լ�
void Four_algrithm_in_one(int size)
{
	Item * items;
	int d = 0, c = 0;
	Data_of_alg  temp_data;
	Data_number datas;
	
	
	items = Make_random(size);//���� �Լ��� �����
	

	//�װ��� �˰��� ������
	if (Brute_force_fail == 0)//���� ���� Ÿ������ ������ ���� ���ٸ�
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
	

	

	//����Ѵ�
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
	int i = 0,j = 0; //�ݺ���
	Data_of_alg result;
	Data_of_alg fail;
	clock_t start=0, end = 0;

	start = clock();

	//���н� -1�� ��ȯ
	fail.Maximun_benefit_value = -1;
	fail.Processing_time = -1;


	//�������� ���� �迭�� �����޸� �Ҵ�
	binary_num = (int*)malloc(sizeof(int)*size);
	if (binary_num == NULL){
		printf("�޸� �Ҵ��� �����Ͽ����ϴ�\n");
		exit(-1);
	}
	//0���� �ʱ�ȭ
	for (i = 0; i < size-1; i++)
	{
		binary_num[i] = 0;
	}

	
	while(1)
	{
		//��ΰ� 1�� ä������ �ݺ����� ��������
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
		while (1)//�������� ���ڸ� 1�� ������Ų��
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
		//�������� compare_benefit�� �����
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

		//20���� ������ ���α׷� ����
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
	int i = 0;//�ݺ���
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


	//insertion sort�� �̿��Ͽ� sort
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
	//greedy �˰���
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
	int W = size * 25; //�ִ� ����
	int i = 0,j=0;//�ݺ���
	int **B;//2���� �迭
	Data_of_alg result;
	Data_of_alg fail;
	clock_t start = 0, end = 0;

	start = clock();

	fail.Maximun_benefit_value = -1;
	fail.Processing_time = -1;


	//2���� �迭 B ����
	B = (int **)malloc(sizeof(int *) * (W+1) );
	B[0] = (int *)malloc(sizeof(int) * 2 * (W+1) );
	if (B == NULL || B[0] == NULL){
		printf("�޸� �Ҵ��� �����Ͽ����ϴ�\n");
		exit(-1);
	}
	for (int i = 1; i<W+1 ; i++)
	{
		B[i] = B[i - 1] + 2 ;
	}

	//ùĭ�� 0���� �ʱ�ȭ
	for (i = 0; i < 2; i++)
		B[0][i] = 0;
	for (i = 1; i < W + 1; i++)
		B[i][0] = 0;
	

	//dynamic programming
	for (i = 1; i < size + 1; i++)//����
	{
		for (j = 1; j < W + 1; j++)//����
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


	//insertion sort�� �̿��Ͽ� sort
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


	//���� ���� first�� �����
	first = (Tree_node *)malloc(sizeof(Tree_node));
	if (first == NULL){
		printf("�޸� �Ҵ��� �����Ͽ����ϴ�\n");
		exit(-1);
	}
	first->item.benefit = 0;
	first->item.weight = 0;
	first->level = 0;
	first->bound = 0;
	first->Next = NULL;
	//boud���ϴ� ��
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
	///////////�������


	root = first;//linked list�� �����
	max_benefit = first->item.benefit;



	while (root != NULL)//linked list �� ������ ���� ����Ѵ�
	{
		//��带 �ϳ� �����
		temp = root;
		root = root->Next;
		if (temp->bound > max_benefit && temp->level < size)//���� promising�ϴٸ� ���ʰ� ������ node�� �����
		{
			///���� ���� node�� �����//���� ���� �� level�� items�� �߰��Ѵ�
			left = (Tree_node *)malloc(sizeof(Tree_node));
			if (left == NULL){
				printf("�޸� �Ҵ��� �����Ͽ����ϴ�\n");
				exit(-1);
			}
			left->Next = NULL;
			left->bound = 0;
			left->level = temp->level + 1;
			left->item.benefit = temp->item.benefit + items[left->level - 1].benefit;
			left->item.weight = temp->item.weight + items[left->level - 1].weight;
			//boud���ϴ� ��
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
			//////////////////////�������
			if (left->item.benefit > max_benefit && left->item.weight <= W)//������ benefit�� max�̸� �ٲپ� ���´�
				max_benefit = left->item.benefit;

			if (left->bound > max_benefit && left->item.weight<W)//promisiing�ϴٸ� ť�� �ִ´�
			{
				temp2 = root;
				do{
					if (temp2 == NULL)//�ƹ��͵� ������ �׳� �ִ´�
					{
						root = left;
						break;
					}
					else if (temp2->bound > left->bound && temp2->Next == NULL)//�ι�° �ڸ��� �ٸ� �ڸ��̸� �ִ´�
					{
						temp2->Next = left;
						break;
					}
					while (1)//�ٸ� �ڸ��� ã��
					{
						if (temp2->Next == NULL)
							break;
						else if (temp2->Next->bound < left->bound)
							break;
						temp2 = temp2->Next;
					}
					//�ִ´�
					temp1 = temp2->Next;
					temp2->Next = left;
					left->Next = temp1;

				} while (0);
			}
			//�������� ������ node�� �����, ������ ���� item�� �߰����� �ʴ� ���̴�
			right = (Tree_node *)malloc(sizeof(Tree_node));
			if (right == NULL){
				printf("�޸� �Ҵ��� �����Ͽ����ϴ�\n");
				exit(-1);
			}
			right->level = temp->level + 1;
			right->bound = 0;
			right->Next = NULL;
			right->item.benefit = temp->item.benefit;
			right->item.weight = temp->item.weight;
			//boud���ϴ� ��
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
			///////////�������

			if (right->bound > max_benefit && right->item.weight < W)//promising �ϴٸ� ť�� �ִ´�
			{
				temp2 = root;
				do{
					if (temp2 == NULL)//�ƹ��͵� ������ �׳� �ִ´�
					{
						root = right;
						break;
					}
					else if (temp2->bound > right->bound && temp2->Next == NULL)//�ι�° �ڸ��� �ٸ� �ڸ��̸� �ִ´�
					{
						temp2->Next = right;
						break;
					}
					while (1)//�ٸ� �ڸ��� ã��
					{
						if (temp2->Next == NULL)
							break;
						else if (temp2->Next->bound < right->bound)
							break;
						temp2 = temp2->Next;
					}
					//�ִ´�
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
