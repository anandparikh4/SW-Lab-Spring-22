// Name : Anand Parikh
// Roll no.: 20CS10007
// Software Lab assignment 2 - USE CASE 3

#include<bits/stdc++.h>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

#define ll long long
#define INF DBL_MAX

xml_document<> doc;
xml_node<> * root_node = NULL;

double convert_to_radians(double x){       // converts degreed to radians
    double unit_degree = M_PI / 180;
    return unit_degree * x;
}

double geodesic_distance(double lat1,double lon1,double lat2,double lon2){      // calculates geodesic distance between a pair of (latitude , longitude) pairs

    lat1 = convert_to_radians(lat1);
    lat2 = convert_to_radians(lat2);
    lon1 = convert_to_radians(lon1);
    lon2 = convert_to_radians(lon2);

    double lon = lon2 - lon1;
    double lat = lat2 - lat1;

    double ans = pow(sin(lat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(lon / 2), 2);      // Haversine formula

    double Re = 6371;
    ans = 2 * Re * asin(sqrt(ans));

    return ans;
}

class node{
    public :
    ll id;
    double lat,lon;
    string name;
};

class way{
    public :
    ll id;
    vector<ll> path;
};

//typedef struct node node;

//typedef struct way way;

double Dijkstra(vector<unordered_map<ll , double>>& a,ll start,ll n,ll finish,unordered_map<ll,ll>& b){

    set<pair<double,ll>> s;               // the first integer is distance and the second is city number
    vector<double> distance(n,INF);       // stores the distances of all elements from the starting node
    vector<ll> nearest(n,start);          // stores the nearest node on the shortest path start to current

    s.insert({0,start});
    distance[start] = 0;         // insert the starting node, as it is closest to itself with distance 0
    nearest[start] = -1;

    while(!s.empty()){
        pair<double,ll> curr = *(s.begin());       // pop the nearest node
        s.erase(s.begin());

        ll vnear = curr.second;
        if(vnear == finish) break;           // if it is the finish node, we need not proceed further

        for(pair<ll,double> v : a[vnear]){
            if(distance[v.first] > distance[vnear] + v.second){       // if a shorter path through vnear is found

                if(distance[v.first]<INF) s.erase(s.find({distance[v.first],v.first}));     // if it is already a candidate for nearest, then pop it

                distance[v.first] = distance[vnear] + v.second;     // update distance
                s.insert({distance[v.first],v.first});              // add it to list of candidate nodes for the nearest one
                nearest[v.first] = vnear;                           // update nearest node
            }
        }
    }

    stack<ll> stk;
    ll x = finish;
    while(x!=-1){             // just go to parent or nearest, till the start is found
        stk.push(x);
        x = nearest[x];
    }

    cout << "\nThe nodes on the path are :\n" ;

    while(stk.top()!=finish){    // then reverse the stack by popping and print the path
        cout << b.find(stk.top())->second << " -> " ;
        stk.pop();
    }
    cout << b.find(stk.top())->second ;
    stk.pop();
    cout << "\n\n" ;

    /*for(ll i=0;i<n;i++) cout << distance[i] << " ";
    cout << endl;

    return;*/

    return distance[finish];
}

int main(){

    // Store the input file in a buffer
    ifstream theFile ("map.osm");
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);       // then parse the buffer

    root_node = doc.first_node("osm");

    //cout << "Name of my first node is: " << doc.first_node()->name() << "\n";

    int n_nodes=0;
    int n_ways=0;

    unordered_map<ll,node> nodes;
    unordered_map<ll,ll> node_id_to_index;
    unordered_map<ll,ll> index_to_node_id;
    unordered_map<ll,way> ways;

    // Iterate over the nodes
    for (xml_node<> * _node = root_node->first_node("node"); _node; _node = _node->next_sibling()){
        if(strcmp(_node->name(),"node")!=0) continue;

        node new_node;

        new_node.id = stoll(_node->first_attribute()->value());
        new_node.lon = stod(_node->last_attribute()->value());
        new_node.lat = stod(_node->last_attribute()->previous_attribute()->value());
        new_node.name = "";

        for (xml_node<> * tags = _node->first_node("tag"); tags; tags = tags->next_sibling()){
            if(strcmp(tags->first_attribute()->value() , "name") == 0) new_node.name = tags->first_attribute()->next_attribute()->value();
        }

        node_id_to_index.insert({new_node.id , n_nodes});
        //cout << n_nodes << endl;
        index_to_node_id.insert({n_nodes , new_node.id});
        nodes.insert({new_node.id , new_node});

        n_nodes++;
    }

    //cout << n_nodes << endl;
    vector<unordered_map<ll,double>> a(n_nodes);    // adjacency list

    int edges = 0;

    for (xml_node<> * _way = root_node->first_node("way"); _way; _way = _way->next_sibling()){
        if(strcmp(_way->name(),"way")!=0) continue;

        n_ways++;
        way new_way;

        new_way.id = stoll(_way->first_attribute()->value());
        int cnt=0;

        for (xml_node<> * tags = _way->first_node("nd"); tags; tags = tags->next_sibling()){
            if(strcmp(tags->first_attribute()->name() , "ref") != 0) continue;

            ll nd_id = stoll(tags->first_attribute()->value());
            new_way.path.push_back(nd_id);

            if(cnt > 0){
                edges++;
                node node_x = nodes.find(new_way.path[cnt-1])->second;
                node node_y = nodes.find(new_way.path[cnt])->second;

                ll x = node_id_to_index.find(node_x.id)->second;
                ll y = node_id_to_index.find(node_y.id)->second;
                double weight = geodesic_distance(node_x.lat , node_x.lon , node_y.lat , node_y.lon);

                auto check1 = a[x].find(y);
                if(check1!=a[x].end()) check1->second = min(check1->second , weight);
                else a[x].insert({y , weight});

                auto check2 = a[y].find(x);
                if(check2!=a[y].end()) check2->second = min(check2->second , weight);
                else a[y].insert({x , weight});
            }

            cnt++;
        }

        ways.insert({new_way.id , new_way});
    }

    //cout << edges << endl;
    cout << "Menu\n1 - Find shortest distance between nodes\n0 - Terminate Process\n\n" ;
    int f;
    while(1){
        cout << "\nEnter new function: " ;
        cin >> f ;

        if(f == 0) break;

        else if (f!=1){
            cout << "\nPlease enter a valid function!\n";
            continue;
        }

        cout << "Enter the ID of the start node : " ;
        ll start;
        while(1){
            cin >> start;
            if(nodes.find(start) == nodes.end()){
                cout << "No such node found! Please enter a valid ID : " ;
                continue;
            }
            start = node_id_to_index.find(start)->second;
            break;
        }

        cout << "Enter the ID of the finish node : " ;
        ll finish;
        while(1){
            cin >> finish;
            if(nodes.find(finish) == nodes.end()){
                cout << "No such node found! Please enter a valid ID : " ;
                continue;
            }
            finish = node_id_to_index.find(finish)->second;
            break;
        }

        double shortest = Dijkstra(a , start , n_nodes , finish, index_to_node_id);
        if(shortest == DBL_MAX) cout << "No path!\n" ;
        else cout << "The shortest path distance between the two places is : " << shortest << " km\n" ;
    }

return 0;
}


