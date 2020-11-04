/*
*
* graph.hpp - contains a graph class
*
* Implementation of algorithms and data structures as described in section 1 of chapter 6 of 
* the book 'Real-World Algorithms - A Beginner's Guide' by PANOS LOURIDAS.
*
* 
* Should compile with any C++ 11 conformant compiler.
*
*
*   
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

#include <vector>

namespace real_world_algorithms{ 

using namespace std;

struct unweighted {};
template<typename T> struct graph;
template<typename T,size_t small_buffer_size = 128> struct graph_rep;

template<typename T, size_t small_buffer_size> struct graph_rep{
	using node_t = int;
    node_t node_ctr = 0;
   	char* adjacency_lists;
    size_t allocated = 0;
    size_t used = 0;
    bool must_free = false;
    char small_buffer[small_buffer_size];
    static constexpr auto adj_list_header = sizeof(node_t) + sizeof(size_t);
    static constexpr auto adj_list_entry = sizeof(node_t) + sizeof(T);
    template<typename U> void reserve(U const & u){
        *((U*)(adjacency_lists + used)) = u;
        used += sizeof(U);
    }

    graph_rep () : adjacency_lists{small_buffer} {

    }
    void make_space(size_t chunk_size){
        if (used + chunk_size <= allocated) return;
    } 
	node_t push() {
        make_space(adj_list_header);
        reserve(node_ctr);
        reserve(size_t{});
		return node_ctr++;
	}

    size_t size() const {
        return allocated;
    }

    std::pair<node_t,size_t> adj_list_header_at(size_t ofs){
        return {*(node_t*)(adjacency_lists+ofs),*(size_t*)(adjacency_lists+ofs+sizeof(node_t))};
    }   
};


using Graph = graph<unweighted>; 

template<>
struct graph<unweighted> : graph_rep<unweighted>{
	/*
	* Nodes are integers.
    * Weights are of type T 
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
	/*
	* Creates a new vertex, pushes an empty adjacency list at the end of adjacency_lists.
	* Returns index of vertex.
	*/
	/*
	* Inserts an edge (u,v). 
	*/
	void add_edge(int u, int v) {
		if (node_ctr <= u || node_ctr <= v) return;
		for (size_t i = 0; i < graph_rep::size();) {
			auto [node,number_of_edges] = adj_list_header_at(i);
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



}
