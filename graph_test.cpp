
// Testing functions for graph class

#include "graph.h"
#include "graph_utils.h"
#include "graph_order.h"

#include <vector>
#include <string>

using namespace std;
using namespace drm;
using namespace drm::GraphUtils;

#define CHECK(x) checkCondition(x, #x)

Graph* g1;
Graph* g2;

bool checkCondition(bool testCondition, const char* testStr)
{
	if (!testCondition)
		printf("FAILED ****\n  %s did not succeed.\n", testStr);
	return testCondition;
}

// Make a new (empty) undirected graph and add some edges and vertices.
void test1()
{
	printf("Create a simple undirected graph g1... \t\t");
	g1 = new Graph(SimpleUndirected);

	g1->addEdge(1, 2);
	g1->addEdge(1, 3);
	g1->addEdge(1, 2);
	g1->addEdge(2, 3);
	g1->addEdge(1, 4);
	g1->addVertex(7);

	if (CHECK(g1->order() == 5) &&
		CHECK(g1->size() == 4) &&
		CHECK(density(*g1) == 4.0 / 10) &&
		CHECK((g1->neighbors(1) == vector<int>{ 2, 3, 4 })) &&
		CHECK((g1->neighbors(2) == vector<int>{ 1, 3 })) &&
		CHECK((g1->neighbors(3) == vector<int>{ 1, 2 })) &&
		CHECK((g1->neighbors(4) == vector<int>{ 1 })) &&
		CHECK((g1->neighbors(7) == vector<int>())) &&
		CHECK((g1->degree(1) == 3)) &&
		CHECK((g1->indegree(1) == 3)) &&
		CHECK((g1->outdegree(1) == 3)))
		printf("PASSED\n");
}

void test1a()
{
	printf("Creating complement graph of g1... \t\t");
	Graph gc1 = complement(*g1);
	if (CHECK((gc1.neighbors(1) == vector<int>{ 7 })) &&
		CHECK((gc1.neighbors(2) == vector<int>{ 4, 7 })) &&
		CHECK((gc1.neighbors(3) == vector<int>{ 4, 7 })) &&
		CHECK((gc1.neighbors(4) == vector<int>{ 2, 3, 7 })) &&
		CHECK((gc1.neighbors(7) == vector<int>{ 1, 2, 3, 4 })))
		printf("PASSED\n");
}

void test2()
{
	printf("Create a simple directed graph g2... \t\t");
	g2 = new Graph(SimpleDirected);

	g2->addEdge(1, 2);
	g2->addEdge(1, 3);
	g2->addEdge(1, 2);
	g2->addEdge(2, 3);
	g2->addEdge(3, 2);
	g2->addEdge(1, 4);
	g2->addVertex(7);
	g2->addEdge(7, 4);

	if (CHECK(g2->order() == 5) &&
		CHECK(g2->size() == 6) &&
		CHECK(density(*g2) == 6.0 / 20) &&
		CHECK((g2->neighbors(1) == vector<int>{ 2, 3, 4 })) &&
		CHECK((g2->neighbors(2) == vector<int>{ 3 })) &&
		CHECK((g2->neighbors(3) == vector<int>{ 2 })) &&
		CHECK((g2->neighbors(4) == vector<int>())) &&
		CHECK((g2->neighbors(7) == vector<int>{ 4 })) &&
		CHECK((g2->degree(3) == 1)) &&
		CHECK((g2->indegree(4) == 2)) &&
		CHECK((g2->outdegree(4) == 0)))
		printf("PASSED\n");
}

void test2a()
{	
	printf("Creating complement graph of g2... \t\t");
	Graph gc2 = complement(*g2);

	if (CHECK((gc2.neighbors(1) == vector<int>{ 7 })) &&
		CHECK((gc2.neighbors(2) == vector<int>{ 1, 4, 7 })) &&
		CHECK((gc2.neighbors(3) == vector<int>{ 1, 4, 7 })) &&
		CHECK((gc2.neighbors(4) == vector<int>{ 1, 2, 3, 7 })) &&
		CHECK((gc2.neighbors(7) == vector<int>{ 1, 2, 3 })))
		printf("PASSED\n");
}

void test3()
{
	printf("Check for an independent set in g1... \t\t");
	vector<int> set1{7, 4, 2};
	vector<int> set2{7, 3};
	vector<int> set3{1, 4, 7};

	if (CHECK((testIndSet(*g1, set1))) &&
		CHECK((testIndSet(*g1, set2))) &&
		CHECK((!testIndSet(*g1, set3))))
		printf("PASSED\n");
}

void test4()
{
	printf("Check for a clique in g1... \t\t\t");
	vector<int> set1{1, 4, 3};
	vector<int> set2{1, 2, 3};

	if (CHECK((!testClique(*g1, set1))) &&
		CHECK((testClique(*g1, set2))))
		printf("PASSED\n");
}

void test4a()
{
	printf("Check vertex orderings for g1 complement... \t");

	Graph gc1 = complement(*g1);

	if (CHECK((identityVertexOrder(gc1) == vector<int>{1, 2, 3, 4, 7})) &&
		CHECK((degreeListVertexOrder(gc1) == vector<int>{7, 4, 2, 3, 1})) &&
		CHECK((degreeSequence(gc1) == vector<int>{4, 3, 2, 2, 1})))
		printf("PASSED\n");
}

void test4b()
{
	printf("Check vertex orderings for g2... \t\t");

	if (CHECK((identityVertexOrder(*g2) == vector<int>{1, 2, 3, 4, 7})) &&
		CHECK((outdegreeListVertexOrder(*g2) == vector<int>{1, 2, 3, 7, 4})) &&
		CHECK((indegreeListVertexOrder(*g2) == vector<int>{2, 3, 4, 1, 7})) &&
		CHECK((outdegreeSequence(*g2) == vector<int>{3, 1, 1, 1, 0})) &&
		CHECK((indegreeSequence(*g2) == vector<int>{2, 2, 2, 0, 0})))
		printf("PASSED\n");
}

void test5()
{
	printf("Delete a vertex and an edge from g1... \t\t");
	g1->delEdge(1, 2);
	g1->delEdge(1, 2);
	g1->delVertex(4);
	g1->delVertex(8);

	if (CHECK(g1->order() == 4) &&
		CHECK(g1->size() == 2) &&
		CHECK(density(*g1) == 2.0 / 6) &&
		CHECK((g1->neighbors(1) == vector<int>{ 3 })) &&
		CHECK((g1->neighbors(2) == vector<int>{ 3 })) &&
		CHECK((g1->neighbors(3) == vector<int>{ 1, 2 })) &&
		CHECK((g1->neighbors(4) == vector<int>())) &&
		CHECK((g1->neighbors(7) == vector<int>())))
		printf("PASSED\n");
}

void test6()
{
	printf("Delete a vertex and an edge from g2... \t\t");
	g2->delEdge(3, 2);
	g2->delEdge(3, 2);
	g2->delVertex(4);

	if (CHECK(g2->order() == 4) &&
		CHECK(g2->size() == 3) &&
		CHECK(density(*g2) == 3.0 / 12) &&
		CHECK((g2->neighbors(1) == vector<int>{ 2, 3 })) &&
		CHECK((g2->neighbors(2) == vector<int>{ 3 })) &&
		CHECK((g2->neighbors(3) == vector<int>())) &&
		CHECK((g2->neighbors(4) == vector<int>())) &&
		CHECK((g2->neighbors(7) == vector<int>())))
		printf("PASSED\n");
}

void test7()
{
	printf("Testing copy constructor, operator=... \t\t");
	Graph gCopy(*g1);
	gCopy.addEdge(1, 2);

	Graph gAssign = *g1;
	gAssign.addEdge(7, 3);

	if (CHECK((gCopy.hasEdge(1, 2))) &&
		CHECK((gCopy.hasEdge(2, 3))) &&
		CHECK((gCopy.hasEdge(1, 3))) &&
		CHECK((!g1->hasEdge(1, 2))) &&
		CHECK((gAssign.hasEdge(7, 3))) &&
		CHECK((gAssign.hasEdge(3, 2))) &&
		CHECK((gAssign.hasEdge(3, 1))) &&
		CHECK((!g1->hasEdge(7, 3))))
		printf("PASSED\n");
}


void end()
{
	// Succeeds if both graphs are successfully deleted
	printf("Deleting graphs g1 and g2... \t\t\t");
	delete g1;
	delete g2;
	printf("PASSED\n");
}

int main(int argc, char* argv[])
{
	test1();
	test1a();
	test2();
	test2a();
	test3();
	test4();
	test4a();
	test4b();
	test5();
	test6();
	test7();
	end();
	return 0;
}



