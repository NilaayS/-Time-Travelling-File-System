#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include "file_structures.cpp"

using namespace std;

filecont cont;
recent_files recent;
biggest_trees biggesttrees;
//assumption : no two files with the same name should exist; enforced at the time of creation
void process_text(string inp){
    //to process file based inputs , system based inputs
    // splitting the input into words using sstream
    stringstream ss(inp);
    string word;
    vector<string> words;
    while (ss >> word) {
        words.push_back(word);
    }
    if (words.size()>=2)
    {
        if (words.size()==2)
        {
            if (words[0] == "CREATE")
            {   
                file* tempr = cont.retrieve(words[1]);
                if(tempr != nullptr){
                    cout<<"File with this name already exists"<<endl;
                }else{
                    file* temp =new file(words[1]);
                    cont.push(temp);
                    recent.push(temp);
                    biggesttrees.push(temp);
                    cout<<"File created succesfully"<<endl;
                }
            }else if (words[0]=="READ")
            {

                file* temp = cont.retrieve(words[1]);
                if(temp!= nullptr){
                    //file with that name exists
                    temp->file_read();
                    cout<<endl;
                }else{
                    cout<<"No file with the given name found."<<endl;
                }
            }else if (words[0] == "ROLLBACK")
            {
                file* temp = cont.retrieve(words[1]);
                temp->file_rollback(-1);
            }else if (words[0]=="HISTORY")
            {
                file* temp = cont.retrieve(words[1]);
                if(temp!= nullptr){
                    //file with that name exists
                    temp->file_history();
                }else{
                    cout<<"No file with the given name found"<<endl;
                }
            }else if (words[0]=="RECENT_FILES")
            {
                //checking if the second part is an int(at max 32 itrs)
                int valid = 1;
                for(char i : words[1]){
                    int ascii = i;
                    if(ascii< 48 or ascii>57 ){
                        valid = 0;
                        break;
                    }
                }  
                if(valid){
                    recent.recent(stoi(words[1]));
                }else{
                    cout<<"INVALID INPUT"<<endl;
                }
            }else if(words[0]=="BIGGEST_TREES"){
                //checking if the second part is an int(at max 32 itrs)
                int valid = 1;
                for(char i : words[1]){
                    int ascii = i;
                    if(ascii< 48 or ascii>57 ){
                        valid = 0;
                        break;
                    }
                }  
                if(valid){
                    biggesttrees.versions(stoi(words[1]));
                }else{
                    cout<<"INVALID INPUT"<<endl;
                }
            }else{
                cout<<"INVALID INPUT"<<endl;
            }
            
        //-- end of all 2 word commands; no other valid input can have only 2 words;   
        }else{
            //when there are 3 words
            if (words.size()==3 and words[0]=="ROLLBACK")
            {
                //search for the second word if it exists in files;
                file* temp = cont.retrieve(words[1]);
                if(temp!= nullptr){
                    //file with that name exists : rollback :
                    //check if 3rd word is a valid int;
                    int valid = 1;
                    for(char i : words[2]){
                        int ascii = i;
                        if(ascii< 48 or ascii>57 ){
                            valid = 0;
                            break;
                        }
                    }  
                    if(valid){
                        
                        temp->file_rollback(stoi(words[2]));
                    }else{
                        cout<<"INVALID INPUT"<<endl;
                    }
                }else{
                    cout<<"No file with the given name found"<<endl;
                }
            }else{
                // Insert, update or snapshot : multi worded input
                string toent = "";
                for(int i = 2; i<words.size();i++){
                    toent+=words[i]+ " ";
                }
                if(words[0]=="UPDATE"){
                    file* temp = cont.retrieve(words[1]);
                    if(temp!= nullptr){
                        //file with that name exists : update
                        temp->file_update(toent);
                        cout<<"File updated successfully"<<endl;
                    }else{
                        cout<<"No file with the given name found"<<endl;
                    }
                }else if (words[0] == "INSERT" )
                {
                    //search for the second word if it exists in files;
                    file* temp = cont.retrieve(words[1]);
                    if(temp!= nullptr){
                        //file with that name exists : insert
                        temp->file_insert(toent);
                        cout<<"Inserted into the file successfully"<<endl;
                    }else{
                        cout<<"No file with the given name found"<<endl;
                    }
                }else if (words[0] == "SNAPSHOT")
                {
                    file* temp = cont.retrieve(words[1]);
                    if(temp!= nullptr){
                        //file with that name exists : snapshot
                        temp->file_snapshot(toent);
                    }else{
                        cout<<"No file with the given name found"<<endl;
                    }
                }else{
                    cout<<"INVALID INPUT"<<endl;
                }
            }
        }
    }else{
        cout<<"INVALID INPUT"<<endl;
    }
    
    
}

int main()
{
    //setup : a command prompt that keeps running until stopped
    cout<<"To stop/end the programme, enter STOP"<<endl;

    //string status = "ACTIVE";
    while (1)
    {
        //take input
        string temp = "";
        getline(cin, temp);
        //process and give output if necessary;
        if (temp == "STOP")
        {
            //status = "STOPPED";
            break;
        }
        process_text(temp);
    }
    
    return 0;
}