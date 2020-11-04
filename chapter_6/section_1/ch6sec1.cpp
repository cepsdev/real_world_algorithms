/*
*
* ch6sec1.cpp - implements topological sort
*
* Implementation of algorithms and data structures as described in section 1 of chapter 6 of 
* the book 'Real-World Algorithms - A Beginner's Guide' by PANOS LOURIDAS.
*
* 
* Should compile with any C++ 11 conformant compiler.
* 
* Copyright (c) 2020 Tomas Prerovsky.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <iostream>
#include "graph.hpp"
using namespace std;
using namespace real_world_algorithms;


void print(ostream& os, Graph const& g) {
	for (size_t i = 0; i < g.adjacency_lists.size();)
	{
		auto u = g.adjacency_lists[i];
		auto n = g.adjacency_lists[i+1];
		if (n != 0) {
			for (auto j = 0; j < n; ++j)
				os << u << " -> " << g.adjacency_lists[i + 2 + j] << "\n";
		} else os << u << "\n";

		i += n + 2;
	}
}

void debug_print(ostream& os, Graph const& g) {
	for (auto v : g.adjacency_lists)
		os << v << "\n";
}

void Graph::DFSTopologicalSort(Graph::node_t node, vector<bool>& visited, vector<node_t>& sorted) {
	visited[node] = true;
	for (auto v : AdjacencyList(node)) {
		if (!visited[v])
			DFSTopologicalSort(v,visited,sorted);
	}
	sorted.insert(sorted.begin(), node);
}

vector<Graph::node_t> Graph::TopologicalSort() {
	vector<bool> visited(node_ctr,false);
	vector<node_t> sorted;
	for (int i = 0; i < node_ctr; ++i)
		if (!visited[i])
			DFSTopologicalSort(i, visited, sorted);
	return sorted;
}

int main()
{
	// Graph example, see Figure 6.1 p.150
	Graph g;
	auto Alice = g.push(); 
	auto Bob = g.push();
	auto Charlie = g.push();
	auto Dan = g.push();
	auto Erin = g.push();
	auto Frank = g.push();
	auto George = g.push();
	auto Harry = g.push();
	auto Irene = g.push();
	auto Janet = g.push();
	auto Kenneth = g.push();
	g.add_edge(Alice, Bob);
	g.add_edge(Alice, Dan);
	g.add_edge(Dan, Erin);
	g.add_edge(Bob, Charlie);
	g.add_edge(Erin, Charlie);
	g.add_edge(Erin, Frank);
	g.add_edge(George, Dan);
	g.add_edge(George, Harry);
	g.add_edge(Irene, George);
	g.add_edge(Irene, Janet);	
	auto v = g.TopologicalSort();
	for (auto u : v) cout << u << "\n";
}