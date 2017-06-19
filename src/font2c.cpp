//
// Created by damiles on 19/06/17.
//
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, const char* argv[])
{
    FILE* fp = 0;
    int dataSize = 0, readed;
    unsigned char* data = NULL;

    if(argc!=4){
        cout << "Arguments not correctly passed\n" << endl;
        cout << "font2c font_path out_file variable_name\n" << endl;
        return 0;
    }


    const char* path= argv[1];
    const char* file_result= argv[2];
    const char* variable_name= argv[3];


    // Read in the font data.
    fp = fopen(path, "rb");
    if (fp == NULL){
        cout << "Error reading file" << endl;
        return 0;
    }
    fseek(fp,0,SEEK_END);
    dataSize = (int)ftell(fp);
    fseek(fp,0,SEEK_SET);
    data = (unsigned char*)malloc(dataSize);
    if (data == NULL){
        cout << "Data is null" << endl;
        return 0;
    }
    readed = fread(data, 1, dataSize, fp);
    fclose(fp);
    fp = 0;
    if (readed != dataSize){
        cout << "Error reading data" << endl;
        return 0;
    }


    ofstream myfile;
    myfile.open (file_result);
    myfile << " /* Font to C by Damiles */ \n";
    myfile << "int font_" << variable_name << "_size= " << dataSize << ";\n";
    myfile << "unsigned char font_" << variable_name << "[]= {\n";
    for(int i=0; i<dataSize; i++){
        if(i%32==31)
            myfile << "\n";
        if(i==dataSize-1){
            myfile << (int)data[i];
        }else{
            myfile << (int)data[i] << ", ";
        }
    }
    myfile << "};\n";
    myfile.close();

}