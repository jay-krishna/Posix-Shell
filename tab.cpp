#include <iostream>
#include <vector>
#include <cstring>
#include <dirent.h>

using namespace std;

#define lx 95
#define sx 32

struct Node{
	int is_end;
	struct Node* child[lx];
};

struct Node* GetLoc(string key,struct Node* root){
	cout<<"Loc";
	struct Node* temp=root;
	while (key.size()!=0 && temp!=NULL){
		temp=temp->child[key[0]-sx];
		key.erase(0,1);
	}
	return temp;
}

void print_v(std::vector<string> v){
	for(int i=0;i<v.size();++i){
		cout<<v[i]<<endl;
	}
	// cout<<endl;
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
    // ++k;
} 

void FindWords(struct Node* root, char str[], int level,vector<string> &v) 
{ 
    if (root->is_end==1)  
    { 
    	str[level]='\0';
    	cout<<"Loc";
        // cout<<str<<endl;
        v.push_back(string(str)); 
    }
    for (int i = 0; i < lx; i++)  
    { 
        if(root->child[i]!=NULL)  
        { 
            str[level]=i+' ';
            FindWords(root->child[i],str,level + 1,v); 
        } 
    } 
}

void FindWordsDriver(struct Node* root,string key) 
{ 
	cout<<"Loc";
	char str[100];
	for(int i=0;i<key.size();++i){
		str[i]=key[i];
	}
	std::vector<string> v;
	cout<<"OUT";
	auto x=GetLoc(key,root);
	// if(x!=NULL)
    // FindWords(x,str,key.size(),v);
	cout<<"OUT";
    print_v(v);
}

int main(){
	cout<<"Loc";
	struct Node* root=new Node;
	for(int i=0;i<lx;++i){
		root->child[i]=NULL;
		root->is_end=0;
	}

	char str[100];
	string token("/usr/bin");

	dirent *pdir;
	DIR *dir = opendir(token.c_str());

	if (dir == NULL) {
		
	}
	cout<<"Loc";
	while ((pdir = readdir(dir))){
		if(strcmp(pdir->d_name,".")==0||strcmp(pdir->d_name,"..")==0)
			continue;
		string real=pdir->d_name;
		AddNode(root, real);
	}
	closedir(dir);
	// auto x=GetLoc("t",root);
	string key("xxxxxxx");
	cout<<"OUT";
	// vector<char>str;
	// str.push_back('t');
	FindWordsDriver(root,key); 
	// cout<<v.size();
	return 0;
}