#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int countNumberofColums(string line);

void splitInputStringToArrayOfColumns(string line,vector<string> &v ,int numberOfColums);

int  countNumberOfLinesInFile(string fileName, ifstream &in, string &tmpData );

void loadTheDateFromFile(string fineName, ifstream &in,  vector<string> v[], string &tmpData, int numberOfColumnsInFile);

void checkForRepeatingColumns(vector<string> &fileAData, vector<string> &fileBData , bool isUnique[],int numberOfColumnsInMergedfile);

void saveToFile(string destFileName, ofstream &out , vector<string> plikAData[] , vector<string> plikBData[] , size_t saveCounter , size_t plikALineCount, size_t plikBLineCount,
                size_t plikAColumnCount , size_t plikBColumnCount, bool isUnique[], int numberOfColumnsInMergedfile );

bool exist(const string &name){
    ifstream file;
    file.open(name);
    if(!file){
        file.close();
        return false;
    }else{
        file.close();
        return true;
    }
}
// Option 0 if getting source file, option 1 if sprcifing destination.

string getFileNameFromUser(int option ){

    bool correctInput = false;
    string userInput;
    string parsedString;

    while(!correctInput){
        parsedString = "";
        cin >> userInput;
        size_t counter =0;

        // Exiting the program.
        if (userInput[0] == '0'){
            exit(0);
        }

        // Adding extra slash to the path for the file to be recognized
        while(userInput[counter] != 0){
            parsedString += userInput[counter];
            if(userInput[counter] == '\\'){
                parsedString += '\\';
            }
            counter ++;
        }
        if(option == 0 ){
            if(exist(parsedString)){
                correctInput = true;

            }else{
                correctInput = false;
                cout << "File not exists, provide proper file path" << endl;
            }
        }else if(option ==1){
            correctInput = true;
        }
    }
    return parsedString;
}

int main()
{
    string userInput;

    while(true){
        string plikA;
        string plikB;
        string plikZ;

        cout << "Enter absolute path to two file that you want to merge , or 0 to exit" <<endl;

        cout << "Path to first file" <<endl;
        plikA = getFileNameFromUser(0);

        cout<< "Enter path to second file" << endl;
        plikB = getFileNameFromUser(0);

        cout << "Enter path to new file to be created" << endl;
        plikZ = getFileNameFromUser(1);


//                 plikA   =    "C:\\Users\\Piotr\\Desktop\\ProgramowanieCpp\\Hard\\A.csv";
//        string plikA = "C:\\Users\\Piotr\\Desktop\\ProgramowanieCpp\\Hard\\A.csv";
//                plikB = "C:\\Users\\Piotr\\Desktop\\ProgramowanieCpp\\Hard\\B.csv";

        int plikALineCount = 0;
        int plikBLineCount = 0;
        int plikAColumnCount = 0;
        int plikBColumnCount = 0;

        string tmpData;

        ifstream in;
        ofstream out;

//            plikZ = "C:\\Users\\Piotr\\Desktop\\ProgramowanieCpp\\Hard\\Z3.csv";

        // 1. Counting number of lines in the files and number of columns;

        plikALineCount = countNumberOfLinesInFile(plikA, in , tmpData);
        plikAColumnCount = countNumberofColums(tmpData);

        plikBLineCount = countNumberOfLinesInFile(plikB, in , tmpData);
        plikBColumnCount = countNumberofColums(tmpData);

    // Saving the lines to an array

        vector<string> plikAData[plikALineCount];
        vector<string> plikBData[plikBLineCount];


        loadTheDateFromFile(plikA, in, plikAData, tmpData,plikAColumnCount);
        loadTheDateFromFile(plikB, in, plikBData, tmpData,plikBColumnCount);

        // 3. SPRAWDZANIE CZY KOLUMNY SA TAKIE SAME

        bool isUnique[plikBColumnCount];
        int numberOfColumnInMergedFile=plikAColumnCount;
        checkForRepeatingColumns(plikAData[0],plikBData[0],isUnique,numberOfColumnInMergedFile);

        // Saving to the file Z
        int saveCounter = (plikALineCount <= plikBLineCount ? plikBLineCount : plikALineCount);


        saveToFile(plikZ, out, plikAData , plikBData , saveCounter , plikALineCount, plikBLineCount , plikAColumnCount , plikBColumnCount , isUnique);

        cout << "File merged successful" << endl;
    }
}


int countNumberofColums(string line){
    // Counting numbers of columns in each file
    int columnCount =1;
    int counter =0;
    while(line[counter] != 0){


        if (line[counter] == ',')
            columnCount++;
        counter++;
    }

    return columnCount;

}

void splitInputStringToArrayOfColumns(string line,vector<string> &v ,int numberOfColums){

    int arrayCounter = 0;
    int stringCounter = 0;
    string tmpColumn;
    while(line[stringCounter] != 0){

        if(line[stringCounter] == ','){
            v.push_back(tmpColumn);
            tmpColumn = "";
            arrayCounter++;
        }else{
            tmpColumn += line[stringCounter];
        }

        stringCounter++;
    }
    v.push_back(tmpColumn);
}

int  countNumberOfLinesInFile(string fileName, ifstream &in, string &tmpData ){
    int lineCount = 0;
    in.open(fileName);
    if (in.is_open()){
        while(getline(in,tmpData)){
            lineCount++;
        }
    in.close();
    }
    return lineCount;
}

void loadTheDateFromFile(string fineName, ifstream &in,  vector<string> v[], string &tmpData, int numberOfColumnsInFile){
    size_t lineCounter =0;
    in.open(fineName);
    if (in.is_open()){
            while(getline(in,tmpData)){
                splitInputStringToArrayOfColumns
                (tmpData,v[lineCounter++], numberOfColumnsInFile);
            }
        in.close();
    }
}

void checkForRepeatingColumns(vector<string> &fileAData, vector<string> &fileBData , bool isUnique[], int numberOfColumnsInMergedfile){

for(size_t i = 0 ; i < fileBData.size() ; i ++){
        for( size_t j = 0 ; j < fileAData.size() ; j ++ ){

            if (fileBData.at(i) == fileAData.at(j)){
                isUnique[i] = false;
                break;
            }else{
                isUnique[i] = true;
                numberOfColumnsInMergedfile++;
            }
        }
    }

}

void saveToFile(string destFileName, ofstream &out , vector<string> plikAData[] , vector<string> plikBData[] , size_t saveCounter , size_t plikALineCount, size_t plikBLineCount,
                size_t plikAColumnCount , size_t plikBColumnCount, bool isUnique[], int numberOfColumnsInMergedfile){

    out.open(destFileName);
    if(out.is_open()){
        for(size_t i = 0 ; i < saveCounter ; i++){
                if ((i < plikALineCount) && (i < plikBLineCount)){
                    for(size_t j = 0; j < plikAData[i].size() ; j ++){
                        out << plikAData[i].at(j);
                        out << ',';
                    }

                    for(size_t j = 0; j < plikBData[i].size() ; j ++){

                            if (isUnique[j]){
                                out << plikBData[i].at(j);
                                out << ',';
                            }
                    }


                } else if (i >= plikALineCount){

                    for(size_t j =0 ; j < plikAColumnCount ; j ++ ){
                        out << ',';
                    }
                    for(size_t j = 0; j < plikBData[i].size() ; j ++){

                            if (isUnique[j]){
                                out << plikBData[i].at(j);
                                out << ',';
                            }
                    }

                } else if (i >= plikBLineCount){

                    for(size_t j = 0; j < plikAData[i].size() ; j ++){
                        out << plikAData[i].at(j);
                        out << ',';
                    }
                    for(size_t j =0 ; j < plikAColumnCount ; j ++ ){
                        if (isUnique[j]){
                                out << ',';
                            }
                    }
                }

                out << endl;
        }

        out.close();
    }


}


