#include<bits/stdc++.h>
using namespace std;

class Table{
    public:
        int idx = -1;
        string data = "";
        int address = -1;

        Table(int idx, string data){
            this->idx = idx;
            this->data = data;
        }
        Table(int idx, string data, int address){
            this->idx = idx;
            this->data = data;
            this->address = address;
        }
};

vector<string> intermediateCode;
vector<Table*> symbolTable;
vector<Table*> literalTable;

map<string, string> dp;

static int counter = 0;

class pass1{
    public: 

    //printing the vector
    static void printLine(vector<string> line){
        for (string word : line)
        {
            cout << "-" << word << "-";
        }
        cout << endl;
    }

    //spliting a sentence of words into word and storing in vector of words
    static vector<string> split(string line){
        vector<string> result;

        string word = "";

        for(char i: line){

            if(i == ' '){
                result.push_back(word);
                word = "";
            }else{
                word += i;
            } 
        }
        result.push_back(word);
        return result;
    }
    //creating MOT
    static void prepareDp(){

        dp["STOP"] = "{IS, 00}";
        dp["ADD"] = "{IS, 01}";
        dp["SUB"] = "{IS, 02}";
        dp["MULT"] = "{IS, 03}";
        dp["MOVER"] = "{IS, 04}";
        dp["MOVEM"] = "{IS, 05}";
        dp["COMP"] = "{IS, 06}";
        dp["BC"] = "{IS, 07}";
        dp["DIV"] = "{IS, 8}";
        dp["READ"] = "{IS, 9}";
        dp["PRINT"] = "{IS, 10}";
        dp["START"] = "{AD, 01}";
        dp["END"] = "{AD, 02}";
        dp["ORIGIN"] = "{AD, 03}";
        dp["EQU"] = "{AD, 04}";
        dp["LTORG"] = "{AD, 05}";

        dp["DS"] = "{DL, 01}";
        dp["DC"] = "{DL, 02}";

        dp["AREG"] = "{REG, 1}";
        dp["BREG"] = "{REG, 2}";
        dp["CREG"] = "{REG, 3}";
        dp["DREG"] = "{REG, 4}";

        dp["LT"] = "{CC, 1}";
        dp["LE"] = "{CC, 2}";
        dp["EQ"] = "{CC, 3}";
        dp["GT"] = "{CC, 4}";
        dp["GE"] = "{CC, 5}";
        dp["ANY"] = "{CC, 6}";
    }

    //for returning constant string in intermediate code
    static string getConstantString(string constant){

        string start = "{C,";
        string end = "}";
        return start + constant + end;
    }

    //for returning string  in intermediate code
    static string addSymbol(string symbol){
        for (Table *row : symbolTable)
        {
            if(row->data == symbol)
                return "{S, " + to_string(row->idx) + "}";
        }
        int idx = symbolTable.size();
        Table *row = new Table(idx, symbol);
        symbolTable.push_back(row);
        return "{S, " + to_string(idx) + "}";
    }
    // checking whether given string data[0] is constant or not 
    static bool isConstant(string data){
        return data[0] >= '0' && data[0] <= '9';
    }
    // checking whether given string word[0] is word or not 
    static bool isSymbol(string word){
        return ((word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A' && word[0] <= 'Z'));
    }

    static string addSymbol(string symbol, int address){
        for (Table *row : symbolTable)
        {
            if(row->data == symbol){
                row->address = address;
                return "{S, " + to_string(row->idx) + "}";
            }
        }
        int idx = symbolTable.size();
        Table *row = new Table(idx, symbol, address);
        symbolTable.push_back(row);
        return "{S, " + to_string(idx) + "}";
    }

    static string addLiteral(string literal)
    {
        for (Table *row : literalTable)
        {
            if (row->data == literal)
                return "{L, " + to_string(row->idx) + "}";
        }
        int idx = literalTable.size();
        Table *row = new Table(idx, literal);
        literalTable.push_back(row);
        return "{L, " + to_string(idx) + "}";
    }

    static void handleLtorg(){

        int temp = counter;
        for (Table *row : literalTable){
            if(row->address == -1){
                row -> address = temp;
                temp += 1;
            }
        }
    }

    // storing in intermediate code txt 
    static void saveIntermediateCode(){

        fstream fout;
        fout.open("intermediate_code.txt", ios::out);
        for(string row: intermediateCode){
            fout << row << endl;
        }
        fout.close();
    }
   
    // storing in literal table code txt 
    static void saveLiteralTable()
    {

        fstream fout;
        fout.open("literal_table.txt", ios::out);

        for(Table* row: literalTable){
            fout << row->idx << " " << row->data << " " << row->address << endl;
        }
    }

    // storing in symbol table code txt
    static void saveSymbolTable()
    {
        fstream fout;
        fout.open("symbol_table.txt", ios::out);

        for (Table *row : symbolTable)
        {
            fout << row->idx << " " << row->data << " " << row->address << endl;
        }
    }
};

void handleLine(vector<string> line)
{

    pass1::printLine(line);
    string currInter;
    if (line[0] == "START")//start 
    {
        // set counter
        counter = stoi(line[1]);

        cout << "counter value set to " << counter << endl;
        // add to intermidiate code
        currInter += dp[line[0]];
        currInter += " {C," + line[1] + "}";

        intermediateCode.push_back(currInter);
        return;
    }//done

    counter += 1;
    if(line[0] == "LTORG"){
        pass1::handleLtorg();
    }

    // if vector is of size 2 then there must be an IS or AD and symbol
    if (line.size() == 2)
    {
        currInter += dp[line[0]];
        currInter += pass1::addSymbol(line[1]);
        intermediateCode.push_back(currInter);
        return;
    }

    // for vector of size 3 and 4
    //  word can be
    //  1. Symbol
    //  2. Memo
    //  3. Literal
    //  4. constant

    for (int i = 0; i < line.size(); i++)
    {

        string word = line[i];
        // if word is memo
        if (dp.find(word) != dp.end())
        {
            currInter += dp[word];
            currInter += " ";
        }
        else if (pass1::isConstant(word))
        {
            currInter += pass1::getConstantString(word);
            currInter += " ";
        }
        else if (pass1::isSymbol(word))
        {

            if (i == 0)
            {
               pass1::addSymbol(word, counter);
            }
            else
            {
                currInter += pass1::addSymbol(word);
                currInter += " ";
            }
        }
        else
        {
            currInter += pass1::addLiteral(word);
            currInter += " ";
        } 
    }
    intermediateCode.push_back(currInter);
}

int main(){

    ifstream inputFile;
    inputFile.open("input.txt", ios::in);

    pass1::prepareDp();

    string line;
    while(getline(inputFile, line)){

        vector<string> words = pass1::split(line);

        handleLine(words);

    }
    pass1::handleLtorg();
    pass1::saveIntermediateCode();
    pass1::saveLiteralTable();
    pass1::saveSymbolTable();
}


