#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <climits> // For INT_MAX
#include <utility> // For std::pair
#include <tuple>   // For std::tuple
#include <algorithm> // For std::sort

// 优先级队列封装
template<typename T, typename Compare = std::greater<T>>
class PriorityQueue {
private:
    std::priority_queue<T, std::vector<T>, Compare> pq;

public:
    void push(const T& value) {
        pq.push(value);
    }

    void pop() {
        pq.pop();
    }

    T top() const {
        return pq.top();
    }

    bool empty() const {
        return pq.empty();
    }

    size_t size() const {
        return pq.size();
    }
};

// 图数据结构
class Graph {
public:
    Graph(int vertices) : vertices(vertices) {
        adjList.resize(vertices);
    }

    void addEdge(int u, int v, int weight = 1) {
        adjList[u].push_back({ v, weight });
        adjList[v].push_back({ u, weight }); // 无向图
    }

    void printGraph() const {
        for (int i = 0; i < vertices; ++i) {
            std::cout << i << ": ";
            for (auto& edge : adjList[i]) {
                std::cout << "(" << edge.first << ", " << edge.second << ") ";
            }
            std::cout << std::endl;
        }
    }

    const std::vector<std::list<std::pair<int, int>>>& getAdjList() const {
        return adjList;
    }

    int getVertices() const {
        return vertices;
    }

private:
    int vertices;
    std::vector<std::list<std::pair<int, int>>> adjList;
};

// BFS 算法
void BFS(const Graph& graph, int start) {
    int vertices = graph.getVertices();
    std::vector<bool> visited(vertices, false);
    std::queue<int> q;

    visited[start] = true;
    q.push(start);

    std::cout << "BFS: ";
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        std::cout << v << " ";

        for (auto& edge : graph.getAdjList()[v]) {
            int neighbor = edge.first;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    std::cout << std::endl;
}

// DFS 算法
void DFSUtil(const Graph& graph, int v, std::vector<bool>& visited) {
    visited[v] = true;
    std::cout << v << " ";

    for (auto& edge : graph.getAdjList()[v]) {
        int neighbor = edge.first;
        if (!visited[neighbor]) {
            DFSUtil(graph, neighbor, visited);
        }
    }
}

void DFS(const Graph& graph, int start) {
    int vertices = graph.getVertices();
    std::vector<bool> visited(vertices, false);

    std::cout << "DFS: ";
    DFSUtil(graph, start, visited);
    std::cout << std::endl;
}

// Dijkstra 算法
void dijkstra(const Graph& graph, int start) {
    int vertices = graph.getVertices();
    std::vector<int> dist(vertices, INT_MAX);
    dist[start] = 0;

    using PII = std::pair<int, int>; // {distance, vertex}
    PriorityQueue<PII, std::greater<PII>> pq;
    pq.push({ 0, start });

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto& edge : graph.getAdjList()[u]) {
            int v = edge.first, weight = edge.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({ dist[v], v });
            }
        }
    }

    std::cout << "Shortest distances from vertex " << start << ": ";
    for (int i = 0; i < vertices; ++i) {
        std::cout << (dist[i] == INT_MAX ? -1 : dist[i]) << " ";
    }
    std::cout << std::endl;
}

// Kruskal 最小生成树算法
class UnionFind {
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unionSets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) return false;

        if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        }
        else if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        }
        else {
            parent[rootY] = rootX;
            ++rank[rootX];
        }
        return true;
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

void kruskalWithPriorityQueue(const Graph& graph) {
    int vertices = graph.getVertices();
    using Edge = std::tuple<int, int, int>; // {weight, u, v}

    PriorityQueue<Edge, std::greater<Edge>> edgeQueue;

    // 将所有边加入优先级队列
    for (int u = 0; u < vertices; ++u) {
        for (auto& edge : graph.getAdjList()[u]) {
            int v = edge.first, weight = edge.second;
            if (u < v) { // 避免重复边
                edgeQueue.push({ weight, u, v });
            }
        }
    }

    UnionFind uf(vertices);
    std::vector<Edge> mst;

    while (!edgeQueue.empty() && mst.size() < vertices - 1) {
        auto edge = edgeQueue.top();
        edgeQueue.pop();

        int weight = std::get<0>(edge);
        int u = std::get<1>(edge);
        int v = std::get<2>(edge);

        if (uf.unionSets(u, v)) {
            mst.push_back(edge);
        }
    }

    std::cout << "Minimum Spanning Tree (Kruskal with Priority Queue):" << std::endl;
    for (auto& edge : mst) {
        int weight = std::get<0>(edge);
        int u = std::get<1>(edge);
        int v = std::get<2>(edge);
        std::cout << u << " - " << v << " : " << weight << std::endl;
    }
}

// 主函数
int main() {
    Graph g(6);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 4);
    g.addEdge(1, 2, 2);
    g.addEdge(1, 3, 5);
    g.addEdge(2, 3, 8);
    g.addEdge(3, 4, 6);
    g.addEdge(4, 5, 9);

    std::cout << "Graph:" << std::endl;
    g.printGraph();

    BFS(g, 0);
    DFS(g, 0);

    dijkstra(g, 0);
    kruskalWithPriorityQueue(g);

    return 0;
}
