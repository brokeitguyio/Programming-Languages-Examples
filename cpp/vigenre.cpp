#include <iostream>
using namespace std;

int main(){
    string plaintext;
    string ciphertext;
    string key;

    cout<<"Enter plaintext: ";
    cin>>plaintext;
    cout<<"Enter key: ";
    cin>>key;

    sting newKey;
    while(newkey.length()<plaintext.length()){
        newKey+=key;
    }
    newKey=newKey.substr(0,plaintext.length())

    for(int i=0;i<plaintext.length();i++){
        ciperText+=(plaintext[i]+newkey[i])%26+65;
        //ciperText+=(plaintext[i]-newkey[i])%26+65;
    }
    cout<< ciperText << endl;
    return 0;
}