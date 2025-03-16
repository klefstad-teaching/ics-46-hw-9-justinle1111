#include <iostream>
#include "dijkstras.h"

using namespace std;

int main() {
    Graph G;
    file_to_graph("small.txt", G);  // Read graph from file

    vector<int> previous(G.numVertices);
    vector<int> distances = dijkstra_shortest_path(G, 0, previous); // Run Dijkstra

    // Print paths from source (0) to all other vertices
    for (int i = 0; i < G.numVertices; i++) {
        if (i == 0) continue; // Skip source itself

        vector<int> path = extract_shortest_path(distances, previous, i);
        
        if (path.empty()) {
            cout << "No path from 0 to " << i << endl;
        } else {
            cout << "Path from 0 to " << i << ": ";
            for (int v : path) {
                cout << v << " ";
            }
            cout << " | Cost: " << distances[i] << endl;
        }
    }

    return 0;
}
