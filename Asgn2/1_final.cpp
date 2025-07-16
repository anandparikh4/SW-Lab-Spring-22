// Name : Anand Parikh
// Roll no.: 20CS10007
// Software Lab assignment 2 - USE CASE 1

#include<bits/stdc++.h>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

#define ll long long

xml_document<> doc;                  // initialize a new XML document doc, then process it
xml_node<> * root_node = NULL;

bool Rabin_Karp(string,string);      // defined after main

class node{       // to store the data of a node

    ll id;
    double lat,lon;
    string name;

    public :

    node(ll id , double lat , double lon , string name){     // constructor
        this->id = id;
        this->lat = lat;
        this->lon = lon;
        this->name = name;
    }

    void change_name(string s){this->name = s;}       // if name found, update it

    ll get_id(){return this->id;}     // since ID is a private variable, we cannot access directly, so this method can be called to read the ID

    static void search_node_by_name(unordered_map<ll , node>& _map , string check){      // searches for a node whose name has the substring check
        int total=0;

        cout << "\n" ;
        for(pair<ll , node> p : _map){
            if(Rabin_Karp(check , p.second.name)){          // if match found, then prints all info about it
                total++;
                cout << "   ID - " << p.second.id << " , Latitude - " << p.second.lat << " , Longitude - " << p.second.lon ;
                if(!(p.second.name).empty()) cout << " , Name - " << p.second.name << "\n\n" ;
            }
        }

        if(total == 0) cout << "No such node found!\n" ;
    }

    static void search_node_by_ID(unordered_map<ll , node>& _map , ll ID){     // if the user wants to search for a specific node by id, then call this function
        auto it = _map.find(ID);

        if(it != _map.end()){
            pair<ll,node> p = *it ;
            cout << "   Node found :\n  ID - " << p.second.id << " , Latitude - " << p.second.lat << " , Longitude - " << p.second.lon ;
            if(!(p.second.name).empty()) cout << " , Name - " << p.second.name << "\n" ;
        }

        else cout << "   No such node found!\n" ;

        return;
    }


};

class way{     // to store data of ways

    ll id;
    vector<ll> path;  // the list of nodes on this way
    string name;

    public :

    way(ll id , string name){    // constructor
        this->id = id;
        this->name = name;
        this->path.clear();
    }

    void change_name(string s){this->name = s;}     // if name found, then update it

    ll get_id(){return this->id;}     // again, since id is a private variable, this method is used to read it

    void push(ll nd_id){this->path.push_back(nd_id);}     // push a new node in the path of a way

    static void search_way_by_name(unordered_map<ll , way>& _map , string check){      // searches for a way whose name has the substring check

    cout << "\n" ;
    int total=0;
    for(pair<ll , way> p : _map){
        if(Rabin_Karp(check , p.second.name)){        // if match found, then prints all info about it, along with the nodes on it's way
            total++;
            cout << "   ID - " << p.second.id << " , Name - " << p.second.name << "\n   List of node IDS on this way :\n" ;
            for(ll v : p.second.path) cout << "\t" << v << "\n" ;
            cout << "\n" ;
        }
    }

    if(total == 0) cout << "No such way found!\n" ;
    }

    static void search_way_by_id(unordered_map<ll , way>& _map , ll ID){      //if the user wants to search for a specific way by id, then call this function
        auto it = _map.find(ID);

        if(it != _map.end()){
            pair<ll,way> p = *it ;
            cout << "   Way found :\n   ID - " << p.second.id << " , List of node IDs on this Way :\n" ;
            for(ll v : p.second.path){
                cout << v << "\n" ;
            }
        }

        else cout << "   No such way found!\n" ;

        return;
    }
};

int main(){

    // Store the input file in a buffer
    ifstream theFile ("map.osm");
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);       // then parse the buffer

    root_node = doc.first_node("osm");     // the root node is osm (see by opening the file in .txt format) and ALL other nodes are children of it

    int n_nodes=0;    // number of nodes and ways
    int n_ways=0;

    unordered_map<ll,node> nodes;    // to store and access the nodes and ways by their ID in O(1) time
    unordered_map<ll,way> ways;

    // Iterate over the nodes
    for (xml_node<> * _node = root_node->first_node("node"); _node; _node = _node->next_sibling()){

        if(strcmp(_node->name(),"node")!=0) continue;     // check for all nodes with name "node" and store the data accordingly

        n_nodes++;

        ll temp_id = stoll(_node->first_attribute()->value());     // get ID, latitude and longitude
        double temp_lat = stod(_node->last_attribute()->value());
        double temp_lon = stod(_node->last_attribute()->previous_attribute()->value());
        string temp_name = "";     // initialize name to empty string since all nodes might not have a name

        node new_node = node(temp_id , temp_lat , temp_lon , temp_name);    // call constructor

        // iterate over all sub-nodes (called "tags" here) and if a tag "name" is found, store it in node name
        for (xml_node<> * tags = _node->first_node("tag"); tags; tags = tags->next_sibling()){
            if(strcmp(tags->first_attribute()->value() , "name") == 0){
                string new_name = tags->first_attribute()->next_attribute()->value();
                new_node.change_name(new_name);
            }
        }

        nodes.insert({new_node.get_id() , new_node});     // put the new node in the container
    }

    for (xml_node<> * _way = root_node->first_node("way"); _way; _way = _way->next_sibling()){

        if(strcmp(_way->name(),"way")!=0) continue;     // check for all the nodes with name "way" and store the data accordingly

        n_ways++;

        ll temp_id = stoll(_way->first_attribute()->value());     // get ID
        string temp_name = "";     // initialize name to empty string since all ways might not have a name

        way new_way = way(temp_id , temp_name);     // call constructor

        for (xml_node<> * tags = _way->first_node("nd"); tags; tags = tags->next_sibling()){   // then iterate over all sub-nodes (called "tags" here)
            if(strcmp(tags->first_attribute()->name() , "ref") == 0){
                ll nd_id = stoll(tags->first_attribute()->value());        // if any tags by the name "ref" found , then push that node value (node ID)
                new_way.push(nd_id);
            }
            if(strcmp(tags->first_attribute()->value() , "name") == 0){
                string new_name = tags->first_attribute()->next_attribute()->value();     // if name found, store it also
                new_way.change_name(new_name);
            }
        }

        ways.insert({new_way.get_id() , new_way});     // put the new way in the container
    }

    cout << "Number of nodes found : " << n_nodes << "\n" ;
    cout << "Number of ways found : " << n_ways << "\n" ;

    // the menu allows the user to choose from some options to call different functions or terminate
    cout << "Menu :\n1 - Search nodes by name (will give all nodes with input as substring, case INSENSITIVE)\n4 - Search ways by name (again, by substring, case INSENSITIVE)\n3 - Search nodes by ID\n4 - Search way by ID\n0 - TERMINATE PROCESS\n" ;

    int f;
    while(1){
        cout << "Enter the next function to execute : " ;
        cin >> f;

        if(f == 0) break;

        else if(f == 1){
            string s;
            cout << "\nEnter string : " ;
            cin >> s;
            node::search_node_by_name(nodes,s);
            cout << "\n";
        }

        else if(f == 2){
            string s;
            cout << "\nEnter string : " ;
            cin >> s;
            way::search_way_by_name(ways,s);
            cout << "\n" ;
        }

        else if(f == 3){
            ll ID;
            cout << "\nEnter node ID : " ;
            cin >> ID;
            node::search_node_by_ID(nodes,ID);
            cout << "\n";
        }

        else if(f == 4){
            ll ID;
            cout << "\nEnter way ID : " ;
            cin >> ID;
            way::search_way_by_id(ways,ID);
            cout << "\n" ;
        }

        else cout << "\nPlease enter a valid entry!\n" ;
    }

return 0;
}

/// The search tools internally use the Rabin-Karp string matching algorithm, so checking for substring can be done in O(max(l1,l2)), rather than the naive O(l1 * l2)

bool Rabin_Karp(string s1, string s2){  // returns true if s1 is contained in s2, using Rabin_Karp string matching algorithm

transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
transform(s2.begin(), s2.end(), s2.begin(), ::tolower);

ll q = 101;    // a suitable prime number
ll d = 1097;   // a reasonably large number, preferably prime

ll p=0; // hash value for s1
ll t=0; // hash value for s2
ll l1 = s1.size() , l2 = s2.size();
ll i,j,h=1;


for(i=0;i < l1-1;i++) h = (h * d) % q;   // calculating d^(m-1) % q

for(i=0;i<l1;i++){
    p = (d * p + s1[i]) % q;      // find the hash value of s1
    t = (d * t + s2[i]) % q;      // and the hash value of first l1 characters of s2
}

for (i=0;i <= l2-l1;i++){
    if(p==t){                           // if hash values are same, only then check character-wise
        bool flag = true;
        for (j=0;j < l1;j++){
            if (s2[i+j] != s1[j]){
                  flag = false;         // if any character is mismatched, flag = false
                  break;
            }
        }
        if(flag) return true;       // if all characters match, then we have s1 as a substring of s2
    }

    if (i<l2-l1){
        t = (d*(t - s2[i]*h) + s2[i+l1])%q;      // find next hash value by removing first character and adding the next one
            if (t<0) t = t+q;
    }
}

return false;
}


