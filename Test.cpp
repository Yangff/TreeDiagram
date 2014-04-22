/*
	Project: Tree Diagram
	Author : Yangff
	Date   : 2014-4-21
*/

#include "Common.h"
#include "Graph.h"
#include "Tree.h"
#include "Generator.h"
#include <iostream>

using namespace TreeDiagram;

void f(int x){
	printf("x");
}

struct MyInt : Int{
	MyInt(int x = 0):Int(x % 500){};
	Ints(MyInt);
};

int main(){
	Graph<Nothing, MyInt> G;
/*	G = RandomGraphGenerator< Graph<Nothing, int> >(10000, 200000).Generate();
	G.EachEdge([](int u, int v){
		printf("%d %d\n", u, v);
	});*/

	UndirectedTree<Nothing, MyInt> T = RandomTreeGenerator<UndirectedTree<Nothing, MyInt > >(10).Generate();
	//T.EachEdge([](int u, int v, MyInt k){
	//	printf("%d %d %d\n", u, v, k.x);
	//});
	G = MSTGraphGeneratorByTree<Graph<Nothing, MyInt>, UndirectedTree<Nothing, MyInt > >(T, 30).Generate();
	G.EachEdge([](int u, int v, MyInt k){
		printf("%d %d\n", u, v, k);
	});
/*	T.Traversal(1, [&](int cur, int deep){
		printf("%d %d\n", cur, deep);
	});
*/

	std::cout << GetLastError() << std::endl;
} // function
