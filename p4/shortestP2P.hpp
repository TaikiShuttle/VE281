#include<iostream>
#include<list>
#include<vector>
#include<climits>
// You are not allowed to include additional libraries, either in shortestP2P.hpp or shortestP2P.cc

#define INF INT_MAX

using namespace std;

class ShortestP2P {
  public:
      ShortestP2P() {}

      /* Read the graph from stdin
       * The input has the following format:
       *
       * Firstline: 1 unsigned int, the total number of verticies, X.
       * Secondline: 1 unsigned int, the total number of edges between vertices, Y.
       * A set of Y lines, each line contains tuple of 2 unsigned int and 1 int (not unsigned int!), in the format of: A(point 1, unsigned) B(point 2, unsigned) dist(distance, int)
       *
       * Example:
       * 4
       * 4
       * 0 1 5
       * 1 2 -1
       * 2 3 4
       * 3 1 6
       * 
       * 
       * 
       *
       * Vertices that are not connected have a infinitly large distance. You may use INF (previously defined at the top of this cope snippet) for infinitly large distance.
       * 
       * Special: when the graph is not proper, where there exist any pair of vertices whose minimum distance does not exist, terminate immediately by printing:
       * cout << "Invalid graph. Exiting." << endl;
       *
       * Note: vertex pairs that are not connected, which have infinitely large distances are not considered cases where "minimum distances do not exist".
       */
      void readGraph();

      /* Input: 2 vertices A and B
       * Output: distance between them.
       * cout << dist << endl;
       *
       * When the A and B are not connected print INF:
       * cout << "INF" << endl;
       */
      void distance(unsigned int A, unsigned int B);

  private:
    // internal data and functions.
      
    // typedef struct node_t {
    //   int id;
    //   int distance = INF; //distance to the source node
      
    //   node_t(int id) {
    //     this->id = id;
    //   }

    // } node;

    // typedef struct edge_t{
    //   unsigned int begin;
    //   unsigned int end;
    //   int weight;

    //   edge_t(unsigned int begin, unsigned int end, int weight) {
    //     this->begin = begin;
    //     this->end = end;
    //     this->weight = weight;
    //   }
    // } edge;

    //number of edges and vertices
    size_t vertex_num;
    size_t edge_num;

    std::vector<vector<int>> adj_matrix; //define a adjacency_matrix
    // std::vector<int> in_deg; //define a vector that record the in degree of all the nodes
    // std::vector<int> out_deg; //define a vector that record the out degree of all the nodes  
    // std::vector<edge> edges; //define a vector of edges

    //EFFECT: initialize the adjacency matrix
    void initialize() {
      for (unsigned int i = 0; i < vertex_num; i++ ) {
        vector<int> empty_row;
        // in_deg.push_back(0);
        // out_deg.push_back(0);
        for(unsigned int j = 0; j < vertex_num; j++) {
          empty_row.push_back(INF);
        }
        adj_matrix.push_back(empty_row);
      }

      //set the distance to the node itself to 0
      for(unsigned int m = 0; m < vertex_num ; m++) {
        adj_matrix[m][m] = 0;
      }
    }

    // void AddVertex(unsigned int id) {
    //   //if the node already exists, do nothing
    //   for(auto i:nodes) {
    //     if(i.id == id) {
    //       return;
    //     }
    //   }

    //   //else we push the node at the end of the array
    //   node new_node(id);
    //   nodes.push_back(new_node);
    //}

    // //Return the distance ot the source node 
    // int getVertexDist(vector<node> nodes, unsigned int id) {

    // }


    //REQUIRE: The node begin and node end should already been added to the list
    void AddEdge(unsigned int begin, unsigned int end, int weight) {
      adj_matrix[begin][end] = weight; //update the adjacency matrix
      // in_deg[end] ++;
      // out_deg[begin] ++;
      // edge new_edge(begin,end,weight);
      // edges.push_back(new_edge);
      // this->edge_num ++;
    }

    //update the adjacency matrix using the node i as the middle edge
    bool update_for_i(unsigned int i) {
      for(unsigned int a = 0; a < vertex_num; a++) {
        if (adj_matrix[a][i] != INF) { //do the comparsion to rule out useless edges in advance -> reduce about 1000ms
          for(unsigned int b = 0; b < vertex_num; b++) {
            int temp = adj_matrix[a][i] + adj_matrix[i][b];
            if (adj_matrix[i][b] != INF && (temp < adj_matrix[a][b])) {
              adj_matrix[a][b] = temp;
              if(a == b && adj_matrix[a][b] < 0) {
                return false;
              }
            } 
          }
        }
      }
      return true;
    }
};