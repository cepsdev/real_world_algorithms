/*
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
#include <vector>
using namespace std;

struct Graph {
	/*
	* Nodes are integers.
	* Graph structure is represented by adjacency lists which are stored in a single array.
	* For each node u in V there is exactly one block of consecutive integers of the form: 
	* Index(u),deg(u),v_1,...,v_k (k = deg(v)), where v_i = Index(v) for some v with (u,v) in E.
	* Examples:
	* A valid representation of the graph G = ( V = {0,1}, E = {(0,1)}) is 	
	* [0,1,1,1,0];
	* A valid representation of the graph G = ( V = {0,1}, E = {(0,1),(1,0)}) is
	* [0,1,1,1,1,0];
	* A valid representation of the graph G = ( V = {0,1,2}, E = {(0,1),(1,0)}) is
	* [0,1,1,1,1,0,2,0];
	*/
	using node_t = int;
	vector<node_t> adjacency_lists;
	node_t node_ctr = {};
	/*
	* Creates a new vertex, pushes an empty adjacency list at the end of adjacency_lists.
	* Returns index of vertex.
	*/
	node_t push() {
		adjacency_lists.push_back(node_ctr);
		adjacency_lists.push_back({});
		return node_ctr++;
	}
	/*
	* Inserts an edge (u,v). 
	*/
	void add_edge(int u, int v) {
		if (node_ctr <= u || node_ctr <= v) return;
		for (size_t i = 0; i < adjacency_lists.size();) {
			auto node = adjacency_lists[i];
			if (node != u) {
				i += adjacency_lists[i + 1] + 2;
				continue;
			}
			++adjacency_lists[i+1];
			adjacency_lists.emplace(adjacency_lists.begin() + i + 2, v);
			return;
		}		
	}

	struct adj_list {
		struct iterator {
			adj_list& parent;
			int ofs;
			bool operator == (iterator rhs) const { return parent.start == rhs.parent.start && ofs == rhs.ofs && &parent.adjacency_lists == &rhs.parent.adjacency_lists; }
			bool operator != (iterator rhs) const { return ! this->operator== (rhs); }
			iterator operator ++ () { ++ofs;  return *this; }
			node_t operator * () { return parent.adjacency_lists[parent.start + 2 + ofs] ;  }
		};
		bool valid;
		size_t start;
		vector<node_t>& adjacency_lists;
		iterator begin() { return {*this,0}; }
		iterator end() { return {*this,adjacency_lists[start + 1]}; }
	};

	adj_list AdjacencyList(node_t node) {
		if (node >= node_ctr) return adj_list{ false, {}, adjacency_lists };
		for (size_t i = 0; i < adjacency_lists.size();) {
			if (node == adjacency_lists[i]) return adj_list{ true, i, adjacency_lists };
			i += adjacency_lists[i + 1] + 2;
		}
		return adj_list{ false, {}, adjacency_lists };
	}
	
	/*
	* Computes a topological sort.
	* Precondition: graph is cycle free.
	*/
	vector<node_t> TopologicalSort();
private:
	void DFSTopologicalSort(Graph::node_t node, vector<bool>& visited, vector<node_t>& sorted);
};

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