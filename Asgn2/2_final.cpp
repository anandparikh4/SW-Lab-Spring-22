// Name : Anand Parikh
// Roll no.: 20CS10007
// Software Lab assignment 2 - USE CASE 2

#include<bits/stdc++.h>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

#define ll long long

xml_document<> doc;                 // Parsing is the same as in Program 1
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

    double ans = pow(sin(lat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(lon / 2), 2);     /// Haversine formula

    double Re = 6371;
    ans = 2 * Re * asin(sqrt(ans));

    return ans;
}

class node{                   // the geographical node structure
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

    void change_name(string s){this->name = s;}     // if name found, then update it

    ll get_id(){return this->id;}     // since ID is a private variable, we cannot access directly, so this method can be called to read the ID, similarly for lat and lon

    double get_lat(){return this->lat;}

    double get_lon(){return this->lon;}

};

struct heap_node{        // note that this node has NOTHING to do with graphs, it is a heap node
    ll id;
    int index;        // node ID, distance from the central node , index in heap (DIFFERENT FROM "node" STRUCTURE)
    double dist;
};

/// THE FOLLOWING ARE SOME CUSTOM MADE MINIMUM HEAP DATA STRUCTURE FUNCTIONS

struct heap_node * find_min(struct heap_node ** heap,int _size){   // return the min element of a min heap
    if(_size > 0) return heap[0];
    return NULL;
}

void _swap(struct heap_node ** a,struct heap_node ** b){    // custom swap function to swap heap node pointers
    struct heap_node * temp = *a;
    *a = *b;
    *b = temp;
    return;
}

void min_heapify(struct heap_node ** heap,int first,int _size){   // the min heapify function - O(log n) time

    int i = first;
    int last = _size-1;
    while(1){
        int l = 2*i+1;
        int r = 2*i+2;

        if(l > last) return;

        int mN = r;

        if(r > last) mN = l;
        else if(heap[l]->dist < heap[r]->dist) mN = l;

        if(heap[i]->dist <= heap[mN]->dist) return;

        _swap(&heap[i] , &heap[mN]);
        i = mN;
    }

    return;
}

void delete_min(struct heap_node ** heap,int *_size){    // to delete the top (minimum) element from a min heap - O(log n) time

heap[0] = heap[(*_size)-1];
(*_size)--;

min_heapify(heap,0,(*_size));

return;
}

void insert_min(struct heap_node ** heap,int *_size,struct heap_node * ele){     // to insert an element in a min heap - O(log n) time

    (*_size)++;
    heap[(*_size)-1] = ele;

    int i = (*_size)-1;
    int parent;

    while(1){
        if(i==0) return;

        parent = (i-1)/2;

        if(heap[parent]->dist <= heap[i]->dist) return;

        _swap(&heap[i] , &heap[parent]);
        i = parent;
    }

    return;
}

void build_Heap(struct heap_node ** heap,int _size){   // build heap - O(n) time

    int i;
    for(i=_size/2 - 1;i>=0;i--){
        min_heapify(heap,i,_size);
    }

    return;
}


int main(){

    cout << fixed << setprecision(7) ;   // printing numbers accurate up to 7 decimal places

    // Store the input file in a buffer
    ifstream theFile ("map.osm");
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);       // then parse the buffer

    root_node = doc.first_node("osm");

    int n_nodes=0;           // number of nodes

    unordered_map<ll,node> nodes;

    // Iterate over the nodes and store them, like in program 1
    for (xml_node<> * _node = root_node->first_node("node"); _node; _node = _node->next_sibling()){
        if(strcmp(_node->name(),"node")!=0) continue;

        n_nodes++;

        ll temp_id = stoll(_node->first_attribute()->value());     // get ID, latitude and longitude
        double temp_lat = stod(_node->last_attribute()->value());
        double temp_lon = stod(_node->last_attribute()->previous_attribute()->value());
        string temp_name = "";     // initialize name to empty string since all nodes might not have a name

        node new_node = node(temp_id , temp_lat , temp_lon , temp_name);    // call constructor

        for (xml_node<> * tags = _node->first_node("tag"); tags; tags = tags->next_sibling()){
            if(strcmp(tags->first_attribute()->value() , "name") == 0){
                string new_name = tags->first_attribute()->next_attribute()->value();
                new_node.change_name(new_name);
            }
        }

        nodes.insert({new_node.get_id() , new_node});        // insert the new object in the constructor
    }

    ll node_id;
    node center = node(-1 , -1 , -1 , "");
    cout << "Enter node ID : " ;
    while(1){                                             // taking input
        cin >> node_id;
        auto c = nodes.find(node_id);
        if(c == nodes.end()){
            cout << "\nNo such node exists! Please enter a valid ID : " ;
            continue;
        }
        center = c->second;
        break;
    }

    struct heap_node ** heap = (struct heap_node **)malloc(n_nodes*sizeof(struct heap_node*));    // declaring the heap
    int i=0;

    // Populate the elements of the heap in random order, then call build heap to order them
    for(pair<ll , node> p : nodes){
        if(p.first == node_id) continue;   // put in all except the same node itself, of course

        double d = geodesic_distance(center.get_lat() , center.get_lon() , (p.second).get_lat() , (p.second).get_lon());

        heap[i] = (struct heap_node*)malloc(sizeof(struct heap_node));
        heap[i]->dist = d;
        heap[i]->id = p.first;
        heap[i]->index = i;

        i++;
        //cout << d << endl;
    }

    int SIZE = i;
    build_Heap(heap,SIZE);         // build the heap now, in O(n) time

    for(i=0;i<SIZE;i++) heap[i]->index = i;    // set the value of the index parameter


    struct heap_node ** heap_2 = (struct heap_node **)malloc(n_nodes*sizeof(struct heap_node*)); // second heap for implementing the algorithm

    int k;
    cout << "Enter the number of closest nodes you want : " ;

    while(1){
        cin >> k;
        if(k<0 || k>SIZE){
            cout << "\nPlease enter a valid number!  " ;        // take input of k from the user
            continue;
        }
        break;
    }

    int SIZE_2 = 0;
    vector<heap_node> v;
    struct heap_node * temp = heap[0];
    insert_min(heap_2 , &SIZE_2 , temp);

    for(i=0;i<k;i++){                          // take elements from the first heap and put in the second one, starting at the smallest element
        temp = find_min(heap_2 , SIZE_2);      // then each time, pop the top element (the smallest element) of the second heap
        v.push_back(*temp);                    // and push in a stack or vector
        delete_min(heap_2 , &SIZE_2);

        int idx = temp->index;               //and note it's index in the first heap
        int l = 2*idx + 1;           // the only candidates for the next minimum are it's children
        int r = 2*idx + 2;

        if(l<SIZE) insert_min(heap_2 , &SIZE_2 , heap[l]);         // so insert both children of that node in the second heap
        if(r<SIZE) insert_min(heap_2 , &SIZE_2 , heap[r]);
    }

    // this way, the second heap never has more than k elements and we are deleting and inserting O(k) times, so O(k log k) complexity added to O(n) of build heap and scanning
    // not that we never perform any operation other than build heap on the first heap, which preserves complexity

    cout << "\nThe closest " << k << " nodes to the requested node in sorted order are as follows : \n\n" ;
    cout << "Distance(in km)   Node ID\n" ;
    for(i=0;i<k;i++){
        cout << v[i].dist << "         " << v[i].id << endl;
    }

return 0;
}

/*
The algorithm runs in O(n + k log k ) time which is the fastest possible way of getting the k smallest elements in an unsorted array in sorted order
Alternatively, it can be implemented using the median of medians algorithm which finds the k smallest in O(n) time and then we can sort in O(k log k ) time
However, this algorithm is considerably simpler to understand and implement.
*/

