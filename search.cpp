/*
    2019201019
    Trie for tab & history
*/
#include "search.h"

struct Node{
	int is_end;
	struct Node* child[lx];
};

struct Node* GetLoc(string key,struct Node* root){
	struct Node* temp=root;
	while (key.size()!=0 && temp!=NULL){
		temp=temp->child[key[0]-sx];
		key.erase(0,1);
	}
	return temp;
}

string print_v(std::vector<string> v){
    string data="\n\n";
    for(int i=0;i<(int)v.size();++i){
        data+=v[i]+"\n";
        // if(i%5==0&&i!=0)
        //     data+="\n";
    }
    // cout<<endl;
    data+="\n";
    return data;
}

void AddNode(struct Node *root, string key) 
{
    struct Node *temp = root;
    while (key.size()!=0) 
    {
        if (!temp->child[key[0]-sx]){
        	struct Node* temp2=new Node;
			for(int i=0;i<lx;++i){
				temp2->child[i]=NULL;
				temp2->is_end=0;
			}
            temp->child[key[0]-sx]=temp2; 
        }
        temp = temp->child[key[0]-sx];
        key.erase(0,1);
    }
    temp->is_end = 1;
} 

void FindWords(struct Node* root, char str[],int level,vector<string>&optionlist) 
{ 
    if (root->is_end==1)  
    { 
    	str[level]='\0';
        optionlist.push_back(string(str)); 
    }

    for (int i=0;i<lx;i++)  
    { 
        if(root->child[i]!=NULL)  
        { 
            str[level]=i+' ';
            FindWords(root->child[i],str,level + 1,optionlist); 
        } 
    } 
}

vector<string> FindWordsDriver(struct Node* root,string key) 
{ 
	char str[100];
	for(int i=0;i<(int)key.size();++i){
		str[i]=key[i];
	}
    vector<string> optionlist;
    auto it=GetLoc(key,root);
    if(it!=NULL)
        FindWords(it,str,key.size(),optionlist);
    return(optionlist);
}

string TabDisplay(char buffer[],unordered_map <string,string> environment_var,vector <string>executable_var2){
    string key(buffer);
    // string temp=environment_var.find("PATH")->second;
    // const char *delim=":";
    struct Node* root=new Node;
    for(int i=0;i<lx;++i){
        root->child[i]=NULL;
        root->is_end=0;
    }
    // char str[100];

    // char *token = strtok(const_cast<char*>(temp.c_str()), delim);
    for(int i=0;i<(int)executable_var2.size();++i)
    {
        AddNode(root,executable_var2[i]);
    }

    string data="";
    auto optionlist=FindWordsDriver(root,key);
    if(optionlist.size()>0)
        data=print_v(optionlist);
    if(optionlist.size()==0)
        data="\n\n";

    return data;

}

string HistoryDisplay(char buffer[],unordered_map <string,string> environment_var){
    string key(buffer);
    string filename2=environment_var.find("HOME")->second+"/"+"my_history.txt";
    // string filename2="/home/singular/my_history.txt";
    // cout<<"Inside"<<endl;
    struct Node* root=new Node;
    for(int i=0;i<lx;++i){
        root->child[i]=NULL;
        root->is_end=0;
    }
    
    string line;
    ifstream infile1(filename2);
    while (getline(infile1,line)){
        // write(STDOUT_FILENO,line.c_str(),(size_t)line.size());
        AddNode(root,line);
    }

    string data="";
    auto optionlist=FindWordsDriver(root,key);
    if(optionlist.size()>0)
        data=print_v(optionlist);
    if(optionlist.size()==0)
        data="\n\n";

    return data;

}