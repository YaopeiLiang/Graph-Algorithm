#include<iostream>
#include<time.h>
#include<math.h>
#include"Graph.h"
#include"heap.h"
using namespace std;

Graph::Graph(int VexNum)
{
	vexnum = VexNum;
	adj = new GNode*[vexnum];
	int i,j;
	for (i = 0; i < vexnum; ++i)
	{
		adj[i] = new GNode();
	}
	source = rand();
	destination = rand();
	cout << "Դ��:" << source << ",Ŀ��ڵ�:" << destination << endl;


	queryLen = 3;
	query = new int[queryLen];//��ѯ3����
	cout << "Ҫ��ѯ�ĵ�:";
	for (i = 0; i < queryLen; ++i)
	{
		query[i] = rand();
		cout << query[i] << ' ';
	}
	cout << endl;

	int totalPath = (queryLen + 2)*(queryLen + 1);//n���㹲��n*n(-1)����
	pathStack = new stack<int>[totalPath+1];
	currentPath = 1;//��1��ʼ����

	arc = new int*[queryLen + 2];//��ʼ���ڽӾ���
	for (i = 0; i < queryLen + 2; ++i)
	{
		arc[i] = new int[queryLen + 2];
		arc[i][i] = INT_MAX;
	}
	vertexThrough = 0;
	runTime = 1;
}


//β�ӷ��ӱ�
void Graph::addEdge(int from, int to, int w)
{
	GNode* node = new GNode();
	node->vex = to;
	node->weight = w;
	node->next = adj[from]->next;
	adj[from]->next = node;
}




//����BFS�͵Ͻ�˹�����㷨˼��Ľ�������·��
int Graph::BFS(int src, int des)
{
	int capacity = 1971280;
	Heap heap(capacity,src,des);
	bool* visit = new bool[vexnum];//��¼��ǰ�ڵ��Ƿ񱻷��ʹ�
	int* path = new int[vexnum]; //��¼�ڵ��ǰ��

	GNode* p = adj[src]->next;//Դ����ڱ�
	int i = 0;
	for (; i < vexnum; ++i)//��ʼ��
	{
		visit[i] = false;
		path[i] = -1;
	}
		
	//����Դ��ֱ�����ڵĵ����,����Ѿ�����
	for (; p != NULL; p = p->next)
	{
		heap.insert2(p->vex, p->weight);
		visit[p->vex] = true;
	}
	path[src] = src;
	//heap.buildMinHeap();

	HNode node;
	i = 0;
	while (!heap.isEmpty())
	{
		node = heap.extractMin();
		visit[node.id] = true;//��ȡ�����ĵ���Ϊ�Ѿ����ʣ���ֹ�ɻ�

		if (node.id == des)//���ȡ���ĵ���Ŀ��ڵ㣬����ѭ��
			break;

		int dist = node.dist;//��õ�ǰ����

		p = adj[node.id]->next;//Ѱ���ڱ�
		for (; p != NULL; p = p->next)
		{
			if (!visit[p->vex])//��ֹ�ɻ�
			{
				heap.insert2(p->vex, p->weight + dist);//������ǵ�ǰ�ڵ��Ȩ�ؼ�����һ���ߵľ���
				path[p->vex] = node.id; //��¼��ǰ�ڵ��ǰ���ڵ�
			}
				
		}
		//heap.buildMinHeap();
	}
	
	//cout << src << " to " << des << ",dist is:" << node.dist << endl;
	//cout << "path" << endl;
	
	
	i = node.id;//��Ŀ��ڵ���ǰ��ǰ����������·��
	pathStack[currentPath].push(i);//����Ŀ��ڵ�
	while (path[i] != -1)
	{
		pathStack[currentPath].push(path[i]);
		++vertexThrough;
		//cout << path[i] << ' ';
		i = path[i]; //����һ��ǰ��
	}
	pathStack[currentPath].push(src);//����Դ��

	pathStack[currentPath].push(des);//����һ��Ŀ��ڵ㣬�����
	//cout << endl;

	++currentPath;//��ǰ·��+1

	delete[] visit;
	delete[] path;

	return node.dist;//���ؾ���
}



void Graph::initArc()
{
	//Դ��
	int srcToQ0 = BFS(source, query[0]);
	int srcToQ1 = BFS(source, query[1]);
	int srcToQ2 = BFS(source, query[2]);
	int srcToDes = BFS(source, destination);
	arc[0][1] = srcToQ0;
	arc[0][2] = srcToQ1;
	arc[0][3] = srcToQ2;
	arc[0][4] = srcToDes;

	//��0
	int q0ToSrc = BFS(query[0], source);
	int q0ToQ1 = BFS(query[0], query[1]);
	int q0ToQ2 = BFS(query[0], query[2]);
	int q0ToDes = BFS(query[0], destination);
	arc[1][0] = q0ToSrc;
	arc[1][2] = q0ToQ1;
	arc[1][3] = q0ToQ2;
	arc[1][4] = q0ToDes;

	//��1
	int q1ToSrc = BFS(query[1], source);
	int q1ToQ0 = BFS(query[1], query[0]);
	int q1ToQ2 = BFS(query[1], query[2]);
	int q1ToDes = BFS(query[1], destination);
	arc[2][0] = q1ToSrc;
	arc[2][1] = q1ToQ0;
	arc[2][3] = q1ToQ2;
	arc[2][4] = q1ToDes;

	//��2
	int q2ToSrc = BFS(query[2], source);
	int q2ToQ0 = BFS(query[2], query[0]);
	int q2ToQ1 = BFS(query[2], query[1]);
	int q2ToDes = BFS(query[2], destination);
	arc[3][0] = q2ToSrc;
	arc[3][1] = q2ToQ0;
	arc[3][2] = q2ToQ1;
	arc[3][4] = q2ToDes;

	//Ŀ��ڵ�
	int desToSrc = BFS(destination, source);
	int desToQ0 = BFS(destination, query[0]);
	int desToQ1 = BFS(destination, query[1]);
	int desToQ2 = BFS(destination, query[2]);
	arc[4][0] = desToSrc;
	arc[4][1] = desToQ0;
	arc[4][2] = desToQ1;
	arc[4][3] = desToQ2;

	
	
}

//n���ڵ�ı���n�У�2^(n-1)��
void Graph::findPath()
{
	clock_t start, end;
	start = clock();
	//initArc();
	anotherInitArc();
	end = clock();
	cout << "��ʱ:" << end - start << "����" << endl;
	
	int len = queryLen + 2;//����
	int col = pow(2,len-1);//��������2��n-1�η���

	int i, j, k,min,temp,route;//�õ��ı���

	int** dist = new int*[len];  //��¼����
	int** mark = new int*[len]; //��¼·��
	for (i = 0; i < len; ++i)
	{
		dist[i] = new int[col];
		mark[i] = new int[col];
	}
	for (i = 0; i < len; ++i)
	{
		for (j = 0; j < col; ++j)
		{
			dist[i][j] = -1;
			mark[i][j] = -1;
		}
	}

	//��dist�ĵ�0�и�ֵ
	for (i = 0; i < len; ++i)
		dist[i][0] = arc[i][0];

	

	for (i = 1; i < col - 1; ++i)//���һ�в���ѭ�������
	{
		for (j = 1; j < len; ++j)
		{
			if (((int)pow(2, j - 1) & i) == 0)//���j����i��ʾ�ļ�����
			{
				min = INT_MAX;
				for (k = 1; k < len; ++k)
				{
					if ((int)pow(2, k - 1) & i)//�����ʾ���k�ڼ�����  
					{
						temp = arc[j][k] + dist[k][i - (int)pow(2, k - 1)];
						if (temp < min)
						{
							min = temp;
							dist[j][i] = min; //�׶�����ֵ
							mark[j][i] = k;   //�������ž���
						}
					}
				}
			}
		}
	}

	//�������һ��,����������ֵ  
	min = INT_MAX;
	for (k = 1; k < len; ++k)
	{
		//b-1�Ķ�����ȫ1����ʾ����{1,2,3,4,5}������ȥ��k��㼴��k��Ӧ�Ķ�����λ��0  
		temp = arc[0][k] + dist[k][col - 1 - (int)pow(2, k - 1)];
		if (temp < min)
		{
			min = temp;
			dist[0][col - 1] = min;//�����Ž�  
			mark[0][col - 1] = k;
		}
	}

	/*for (i = 0; i < len; ++i)
	{
		for (j = 0; j < len; ++j)
		{
			if (arc[i][j] != INT_MAX)
				cout << arc[i][j] << ' ';
			else
				cout << "INF" << ' ';
		}
		cout << endl;
	}*/
	
	cout << "Դ��:" << source << ",Ŀ��ڵ�:" << destination << endl<<"Ŀ��ڵ�:";
	for (i = 0; i < 3; ++i)
		cout << query[i] << ' ';
	cout << "���·������:" << dist[0][col - 1] << endl;

	cout << "���·��:"<<source;
	int result[5] = { source,query[0],query[1],query[2],destination };

	int pathOrder[6];
	int cur = 0;
	pathOrder[cur++] = 0;//0��ӦԴ��
	for (i = col - 1, j = 0; i>0; )//i�Ķ�������5��1����ʾ����{1,2,3,4,5}  
	{
		j = mark[j][i];//��һ��ȥ���ĸ����  
		i = i - (int)pow(2, j - 1);//��i��ȥ��j���  
		cout << "->" << result[j];
		pathOrder[cur++] = j;//��Ӧ���±�
	}
	cout << "->" << result[0] << endl;
	pathOrder[cur++] = 0;
	

	cout << "�����Ľڵ���:" << vertexThrough << endl;//��������Ľڵ���

	j = 0;
	int count=0;
	int PRE;
	int next;
	int index;
	int valid;//У��
	for (i=0; i<cur-2; ++i)
	{
		PRE = pathOrder[i];
		next = pathOrder[i + 1];
	
		cout << "from " << result[PRE] << " to " << result[next] <<". distance:"<<arc[PRE][next]<< endl;
		index = 4 * PRE;
		if (index == 0)
			index += 1;
		
		if (index == 4)
			index += 1;
	
		valid = pathStack[index].top();
		while (valid != result[next])//��֤Ŀ��ڵ�ı��
		{
			++index;
			if (!pathStack[index].empty())
				valid = pathStack[index].top();
		}
		pathStack[index].pop();

		while (!pathStack[index].empty())
		{
			route = pathStack[index].top();
			pathStack[index].pop();
			cout << route << "->";
			++count;
			if (count % 10 == 0)
				cout << endl;
		}
		cout << endl;
	}
	
	//��������һ���ڵ㵽Դ���·��
	index = next * 4;
	for (; pathStack[index].empty(); ++index)
		;

	valid = pathStack[index].top();
	while (valid != result[0])//��֤Ŀ��ڵ�ı��
	{
		++index;
		if (!pathStack[index].empty())
			valid = pathStack[index].top();
	}
	pathStack[index].pop();

	cout << endl << "back  to Source:" << "form:" << result[pathOrder[i]] << " to " << result[0] << endl;;
	while (!pathStack[index].empty())
	{
		route = pathStack[index].top();
		pathStack[index].pop();
		cout << route << "->";
		++count;
		if (count % 10 == 0)
			cout << endl;
	}
	cout << endl;
	
}


void Graph::anOtherBFS(int src, int* arr,int* result)
{
	int capacity = 1971280;
	Heap heap(capacity, src, arr[0]);
	bool* visit = new bool[vexnum];//��¼��ǰ�ڵ��Ƿ񱻷��ʹ�
	int* path = new int[vexnum]; //��¼�ڵ��ǰ��

	GNode* p = adj[src]->next;//Դ����ڱ�
	int i = 0;
	for (; i < vexnum; ++i)//��ʼ��
	{
		visit[i] = false;
		path[i] = -1;
	}

	//����Դ��ֱ�����ڵĵ����,����Ѿ�����
	for (; p != NULL; p = p->next)
	{
		heap.insert2(p->vex, p->weight);
		visit[p->vex] = true;
	}
	path[src] = src;
	//heap.buildMinHeap();

	int arrCopy[4];
	for (i = 0; i < 4; ++i)
		arrCopy[i] = arr[i];

	HNode node;
	i = 0;
	int count = 0;
	while (!heap.isEmpty())
	{
		node = heap.extractMin();
		visit[node.id] = true;//��ȡ�����ĵ���Ϊ�Ѿ����ʣ���ֹ�ɻ�

		if (count < 4)
		{
			if (node.id == arr[0])
			{
				result[0] = node.dist;
				arr[0] = -1;
				++count;
			}
			else if (node.id == arr[1])
			{
				result[1] = node.dist;
				arr[1] = -1;
				++count;
			}
			else if (node.id == arr[2])
			{
				result[2] = node.dist;
				arr[2] = -1;
				++count;
			}
			else if (node.id == arr[3])
			{
				result[3] = node.dist;
				arr[3] = -1;
				++count;
			}
		}
		else
			break;

		int dist = node.dist;//��õ�ǰ����

		p = adj[node.id]->next;//Ѱ���ڱ�
		for (; p != NULL; p = p->next)
		{
			if (!visit[p->vex])//��ֹ�ɻ�
			{
				heap.insert2(p->vex, p->weight + dist);//������ǵ�ǰ�ڵ��Ȩ�ؼ�����һ���ߵľ���
				path[p->vex] = node.id; //��¼��ǰ�ڵ��ǰ���ڵ�
			}

		}
		//heap.buildMinHeap();
	}
	int j;
	for (j = 0; j < 4; ++j)
	{
		i = arrCopy[j];//��Ŀ��ڵ���ǰ��ǰ����������·��
		

		pathStack[currentPath].push(i);//����Ŀ��ڵ�
		while (path[i] != -1)
		{
			pathStack[currentPath].push(path[i]);
			++vertexThrough;
			i = path[i]; //����һ��ǰ��
		}
		pathStack[currentPath].push(src);//����Դ��

		pathStack[currentPath].push(arrCopy[j]);//����һ��Ŀ��ڵ㣬�����
										

		++currentPath;//��ǰ·��+1
	}

	delete[] path;
	delete[] visit;
}

void Graph::anotherInitArc()
{
	int nums[4];
	int result[4];
	int i = 0; 
	int j;

	nums[0] = query[0];
	nums[1] = query[1];
	nums[2] = query[2];
	nums[3] = destination;
	anOtherBFS(source, nums, result);
	arc[0][1] = result[0];
	arc[0][2] = result[1];
	arc[0][3] = result[2];
	arc[0][4] = result[3];

	//��0
	nums[0] = source;
	nums[1] = query[1];
	nums[2] = query[2];
	nums[3] = destination;
	anOtherBFS(query[0], nums, result);
	arc[1][0] = result[0];
	arc[1][2] = result[1];
	arc[1][3] = result[2];
	arc[1][4] = result[3];

	//��1
	nums[0] = source;
	nums[1] = query[0];
	nums[2] = query[2];
	nums[3] = destination;
	anOtherBFS(query[1], nums, result);
	arc[2][0] = result[0];
	arc[2][1] = result[1];
	arc[2][3] = result[2];
	arc[2][4] = result[3];

	//��2
	nums[0] = source;
	nums[1] = query[0];
	nums[2] = query[1];
	nums[3] = destination;
	anOtherBFS(query[2], nums, result);
	arc[3][0] = result[0];
	arc[3][1] = result[1];
	arc[3][2] = result[2];
	arc[3][4] = result[3];

	//Ŀ��ڵ�
	nums[0] = source;
	nums[1] = query[0];
	nums[2] = query[1];
	nums[3] = query[2];
	anOtherBFS(destination, nums, result);
	arc[4][0] = result[0];
	arc[4][1] = result[1];
	arc[4][2] = result[2];
	arc[4][3] = result[3];
}

int Graph::getSource()
{
	return source;
}

int Graph::getDestination()
{
	return destination;
}

void Graph::setSource(int src)
{
	source = src;
}

void Graph::setDestination(int des)
{
	destination = des;
}

void Graph::setQuery()
{
	++runTime;
	source = source*runTime;
	destination = destination*runTime;
	query[0] = query[0] * runTime;
	query[1] = query[1] * runTime;
	query[2] = query[2] * runTime;
	vertexThrough = 0;
}