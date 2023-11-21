#include<bits/stdc++.h>
using namespace std;

vector<pair<string, int>> mnt; //this table stores name of micro and its address in mdt
vector<string> mdt;
map<string, string> kpd;
fstream fin;

class pass1{
    public:

    static vector<string> split(string line){
        vector<string> words;
        string output = "";

        for(char ch: line){

            if(ch == ' '){
                if(output == " ") continue;
                words.push_back(output);
                output = "";
                continue;
            }
            if(ch == ',') continue;
            output += ch;
        }
        //for last word to push
        if(output != " " && output != ""){
            words.push_back(output);
        }
        return words;
    }
    
    static bool is_default_parameter(string parameter){

        int n = parameter.size();
        //iterate over string is = is found inbetween then its default parameter

        for(int i=0; i<n; i++){

            if(parameter[i] == '='){
                return i < n-1;
            }
        }
        return false;   
    }

    static vector<string> splitDefaultParameter(string p){

        string keyword = "";
        string value = "";
        bool flag = false;

        for(char ch: p){

            if(ch == '='){
                flag = true;
                continue;
            }

            if(flag){
                value += ch;
            }else{
                keyword += ch;
            }
        }
        return {keyword, value};
    }

    static void saveMnt(){

        fstream fout;
        fout.open("mnt.txt", ios::out);
        for(pair<string, int> data: mnt){
            fout << data.first << " " << data.second << endl;
        }
    }

    static void savekpd(){

        fstream fout;
        fout.open("kpd.txt", ios::out);
        for(pair<string, string> data: kpd){
            fout << data.first << " " << data.second << endl;
        }
    }

    static void savemdt(){

        fstream fout;
        fout.open("mdt.txt", ios::out);
        for(string data: mdt){
            fout << data << endl;
        }
    }

    static void savemaincode(){
        fstream fout;
        fout.open("pass2_input.txt", ios::out);
        string line;
        while(getline(fin, line)){
            fout << line << endl;
        }
    }
};

void handleMacro(){
    
    string line;
    getline(fin, line); // getting first next line of macro
    string macroMdtFirstLine = "";
    vector<string> firstLine = pass1::split(line);

    //store macro name and mdt address in mnt
    mnt.push_back({firstLine[0], mdt.size()});// initially mdt size is zero 

    macroMdtFirstLine += firstLine[0];
    macroMdtFirstLine += " ";

    map<string, int> macro_parameters;

    //Below for loop is for only first line
    for(int i=1; i<firstLine.size(); i++){

        string curr_para = firstLine[i];

        if(pass1::is_default_parameter(curr_para)){
            vector<string> default_parameter = pass1::splitDefaultParameter(curr_para);

            string key = default_parameter[0];
            string value = default_parameter[1];

            //store key value pair in kpdt
            kpd[key] = value;
            macro_parameters[key] = i;
            macroMdtFirstLine += key;
            macroMdtFirstLine += " ";
        }else{
            
            macro_parameters[curr_para] = i;
            macroMdtFirstLine += curr_para;
            macroMdtFirstLine += " ";
        }    
    }

    mdt.push_back(macroMdtFirstLine);
    //for next line > 1
    while(getline(fin, line)){

        vector<string> words = pass1::split(line);

        if(words.size() == 1){
            mdt.push_back(words[0]);
            return;
        }

        string output = words[0];
        output += " ";

        for(int i=1; i<words.size(); i++){
            output += "#";
            output += to_string(macro_parameters[words[i]]);
            output += " ";
        }

        mdt.push_back(output);
    }
}

int main(){

    fin.open("input.txt", ios::in);

    string line;
    while(getline(fin, line)){
        vector<string> words = pass1::split(line);

        int n = words.size();
        //macro starts here
        if(n == 1 && words[0] == "MACRO"){
            handleMacro();
        }
        if(n == 1 && words[0] == "START"){
            break;
        }
    }
    pass1::saveMnt();
    pass1::savekpd();
    pass1::savemdt();
    pass1::savemaincode();
}