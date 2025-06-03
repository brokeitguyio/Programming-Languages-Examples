#include <iostream>
using namespace std;

string vigenere(string plaintext, string key,bool EndcDec);

int main(){
    
    string plainText;
    string cipherText;
    string key;

do{
    cout<<"==============================="<<endl;
    cout<<"Encryption & Decrptyion station"<<endl;
    coutendl<<"==============================="<<endl;
    cout<<"Please choose"endl;
    cout<<"1. Encryption"<<endl;
    cout<<"2. Decryption"<<endl;
    cout<<"3. Exit"<<endl;
    cout<<"==============================="<<endl;
    int choice;
    cin>>choice;

    switch(choice){
        case 1:
            cout<<"Encryption"<<endl;
            cin>>plainText;
            cout<<"Enter the key: ";
            cin>>key;
            ciphertext=vigenere(plainText,key,0);
            cout<<"Ciphertext: "<<cipherText<<endl;
            cout<< "Press Y/y to repeat the process of any key to exit"<<endl;
            break;
        case 2:
            cout<<"Decryption"<<endl;
            cin>>plainText;
            cout<<"Enter the key: ";
            cin>>key;
            cipherText=vigenere(plainText,key,1);
            cout<<"PlainText: "<<cipherText<<endl;
            out<< "Press Y/y to repeat the process of any key to exit"<<endl;
            break;
        case 3:
            cout<<"Exit"<<endl;
            x='n';
            break;
        default:
            cout<<"Invalid Choice"<<endl;
            break;
    }
    } while(x=='Y'|| x=='y');
    return 0;
}

string vigenere(string plaintext, string key,bool EndcDec){
    string newkey=key,newText;
    while(newkey.length() <plaintext.length){
        newkey+=key;
    }
    newkey=newkey.substr(0,plaintext.length)
    
    if(EncDec==0){
        for(int i=0;i<plaintext.length();i++){
            newText+=((plaintext[i]+newkey[i])%26)+65;
        }      
    }
    else{
         for(int i=0;i<plaintext.length();i++){
            newText+=((plaintext[i]-newkey[i])%26)+65;
        }      
    }
}