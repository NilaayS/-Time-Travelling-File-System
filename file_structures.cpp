#include<iostream>
#include<vector>
#include<string>
#include <ctime>
#include <list>

using namespace std;

//Node : Stores and manages each version
class TreeNode{
    private:
        int version_id;
        string content;
        string message; // Empty if not a snapshot
        time_t created_timestamp;
        time_t snapshot_timestamp; // Null if not a snapshot
        TreeNode* parent;
        vector<TreeNode*> children;
 
    public:
        //constructor for creating basic versions like roots
        TreeNode(int id){
            created_timestamp = time(0);
            content = "";
            if(id == 0){
                snapshot_timestamp = time(0);
                message = "root";
                parent = nullptr;
            }else{
                snapshot_timestamp = 0;
            }
            version_id = id;
        }
        //constructor for creating new versions corresponding to snapshotted versions
        TreeNode(TreeNode* prev, int id){
            created_timestamp = time(0);
            content = prev->content;
            parent = prev;
            version_id = id;
            snapshot_timestamp = 0;
            message = "";
        }
        //snapshots
        void snapshot(string msg){
            snapshot_timestamp = time(0);
            message = msg;
        }
        //checks if the fxn is snapshotted or not
        bool is_snapshot(){
            if (snapshot_timestamp !=0){
                return true;
            }else{
                return false;
            }
            
        }
        //returns snapshotted time : assuming only called if it is snapshotted
        time_t get_snapshot_time(){
            return snapshot_timestamp;
        }
        //content : returns the content
        string get_content(){
            return content;
        }
        //content : adds or inserts to the content
        void insert_content(string s){
            content+=s;
        }
        //content : updates or replaces the content
        void update_content(string s){
            content=s;
        }       
        //message : returns the message
        string get_message(){
            return message;
        } 
        //gets parent
        TreeNode* get_parent(){
            return parent;
        }
        //gets version
        int get_version(){
            return version_id;
        }
        //adds children
        void push_child(TreeNode* s){
            children.push_back(s);
        }


};

//basic implementation of the rollback map : we know that versions will be added sequentially : mapping is version_id -> index;
class mapp{
    private:
        vector<TreeNode*> data;
    public:
        //inserts an element : it'll always be sequential 
        void push(TreeNode*& hi ){
            data.push_back(hi);
        }
        // accesses an element and returns its address
        TreeNode* access(int veridd){
            return data[veridd];
        }
};


//file : essentially manages all versions of a file
class file{
    private:
        string name;
        TreeNode* root; // Your implementation of the tree
        TreeNode* active_version;
        mapp version_map;
        int total_versions;
        time_t most_recent_update ;
    public:
        //constructor
        file(string n){
            root = new TreeNode(0);
            active_version = root;
            version_map.push(active_version) ;
            total_versions = 0;
            most_recent_update = time(0);
            name = n;
        }
        //Returns the name
        string get_name(){
            return name;
        }
        //Returns the last updated time
        time_t get_most_recent_update(){
            return most_recent_update;
        }
        //Returns the total versions
        int get_versions(){
            return total_versions;
        }
        //Read
        void file_read(){
            cout<<(active_version->get_content());
        }
        //Insert
        void file_insert(string s){
            if (active_version->is_snapshot())
            {//if already a snapshot
                TreeNode* prevact = active_version;
                active_version = new TreeNode(prevact,++total_versions);
                most_recent_update = time(0);
                active_version->insert_content(s);
                prevact->push_child(active_version);
                version_map.push(active_version);
            }else{
                active_version->insert_content(s);
                most_recent_update = time(0);
            }
            
        }
        //Update
        void file_update(string s){
            if (active_version->is_snapshot())
            {//if already a snapshot
                TreeNode* prevact = active_version;
                active_version = new TreeNode(prevact,++total_versions);
                most_recent_update = time(0);
                active_version->update_content(s);
                prevact->push_child(active_version);
                version_map.push(active_version);
            }else{
                active_version->update_content(s);
                most_recent_update = time(0);
            }
        }
        //Snapshot
        void file_snapshot(string msg){
            
            if(active_version->is_snapshot()){
                cout<<"THE VERSION IS ALREADY A SNAPSHOT"<<endl;
            }else{
                active_version->snapshot(msg);
                most_recent_update = time(0);
                cout<<"File snapshotted successfully"<<endl;
            }

        }
        //Rollback
        void file_rollback(int ver_id = -1){
            if (ver_id == -1)
            {   
               if (active_version->get_parent()==nullptr)
               {
                //if root:
                cout<<"ALREADY AT THE ROOT"<<endl;
               }else{
                 active_version = active_version->get_parent();
               }
               cout<<"Rolled Back Successfully"<<endl;
            }else if(ver_id>=0 and ver_id<= total_versions){
                active_version = version_map.access(ver_id);
                cout<<"Rolled Back Successfully"<<endl;
            }else{
                cout<<"VERSION DOES NOT EXIST"<<endl;
            }
            
            
            
        }
        //History
        void file_history(){
            //lists all snapshotted versions which lie on the path from active to root... no need to offset
            //assuming we need to return snapshoted time
            TreeNode* temp;
            if ( active_version->is_snapshot())
            {
                temp = active_version;
            }else{
                temp = active_version->get_parent();
            }
            
            while (temp != nullptr)
            {
                char buffer[80];
                time_t val = temp->get_snapshot_time();
                strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", localtime(&(val)));
                cout<< temp->get_version()<<", " << buffer<<", "<<temp->get_message()<<endl;
                temp = temp->get_parent();
            }
        }
        
};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
// System Wide - Metrics
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------


//Manages the recent files operation
class recent_files{
    private:
        vector<file*> data;
    public:
        //adds an element to data
        void push(file* &obj){
            data.push_back(obj);
        }
        //builds a heap using heapify
        void build(){
            if(data.size()!=0){
                for (int j = data.size()-1; j >= 0; j--)
            {
                int i = j;
                //heapify down
                while (2*i+1<=data.size()-1)
                {
                    int max = 2*i+1;
                    if(2*i+2<= data.size()-1 and data[2*i+2]->get_most_recent_update()>data[max]->get_most_recent_update()){
                        max = 2*i+2;
                    }
                    if (data[i]->get_most_recent_update()<data[max]->get_most_recent_update())
                    {
                        file* temp = data[i];
                        data[i]=data[max];
                        data[max] = temp;  
                        i = max;
                    }else{
                        break;
                    }
                }   
             }
            }
        }
        //removes an element from the heap
        void pop(){
            if(data.size()!=0 and data.size()!=1){
                data[0] = data[data.size()-1];
                data.pop_back();
                int i = 0;
                while (2*i+1<=data.size()-1)
                {
                    int max = 2*i+1;
                    if(2*i+2<= data.size()-1 and data[2*i+2]->get_most_recent_update()>data[max]->get_most_recent_update()){
                        max = 2*i+2;
                    }
                    if (data[i]->get_most_recent_update()<data[max]->get_most_recent_update())
                    {
                        file* temp = data[i];
                        data[i]=data[max];
                        data[max] = temp;  
                        i = max;
                    }else{
                        break;
                    }
                    
                }
            }else if (data.size()== 1)
            {
                data.pop_back();
            }
            
        }
        //returns the top element of the heap if not empty else returns nullptr
        file* top(){
            if (data.size()!= 0){
                return data[0];
            }else{
                return nullptr;
            }
 
            
        }
        //checks if the heap is empty or not
        bool empty(){
            if(data.size()==0){
                return true;
            }else{
                return false;
            }
        }
       
        //prints k recently edited files by the time they were last edited
        void recent(int k){
            //first builds a heap of data and then returns k
            build();
            vector<file*> temp;
            //if k> no of files which exists, I'm printing all the existing files.
            cout<<"Filename : last updated"<<endl;
            if(k <= data.size()){
                
                for (int i = 0; i < k; i++)
                {
                    time_t update_time = top()->get_most_recent_update();
                    temp.push_back(top());
                    cout<<top()->get_name()<<" : "<<ctime(&update_time);
                    pop();
                }
                for(file* i : temp){
                    push(i);
                }
            }else{
                cout<<k<<" is larger than the number of files"<<endl;
                int m = data.size();
                for (int i = 0; i < m; i++)
                {
                    time_t update_time = top()->get_most_recent_update();
                    temp.push_back(top());
                    cout<<top()->get_name()<<" : "<<ctime(&update_time);
                    pop();
                }
                for(file* i : temp){
                    push(i);
                }
            }
        }

};



//Manages the biggest trees operation
class biggest_trees{
    private:
        vector<file*> data;
    public:
        //adds an element to data
        void push(file* &obj){
            data.push_back(obj);
        }
        //builds a heap using heapify
        void build(){
            if(data.size()!=0){
                for (int j = data.size()-1; j >= 0; j--)
            {
                int i = j;
                //heapify down
                while (2*i+1<=data.size()-1)
                {
                    int max = 2*i+1;
                    if(2*i+2<= data.size()-1 and data[2*i+2]->get_versions()>data[max]->get_versions()){
                        max = 2*i+2;
                    }
                    if (data[i]->get_versions()<data[max]->get_versions())
                    {
                        file* temp = data[i];
                        data[i]=data[max];
                        data[max] = temp;  
                        i = max;
                    }else{
                        break;
                    }
                }   
             }
            }
        }
        //removes an element from the heap
        void pop(){
           if(data.size()!=0 and data.size()!= 1){
                data[0] = data[data.size()-1];
                data.pop_back();
                int i = 0;
                while (2*i+1<=data.size()-1)
                {
                    int max = 2*i+1;
                    if(2*i+2<= data.size()-1 and data[2*i+2]->get_versions()>data[max]->get_versions()){
                        max = 2*i+2;
                    }
                    if (data[i]->get_versions()<data[max]->get_versions())
                    {
                        file* temp = data[i];
                        data[i]=data[max];
                        data[max] = temp;  
                        i = max;
                    }else{
                        break;
                    }
                    
                }
            }else if (data.size()== 1)
            {
                data.pop_back();
            }
            
        }
        //returns the top element of the heap
        file* top(){
            if (data.size()!= 0){
                return data[0];
            }else{
                return nullptr;
            }
 
            
        }
        //checks if the heap is empty or not
        bool empty(){
            if(data.size()==0){
                return true;
            }else{
                return false;
            }
        }
        //prints k files with the largest versions
        void versions(int k){
            //builds the heap first, and then..
            build();
            vector<file*> temp;
            //if k> no of files which exists, I'm printing all the existing files.
            cout<<"Filename : versions"<<endl;
            if(k <= data.size()){
                for (int i = 0; i < k; i++)
                {
                    temp.push_back(top());
                    cout<<top()->get_name()<<": "<<top()->get_versions()<<endl;
                    pop();
                }
                for(file* i : temp){
                    push(i);
                }
            }else{
                cout<<k<<" is larger than the total number of files"<<endl;
                int size =data.size();
                for (int i = 0; i < size; i++)
                {
                    temp.push_back(top());
                    cout<<top()->get_name()<<": "<<top()->get_versions()<<endl;
                    pop();
                }
                for(file* i : temp){
                    push(i);
                }
            }
        }
};

//-----------------------------------------
//-----------------------------------------
// An hashmap to store and retrieve files
//--------------
//check if all other pass a file/treenode obj by reference
class filecont{
    private:
        //data is stored as an array of linked lists.
        list<file*>* data = new list<file*>[2];
        int size;
        int capacity;
    public:
        //constructor
        filecont(){
            size = 0;
            capacity = 2;
        }
        //converts name to an ascii based int
        int pre_encode(string sss){
            //string temp = "";   
            int to_hash = 0;
            int i = 1 ;       
            for(char s : sss){
                int b = s;
                //temp+= to_string(b);
                to_hash+= 23*b*b*(i);
                to_hash=to_hash%(1000000000000000000);
                i++;
            }
            return to_hash;
        }
        //hash function
        int hash(int tohash){
            return tohash%capacity;
        }
        
        //given a file inserts it into the hashmap
        //file -> filename -> int(constant-bit encoding) : will be fairly random -> %capacity of data container -> store as a vector/linked list
        //maintaining the size : 0.5 = size/capacity :
        void push(file*& filewa){
            if(size+1>(capacity/2)){
                //if it doesnt maintain 0.5 ratio: double , rehash and insert each individual element (n time)
                int oldcapacity = capacity;
                capacity = capacity*2;
                list<file*>* temp = new list<file*>[capacity];
                for(int i = 0; i<oldcapacity;i++){
                    while (!data[i].empty())
                    {
                        file* tempfile = data[i].front();
                        data[i].pop_front();
                        int location = hash(pre_encode(tempfile->get_name()));
                        temp[location].push_back(tempfile);
                    }
                }
                delete[] data;
                data = temp;
                
            }
            int location = hash(pre_encode(filewa->get_name()));
            data[location].push_back(filewa);
            size++;
        }

        //given a file searches and retrieves it from the hashmap; returns nullptr if not found;
        file* retrieve(string name){
            int location = hash(pre_encode(name));
            
            file* found = nullptr;
            for(file* i : data[location]){
                if(i->get_name() == name){
                    found =i;
                    break;
                }
            }
            return found;          
        }
};