#include "shortestP2P.hpp"
void ShortestP2P::readGraph() {
  cin>>this->vertex_num;
  cin>>this->edge_num;
//   cout<<"initialize"<<endl;
  initialize();
//   cout<<"add edge"<<endl;
  for(unsigned int i = 0; i< edge_num; i++) {
    unsigned int begin_node;
    unsigned int end_node;
    int weight;
    cin>>begin_node>>end_node>>weight;

    // AddVertex(begin_node);
    // AddVertex(end_node);
    AddEdge(begin_node, end_node, weight);
  }  
//   cout<<"after adding edge"<<endl;
  for(unsigned int i = 0; i < vertex_num; i++) {
    // if(in_deg[i] != 0 && out_deg[i] != 0) {
      if(!update_for_i(i)) {
        cout<<"Invalid graph. Exiting."<<endl;
        exit(0);
      }
    }
  // }
}


void ShortestP2P::distance(unsigned int A, unsigned int B) {
    if(adj_matrix[A][B] == INF) {
        cout<<"INF"<<endl;
    }
    else {
        cout<<adj_matrix[A][B]<<endl;
    }
}