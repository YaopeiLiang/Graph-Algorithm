#pragma once
#ifndef GRAPH_H
#define GRAPH_H
#include<stack>
using namespace std;

class GNode
{
public:
	int vex;
	int weight;
	GNode* next;
	GNode()
	{
		vex = 0;
		next = NULL;
	}
};

class Graph
{
private:
	GNode** adj;
	int vexnum;

	int source;  //Դ��
	int destination; //Ŀ��ڵ�

	int* query;//Ҫ�ҵļ���
	int queryLen;//��3������
	int** arc;//�ڽӾ���

	stack<int>* pathStack; //��¼·��
	int currentPath;

	int vertexThrough;//�������ܽڵ���

	int BFS(int src, int des);//�Ҵ�src��des�����·�������ؾ���
	void initArc(); //��ʼ���ڽӾ���

	void anOtherBFS(int src, int* arr,int* result);
	void anotherInitArc();
	int runTime;
public:
	Graph(int v);
	void addEdge(int u, int v, int w);//�ڽӱ�ӱ�
	

	
	void findPath();
	void setSource(int src);
	void setDestination(int des);
	int getSource();
	int getDestination();

	void setQuery();
};

#endif
