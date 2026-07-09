#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <iostream>
#include <fstream>
#include <utility>
#include <functional>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <limits>

template <typename T>
class Graph {
 private:
  std::vector<std::unordered_map<int, T> > adjList {};
  int numVertices {};

 public:
  // empty graph with N vertices
  explicit Graph(int N);

  // construct graph from edge list in filename
  explicit Graph(const std::string& filename);

  // add an edge directed from vertex i to vertex j with given weight
  void addEdge(int i, int j, T weight);

  // removes edge from vertex i to vertex j
  void removeEdge(int i, int j);

  // is there an edge from vertex i to vertex j?
  bool isEdge(int i, int j) const;

  // return weight of edge from i to j
  // will throw an exception if there is no edge from i to j
  T getEdgeWeight(int i, int j) const;

  // returns number of vertices in the graph
  int size() const;

  // return iterator to a particular vertex
  const std::unordered_map<int, T>& neighbours(int a) const {
    return adjList.at(a);
  }
};

template <typename T>
Graph<T>::Graph(int N) : adjList(N), numVertices {N} {}

template <typename T>
Graph<T>::Graph(const std::string& inputFile) {
  std::ifstream infile {inputFile};
  if (!infile) {
    std::cerr << inputFile << " could not be opened\n";
    return;
  }
  // first line has number of vertices
  infile >> numVertices;
  adjList.resize(numVertices);
  int i {};
  int j {};
  double weight {};
  // assume each remaining line is of form
  // origin dest weight
  while (infile >> i >> j >> weight) {
    addEdge(i, j, static_cast<T>(weight));
  }
}

template <typename T>
int Graph<T>::size() const {
  return numVertices;
}

template <typename T>
void Graph<T>::addEdge(int i, int j, T weight) {
  if (i < 0 or i >= numVertices or j < 0 or j >= numVertices) {
    throw std::out_of_range("invalid vertex number");
  }
  adjList[i].insert({j, weight});
}

template <typename T>
void Graph<T>::removeEdge(int i, int j) {
  // check if i and j are valid
  if (i >= 0 && i < numVertices && j >= 0 && j < numVertices) {
    adjList[i].erase(j);
  }
}

template <typename T>
bool Graph<T>::isEdge(int i, int j) const {
  if (i >= 0 && i < numVertices && j >= 0 && j < numVertices) {
    return adjList.at(i).contains(j);
  }
  return false;
}

template <typename T>
T Graph<T>::getEdgeWeight(int i, int j) const {
  return adjList.at(i).at(j);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Graph<T>& G) {
  for (int i = 0; i < G.size(); ++i) {
    out << i << ':';
    for (const auto& [neighbour, weight] : G.neighbours(i)) {
      out << " (" << i << ", " << neighbour << ")[" << weight << ']';
    }
    out << '\n';
  }
  return out;
}


// APSP functions
// Use this function to return an "infinity" value
// appropriate for the type T
template <typename T>
T infinity() {
  if (std::numeric_limits<T>::has_infinity) {
    return std::numeric_limits<T>::infinity();
  } else {
    return std::numeric_limits<T>::max();
  }
}

// implement an algorithm for determining if G
// has a negative weight cycle here
template <typename T>
bool existsNegativeCycle(const Graph<T>& G) {
  
  // first, we need to create a distance vector of size n (number of vertices)
  // we initialise all entries of size n, to zero, to showcase a fake vertex 's'
  // that connects to every vertex in the graph with edges of zero weight.
  std::vector<T> distance(G.size(), 0);

  for (int n = 0; n < G.size(); n++) {
    // to check if we have a shorter distance after another traversal through edges
    bool weightChangeChecker = false;

    // check all edges for relaxtion
    for (int vertex = 0; vertex < G.size(); vertex++) {
      // Inner Loop part 2: Loop over all of the current vertex's connected edges
      // using the neighbour function
      for (const auto& vertexWeightPair : G.neighbours(vertex)) {
        int v = vertexWeightPair.first;
        T weight = vertexWeightPair.second;
        // edge relaxation occurs here
        if (distance[vertex] + weight < distance[v]) {
          distance[v] = distance[vertex] + weight;
          weightChangeChecker = true;
        }
      }
    }

    // On the last iteration and there was a change seen after a relaxation,
    // a negative cycle has been detected.
    if (n == G.size() - 1 && weightChangeChecker == true) {
      return true;
    }
  }
  return false;
}

// implement Johnson's APSP algorithm here
template <typename T>
std::vector<std::vector<T> >
johnsonAPSP(const Graph<T>& G) {
  
  // Pre-check to see if graph has negative cycles as johnson algorithm cannot be
  // applied to graphs with negative cycles
  if (existsNegativeCycle(G) == true) {
    // I guess we can return nothing? 
    return {};
  }

  // Similar to our function 1, we must use a vertex 's' with the bellmanford algorithm
  // to gather potential vertex's that may be h(v). this being the shortest path 
  // distance from our new vertex 's' to 'v'
  std::vector<T> h(G.size(), 0);

  for (int n = 0; n < G.size(); n++) {
    for (int vertex = 0; vertex < G.size(); vertex++) {
      for (const auto& vertexWeightPair : G.neighbours(vertex)) {
        int v = vertexWeightPair.first;
        T weight = vertexWeightPair.second;
        // edge relaxation
        if (h[vertex] + weight < h[v]) {
          h[v] = h[vertex] + weight;
        }
      }
    }
  }

  // with a new graph, the edges are reweighted using our 'vertext potential
  // function' from h(v).
  // by having all edge weights to not be negative, it allows us to utilise
  // dijkstras algorithm after
  Graph<T> newGraphReweighted(G.size());

  for (int vertex = 0; vertex < G.size(); vertex++) {
    for (const auto& vertexWeightPair : G.neighbours(vertex)) {
      int v = vertexWeightPair.first;
      T weight = vertexWeightPair.second;
      T newWeight = weight + h[vertex] - h[v];
      newGraphReweighted.addEdge(vertex, v, newWeight);
    }
  }

  // finally, dijkstras algorithm is used once from every vertex in our new graph
  // and then adjust the results back to the original weights using our 'h'.
  
  // first we need to setup the dijkstra algorithm:
  std::vector<std::vector<T>> finalDistanceMatrix(G.size(), std::vector<T>(G.size(), infinity<T>()));  
  // ^^ for above, we initialise it with infinity and not 0 or other numbers to 
  // avoid incorrect comparisons.

  for (int s = 0; s < G.size(); s++) {
    // set up the SSSP distances from the source vertex
    std::vector<T> distance(G.size(), infinity<T>());
    distance[s] = 0;

    // Priority Queue setup for dijkstras algorithm
    std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>, std::greater<std::pair<T, int>>> pq;
    pq.push(std::make_pair(0, s));

    while (!pq.empty()) {
      std::pair<T, int> topPair = pq.top();
      int v = topPair.second;
      pq.pop();

      for (const auto& vertexWeightPair : newGraphReweighted.neighbours(v)) {
        int u = vertexWeightPair.first;
        T weight = vertexWeightPair.second;
        if (distance[v] + weight < distance[u]) {
          distance[u] = distance[v] + weight;
          pq.push({distance[u], u});
        }
      }
    }

    // finally we convert the dijkstras distances from reweighted graph back to
    // the original distances using the new vertex potential.
    for (int v = 0; v < G.size(); v++) {
      if (distance[v] != infinity<T>()) {
        finalDistanceMatrix[s][v] = distance[v] + h[v] - h[s];
      }
    }
  }

  return finalDistanceMatrix;
}

// implement the Floyd-Warshall APSP algorithm here
template <typename T>
std::vector<std::vector<T> >
floydWarshallAPSP(const Graph<T>& G) {
  
  // Distance Matrix initialisation setup Part 1
  std::vector<std::vector<T>> distanceMatrix(G.size(), std::vector<T>(G.size(), 0));

  // Setup Part 2
  for (int i = 0; i < G.size(); i++) {
    for (int j = 0; j < G.size(); j++) {
      if (i == j) {
        distanceMatrix[i][j] = 0;
      } else if (G.isEdge(i, j) == true) {
        distanceMatrix[i][j] = G.getEdgeWeight(i, j);
      } else if (G.isEdge(i, j) == false) {
        distanceMatrix[i][j] = infinity<T>();
      }
    }
  }

  // Now that the distanceMatrix setup is done, we have to iteratively improve on it
  // by checking if the path from i to j can be made smaller by going through a
  // intermediate vertex | (In our case, 'x' is our intermediate vertex)
  for (int m = 0; m < G.size(); m++) {
    for (int i = 0; i < G.size(); i++) {
      for (int j = 0; j < G.size(); j++) {
        // Update for distance matrix 
        // Note: need to account for this condition failing, the infinity check prevents
        // invalid updates by skipping cases where part of the path is unreachable
        if (distanceMatrix[i][m] != infinity<T>() && distanceMatrix[m][j] != infinity<T>()) {
          if (distanceMatrix[i][m] + distanceMatrix[m][j] < distanceMatrix[i][j]) {
            distanceMatrix[i][j] = distanceMatrix[i][m] + distanceMatrix[m][j];
          }
        }
      }
    }
  }

  return distanceMatrix;
}

#endif      // GRAPH_HPP_
