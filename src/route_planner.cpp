#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
  start_node =& m_Model.FindClosestNode(start_x,start_y);
  end_node=& m_Model.FindClosestNode(end_x,end_y);
  //m_Model.FindClosestNode()

}



float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {

    //RouteModel::Node obj;
    //return(obj.distance(node));
    return(node->distance(*end_node));
}



void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
   // RouteModel::Node obj;
    //obj.neighbors.push_back(current_node);
    current_node->FindNeighbors();
    for(auto i:current_node->neighbors)
    {
        i->parent=current_node;
        i->h_value=CalculateHValue(i);
        i->g_value=current_node->g_value+current_node->distance(*i);
        open_list.push_back(i);
        i->visited=true;
        
    }
}


}

RouteModel::Node* RoutePlanner::NextNode() {
    // sorting custom objects
    // I sorted descending here so the smallest node is at the back of the vector
    std::sort(open_list.begin(), open_list.end(), [](const auto &lhs, const auto &rhs) {
        return (lhs->g_value + lhs->h_value) > (rhs->g_value + rhs->h_value);
    });

    RouteModel::Node *next_node = open_list[open_list.size() - 1];
    open_list.pop_back();
    return next_node;
}




std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;
   // std::vector<RouteModel::Node> path_found1;
    RouteModel::Node *temp_node=current_node;
    //for(int tt=0;ni!=)

    // TODO: Implement your solution here.
    while(temp_node->parent != nullptr)
    {
        path_found.push_back(*temp_node);
        
        distance=distance+(temp_node->distance(* temp_node->parent));
    

        temp_node=temp_node->parent;
        
    }
    
    path_found.push_back(*temp_node);
    reverse(path_found.begin(),path_found.end());
  
    
    
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}

void RoutePlanner::AStarSearch() {
    start_node->visited=true;
    open_list.push_back(start_node);
    RouteModel::Node *current_node = nullptr;
    while(!open_list.empty())
    {
       // AddNeighbors(current_node);
        current_node=NextNode();
    
    if(current_node->distance(*end_node)==0)
    {
        m_Model.path=ConstructFinalPath(current_node);
        return;
    }
    AddNeighbors(current_node);
    }
    

}