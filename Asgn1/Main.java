package com.company;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Scanner;

// Hashmap is used here as it is kind of an inbuilt combination of Hashset and Tuple2, so it is easy to work with it
class Var{        // a class to store all the hashmaps and hashsets, so we can access from all static methods in Main (mimicking global variables, in a sense)
    public static HashMap<Integer,manufacturer> h_manufacturer = new HashMap<>();
    public static HashMap<Integer,product> h_product = new HashMap<>();
    public static HashMap<Integer,customer> h_customer = new HashMap<>();       //  all hashmaps map the ID of a fixed kind of entity to
    public static HashMap<Integer,shop> h_shop = new HashMap<>();               //  an object of that type
    public static HashMap<Integer,agent> h_agent = new HashMap<>();             //  the IDs help in searching for an object
    public static HashSet<order> h_order = new HashSet<>();        // set of all PENDING orders, when you PROCESS orders, some of these will be finished
    public static HashSet<Integer> all_ids = new HashSet<>();      // set of ALL ID_S, i.e. of ALL entities
    public static final int INF = Integer.MAX_VALUE;   // a constant INFINITY to initialise "min" variables
}

// NOTE : Each integer scanned from the user is in the form of a try-catch loop which executes until the user enters a valid input
public class Main {

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        // The menu shows all types and their code (1,2,3,4,5) and all the functions (1,2,3,4,5,6,7,8,9,10)
        // Read the menu carefully to navigate through functions
        System.out.println("Type 1 : Manufacturer\nType 2 : Product\nType 3 : Customer\nType 4 : Shop/Warehouse\nType 5 : Delivery Agent");
        System.out.printf("Menu : \nEnter 1 to CREATE an ENTITY \nEnter 2 to DELETE an ENTITY \nEnter 3 to PRINT ALL ENTITIES of a type\nEnter 4 to ADD a PRODUCT to a MANUFACTURER\nEnter 5 to ADD number of COPIES of a PRODUCT to a SHOP\nEnter 6 to PLACE an ORDER\nEnter 7 to PROCESS the LIST of PENDING ORDERS\nEnter 8 to LIST all PURCHASES made by a CUSTOMER\nEnter 9 to LIST the INVENTORY of a SHOP\nEnter 10 to LIST the PRODUCTS made by a MANUFACTURER\nEnter 0 to terminate process\n\n%n");

        System.out.println("Enter new function to execute : ");
        int f;
        //int f = input.nextInt();
        while(true){
            String in = input.nextLine();
            try {f = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }

        // read the function comments for complete explanation
        while(f!=0){
            switch (f) {
                case 1 -> create(input);
                case 2 -> delete(input);
                case 3 -> print(input,-1);
                case 4 -> add_product_to_manufacturer(input);
                case 5 -> add_copies_to_shop(input);
                case 6 -> place_order(input);
                case 7 -> process_order();
                case 8 -> purchase_list(input);
                case 9 -> inventory_list(input);
                case 10 -> manufactured_products(input);
                default -> System.out.println("Please enter a valid entry!");
            }
            System.out.println("Enter new function to execute : ");
            while(true){
                String in = input.nextLine();
                try {f = Integer.parseInt(in);break;}
                catch(Exception e) {
                    System.out.println("Please enter integer!");
                }
            }
        }
    }

    public static void create(Scanner input){                           // Create entities of different types
        System.out.println("Enter the type of entity to create : ");
        int type;
        while(true){
            String in = input.nextLine();
            try {type = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
        switch(type){
            case 1 :    manufacturer m = new manufacturer(input);         // simply create a new object and put in the respective containers
                Var.h_manufacturer.put(m.id , m); return;
            case 2 :    product p = new product(input);
                Var.h_product.put(p.id , p); return;
            case 3 :    customer c = new customer(input);
                Var.h_customer.put(c.id , c); return;
            case 4 :    shop s = new shop(input);
                Var.h_shop.put(s.id , s); return;
            case 5 :    agent a = new agent(input);
                Var.h_agent.put(a.id , a); return;
            case 6 :    System.out.println("Please enter a valid type!");
        }
    }

    public static void print(Scanner input,int type){                 // Print ALL entities of a given type
        if(type == -1) {
            System.out.println("Enter type of entities to print : ");
            while(true){
                String in = input.nextLine();
                try {type = Integer.parseInt(in);break;}
                catch(Exception e) {
                    System.out.println("Please enter integer!");
                }
            }
        }
        switch(type){                                                 // simply iterate over the containers and print everything
            case 1 : for(int v : Var.h_manufacturer.keySet()) Var.h_manufacturer.get(v).print(); return;
            case 2 : for(int v : Var.h_product.keySet()) Var.h_product.get(v).print(); return;
            case 3 : for(int v : Var.h_customer.keySet()) Var.h_customer.get(v).print(); return;
            case 4 : for(int v : Var.h_shop.keySet()) Var.h_shop.get(v).print(); return;
            case 5 : for(int v : Var.h_agent.keySet()) Var.h_agent.get(v).print(); return;
            case 6 : System.out.println("Please enter a valid type!");
        }
    }

    public static void delete(Scanner input){       // function to delete an entity
        System.out.println("Enter the type of entity to delete : ");
        int type;
        while(true){
            String in = input.nextLine();
            try {type = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
        print(input , type);
        System.out.println("Enter the id of the one you wish to delete from the above options : ");
        int new_id;
        while(true){
            String in = input.nextLine();
            try {new_id = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
        switch(type){
            case 1 : del_manufacturer(new_id); return;
            case 2 : del_product(new_id); return;
            case 3 : del_customer(new_id); return;              // these functions do what is needed before the entities can be erased forever
            case 4 : del_shop(new_id); return;                  // like checking if such an entity exists at all
            case 5 : del_agent(new_id); return;                 // and making their UNIQUE ID available for use again by new entities
            default : System.out.println("Please enter a valid type!");
        }
    }

    public static void del_manufacturer(int ID){
        if(!Var.h_manufacturer.containsKey(ID)){    // first check if the requested object exists in the first place
            System.out.println("No such manufacturer found!");
            return;
        }
        Var.all_ids.remove(ID);                     // remove from the respective containers and also from all_ids list, so that it is available for reuse
        Var.h_manufacturer.remove(ID);
    }

    public static void del_product(int ID){
        if(!Var.h_product.containsKey(ID)){
            System.out.println("No such product found!");
            return;
        }
        Var.all_ids.remove(ID);
        del_order(ID);                                 // when a product is deleted, all orders involving that product ARE ALSO DELETED
        product p = Var.h_product.get(ID);
        if(Var.h_manufacturer.containsKey(p.maker)){
            manufacturer m = Var.h_manufacturer.get(p.maker);
            if(m.product_list.contains(ID)) m.product_list.remove(ID);         // remove the product from the manufacturer's list
        }
        for(int key : Var.h_shop.keySet()){
            shop s = Var.h_shop.get(key);
            if(s.inventory.containsKey(ID)) s.inventory.remove(ID);            // remove this product from ALL the shops, if present
        }
        Var.h_product.remove(ID);
    }

    public static void del_customer(int ID){
        if(!Var.h_customer.containsKey(ID)){
            System.out.println("No such customer found!");
            return;
        }
        Var.all_ids.remove(ID);
        del_order(ID);                                 // when a customer is deleted, all order requests made by that customer ARE ALSO DELETED
        Var.h_customer.remove(ID);
    }

    public static void del_shop(int ID){
        if(!Var.h_shop.containsKey(ID)){
            System.out.println("No such shop found!");   // similar to above delete functions
            return;
        }
        Var.all_ids.remove(ID);
        Var.h_shop.remove(ID);
    }

    public static void del_agent(int ID){
        if(!Var.h_agent.containsKey(ID)){
            System.out.println("No such agent found!");
            return;
        }
        Var.all_ids.remove(ID);
        Var.h_agent.remove(ID);
    }

    public static void del_order(int ID){       // this function deletes those orders whose customers or products are deleted
        HashSet<order> delete_these_orders = new HashSet<>();
        for(order o : Var.h_order){
            if(o.product_id == ID){
                System.out.println("Along with Product ID " + ID + ", the following orders placed by the customer have also been deleted : ");
                o.print();
                delete_these_orders.add(o);
            }
            if(o.customer_id == ID){
                System.out.println("Along with Customer ID " + ID + ", the following orders placed by the customer have also been deleted : ");
                o.print();
                delete_these_orders.add(o);
            }
        }
        for(order o : delete_these_orders){        // we need to do this as we cannot iterate over and edit a container at the same time
            Var.h_order.remove(o);
        }
        delete_these_orders.clear();
    }

    public static void add_product_to_manufacturer(Scanner input){     // Links a manufacturer to a product
        System.out.println("Enter manufacturer ID : ");
        int m_id;
        while(true){
            String in = input.nextLine();
            try {m_id = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }                                                           // NOTE : Each product has only one manufacturer, but a manufacturer
        }                                                               //        can make multiple products. If the function is called such that
        if(!Var.h_manufacturer.containsKey(m_id)){                     //        the product entered ALREADY HAS a manufacturer, then p.maker will be
            System.out.println("No such manufacturer found!");         //        OVER-WRITTEN. However, the product still remains in the old manufacturer's
        }                                                              //        m.product_list
        else{
            manufacturer m = Var.h_manufacturer.get(m_id);
            System.out.println("Enter the product ID : ");
            int p_id;
            while(true){
                String in = input.nextLine();
                try {p_id = Integer.parseInt(in);break;}
                catch(Exception e) {
                    System.out.println("Please enter integer!");
                }
            }

            if(!Var.h_product.containsKey(p_id)) System.out.println("No such product found!");
            else m.add_product(p_id);
        }
    }

    public static void add_copies_to_shop(Scanner input){       // Adds a number of copies of a product to a shop
        System.out.println("Enter shop ID : ");
        int s_id ;
        while(true){
            String in = input.nextLine();
            try {s_id = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }                                                           // NOTE : Each product has only one manufacturer, but a manufacturer
        }
        if(!Var.h_shop.containsKey(s_id)){
            System.out.println("No such shop found!");          // checks are added everywhere to ensure existence of entity before attempting to access it
        }
        else{
            shop s = Var.h_shop.get(s_id);
            System.out.println("Enter product ID : ");
            int p_id;
            while(true){
                String in = input.nextLine();
                try {p_id = Integer.parseInt(in);break;}
                catch(Exception e) {
                    System.out.println("Please enter integer!");
                }
            }
            if(!Var.h_product.containsKey(p_id)){
                System.out.println("No such product found!");
            }
            else{
                System.out.println("Enter the number of copies : ");
                int copies;
                while(true){
                    String in = input.nextLine();
                    try {copies = Integer.parseInt(in);break;}
                    catch(Exception e) {
                        System.out.println("Please enter integer!");
                    }
                }
                s.add_copies_to_shop(p_id,copies);             // this functions adds the required copies of product p_id to the shop object s
            }
        }
    }

    public static void place_order(Scanner input){           // This will add your order to the current list of pending orders
        order new_order = new order(input);                  // NOTE : This order is NOT YET processed, processing should be done explicitly
        Var.h_order.add(new_order);
    }

    public static void process_order(){                     // This will go through all pending orders and process those which can be done
        HashSet<order> delete_these_orders = new HashSet<>();
        for(order o : Var.h_order){                         // ANY order from ANY customer which CAN be processed, will be processed here
            customer c = null;
            if(Var.h_customer.containsKey(o.customer_id)) c = Var.h_customer.get(o.customer_id);
            if(c == null) continue;                         // if no such "suitable" customer or shop or agent exist, order will REMAIN PENDING
            agent a = find_agent(c);   // see these functions for criteria of "suitable"
            shop s = find_shop(o,c);
            if(a == null || s == null) continue;
            System.out.println("Successful order : ");
            System.out.println("Agent " + a.id + " delivered " + o.copies + " of product ID " + o.product_id + " from shop ID " + s.id + " to customer ID " + c.id);
            a.finish_order();
            c.finish_order(o);             // these functions update respective qualities of the objects participating in a successful transaction
            s.finish_order(o);             // see respective classes for details

            delete_these_orders.add(o);    // we need to store the orders to be deleted and then later delete them together to avoid ConcurrentModificationException
        }
        for(order o : delete_these_orders){
            Var.h_order.remove(o);
        }
        delete_these_orders.clear();
    }

    public static agent find_agent(customer c){         // returns agent with same zipcode as customer and minimum number of deliveries
        int min = Var.INF;
        agent answer = null;
        for(int key : Var.h_agent.keySet()){
            agent temp = Var.h_agent.get(key);
            if(temp.zipcode == c.zipcode && temp.delivered < min){          // checking is zipcode is same whilst also seeking the agent who has made
                answer = temp;                                              // the least number of deliveries
                min = temp.delivered;
            }
        }
        return answer;        // if nothing found, NULL is returned
    }

    public static shop find_shop(order o,customer c){      // returns shop with the same zipcode as customer and having at least as many as demanded copies of product
        for(int key : Var.h_shop.keySet()){
            shop temp = Var.h_shop.get(key);
            if(temp.zipcode != c.zipcode) continue;                 // check zipcode
            if(!temp.inventory.containsKey(o.product_id)) continue; // check if product is available
            if(temp.inventory.get(o.product_id) < o.copies) continue;  // check if at least the required number of copies can be found
            return temp;
        }
        return null;         // if nothing found, NULL is returned
    }

    public static void purchase_list(Scanner input){             // Prints all the products and number of copies bought by a customer
        System.out.println("Enter the customer ID : ");          // NOTE : This list only shows successful transactions, not pending orders
        int c_id;
        while(true){
            String in = input.nextLine();
            try {c_id = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
        if(!Var.h_customer.containsKey(c_id)){
            System.out.println("No such customer found!");
        }
        else{
            customer c = Var.h_customer.get(c_id);
            c.list_purchased();        // this function prints the purchased list of the customer object c
        }
    }

    public static void inventory_list(Scanner input){            // Prints all the items and number of available copies of a shop
        System.out.println("Enter the shop ID : ");
        int s_id;
        while(true){
            String in = input.nextLine();
            try {s_id = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
        if(!Var.h_shop.containsKey(s_id)){
            System.out.println("No such shop found!");
        }
        else{
            shop s = Var.h_shop.get(s_id);
            s.list_inventory();         // this function prints the inventory of the shop object s
        }
    }

    public static void manufactured_products(Scanner input){     // Prints all the product IDS manufactured by a manufacturer
        System.out.println("Enter the manufacturer ID : ");
        int m_id;
        while(true){
            String in = input.nextLine();
            try {m_id = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
        if(!Var.h_manufacturer.containsKey(m_id)){
            System.out.println("No such manufacturer found!");
        }
        else{
            manufacturer m = Var.h_manufacturer.get(m_id);
            m.list_products();        // this function prints all the products manufactured by the manufacturer object m
        }
    }

}

class entity{             // the parent class of all other entity types
    int id;
    String name;          // these are common for all types of entities

    public entity(Scanner input){              // constructor
        int new_id;
        System.out.println("Enter UNIQUE ID:");
        while(true){
            while(true){
                String in = input.nextLine();
                try {new_id = Integer.parseInt(in);break;}
                catch(Exception e) {
                    System.out.println("Please enter integer!");
                }
            }
            if(Var.all_ids.contains(new_id)){
                System.out.println("This ID already exists! Enter a new UNIQUE ID : ");
            }
            else{
                this.id = new_id;
                Var.all_ids.add(new_id);       // add the ids to all_ids list
                break;
            }
        }
        System.out.println("Enter name:");
        this.name = input.nextLine();
    }

    public void print(){                      // print function
        System.out.println("\n");
        System.out.println("ID : " + this.id);
        System.out.println("Name : " + this.name);
    }
}

class manufacturer extends entity{            // type 1
    HashSet<Integer> product_list;           // list of product IDs

    public manufacturer(Scanner input){      // constructor
        super(input);
        this.product_list = new HashSet<>(); // initialise container
    }

    public void print(){                     // print function
        super.print();
    }

    public void list_products(){             // list ALL the products manufactured
        System.out.println("List of IDs of products sold : ");
        for(int value : this.product_list){
            System.out.print(value + " ");
        }
    }

    public void add_product(int new_id){     // add new products
        this.product_list.add(new_id);       // add product to list
        product p = Var.h_product.get(new_id);
        p.maker = this.id;                   // update that product's maker
    }

}

class product extends entity{       // type 2
    int maker;                      // as per the question, each product has only one manufacturer so one variable is declared - ID of manufacturer

    public product(Scanner input){  // constructor
        super(input);
        this.maker = -1;      // Initialise this to an INVALID ID, as no manufacturer is assigned yet. This will be done in add_product_to_manufacturer
    }

    public void print(){             // print function
        super.print();
        if(this.maker!=-1) System.out.println("This product is manufactured by manufacturer ID : " + this.maker);
        else System.out.println("This product has not been assigned to a manufacturer yet");
    }
}

class customer extends entity{      // type 3
    int zipcode;
    HashMap<Integer , Integer> purchased;         // maps product ID to the number of copies bought

    public customer(Scanner input){               // constructor
        super(input);
        System.out.println("Enter customer zipcode:");
        while(true){
            String in = input.nextLine();
            try {this.zipcode = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
        this.purchased = new HashMap<>();
    }

    public void print(){                          // print function
        super.print();
        System.out.println("Zipcode of this customer is : " + this.zipcode);
    }

    public void list_purchased(){                 // print list of purchased items (successful transactions ONLY)
        System.out.println("List of products purchased : ");
        for(int key : this.purchased.keySet()){
            int value = this.purchased.get(key);
            System.out.println("Product ID : " + key + " bought " + value + " times ");
        }
    }

    public void finish_order(order o){        // does needful when transaction is completed
        int initial = 0;
        if(this.purchased.containsKey(o.product_id)){          // check if this product was bought earlier
            initial = this.purchased.get(o.product_id);
        }
        this.purchased.put(o.product_id , initial+o.copies);    // then add the number of copies bought
    }

}

class shop extends entity{     // type 4
    int zipcode;
    HashMap<Integer , Integer> inventory;     // maps product ID to number of copies available

    public shop(Scanner input){               // constructor
        super(input);
        System.out.println("Enter shop zipcode : ");
        while(true){
            String in = input.nextLine();
            try {this.zipcode = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
        this.inventory = new HashMap<>();     // scan necessary items and initialise container
    }

    public void print(){                      // print function
        super.print();
        System.out.println("Zipcode of this shop is : " + this.zipcode);
    }

    public void list_inventory(){             // list the inventory
        System.out.println("Current inventory : ");
        for(int key : this.inventory.keySet()){
            int value = this.inventory.get(key);
            System.out.println("Product ID : " + key + " X" + value);
        }
    }

    public void add_copies_to_shop(int new_id,int copies){     // add copies of a product
        int initial = 0;
        if(this.inventory.containsKey(new_id)){            // check if product already existed
            initial = this.inventory.get(new_id);
        }
        this.inventory.put(new_id , initial + copies);     // then add number of copies
    }

    public void finish_order(order o){        // does needful when transaction is completed
        int initial = this.inventory.get(o.product_id);
        initial -= o.copies;                                      // remove the number of sold copies
        if(initial == 0) this.inventory.remove(o.product_id);     // if no copies remain after transaction, remove the key itself
        else this.inventory.put(o.product_id , initial);          // else update the value
    }

}

class agent extends entity{             // type 5
    int zipcode;
    int delivered;

    public agent(Scanner input){        // constructor
        super(input);
        System.out.println("Enter agent zipcode : ");
        while(true){
            String in = input.nextLine();
            try {this.zipcode = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
        this.delivered = 0;             // initialise delivered items to 0
    }

    public void print(){                // print function
        super.print();
        System.out.println("Zipcode of this delivery agent is : " + this.zipcode);
        System.out.println("Number of products delivered : " + this.delivered);
    }

    public void finish_order(){        // does needful when transaction is completed, i.e. increments the number of deliveries made by 1
        this.delivered ++;
    }
}

class order{                  // an extra class is required to store the pending orders
    int customer_id;          // id of customer who placed the order
    int product_id;           // id of product required in the order
    int copies;               // number of copies demanded

    public order(Scanner input){       // constructor
        System.out.println("Enter the customer ID : ");
        while(true){
            String in = input.nextLine();
            try {this.customer_id = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
        System.out.println("Enter the product ID : ");          // take input of all three quantities
        while(true){
            String in = input.nextLine();
            try {this.product_id = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
        System.out.println("Enter the number of copies : ");
        while(true){
            String in = input.nextLine();
            try {this.copies = Integer.parseInt(in);break;}
            catch(Exception e) {
                System.out.println("Please enter integer!");
            }
        }
    }

    public void print(){               // print function - executed when the order sees completion, i.e. the transaction was successfully finished
        System.out.println("\n");
        System.out.println("Product : " + this.product_id + " , Copies : " + this.copies);
    }
}