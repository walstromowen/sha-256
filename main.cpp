/*
    SHA - 256 Algorithm.
    Author: Owen Walstrom
    Date: 10/19/2024
    
*/
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <cmath>
using namespace std;

string convertStringToBinary(string message){
    string binaryMessage = "";
    for(int i = 0; i < message.length(); i++){
        bitset<8> currentCharBitset(message[i]);
        binaryMessage += currentCharBitset.to_string();
    };
    return binaryMessage;
};
string createPaddingBits(string message){
    string paddedMessage = "";
    int chunks = floor((message.length() + 63) / (512) ) + 1;
    int paddingLength = (chunks * 512) - 64 - message.length();
    for(int i = 0; i < paddingLength; i++){
        if(i == 0){
            paddedMessage += "1";
        }else{
            paddedMessage += "0";
        }
    }
    return paddedMessage;
};
string createLengthBits(string binaryMessage){
    bitset<64> binaryMessageLength(binaryMessage.length());
    return binaryMessageLength.to_string();
};
string rotateLeft(string word, int rotationCount){//"001100111"
    string rotatedString = word;
    for(int i = 0; i < word.length(); i++){
        int index = i - rotationCount;
        if(index < 0){
            index = word.length() - (index + word.length());
        }
        rotatedString[i] = word[index];
    }
    return rotatedString;
};
string rotateRight(string word, int rotationCount){//"001100111"
    string rotatedString = word;
    for(int i = 0; i < word.length(); i++){
        int index = i + rotationCount;
        if(index >= (word.length())){
            index = index - word.length();
        }
        rotatedString[i] = word[index];
    }
    return rotatedString;
};
string shiftRight(string word, int shiftCount){
    string shiftedString = word;
    for(int i = 0; i < word.length(); i++){
        int index = i + shiftCount;
        if(index >= (word.length())){
            shiftedString[i] = '0';
        }else{
            shiftedString[i] = word[index];
        }
    }
    return shiftedString;
};
string bitwise_not(string string1){
    string output = "";
    string nextNumber;
    int a;
    for(int i = 0; i < 32; i++){
        if(string1[i] == '0'){
             nextNumber = "0";
        }else{
             nextNumber = "1";
        }
        output += nextNumber; 
    }
    return output;
}
string bitwise_xor(string string1, string string2){
    string output = "";
    cin>>output;
    string nextNumber;
    int a, b;
    for(int i = 0; i < 32; i++){
        if(
            (string1[i] == '0' && string2[i] == '0' ) || (string1[i] == '1' && string2[i] == '1')
        ){
             nextNumber = "0";
        }else{
             nextNumber = "1";
        }
        output += nextNumber; 
    }
    return output;
}
string bitwise_and(string string1, string string2){
    string output = "";
    string nextNumber;
    int a, b;
    for(int i = 0; i < 32; i++){
        if(
            (string1[i] == '0' || string2[i] == '0')
        ){
             nextNumber = "0";
        }else{
             nextNumber = "1";
        }
        output += nextNumber; 
    }
    return output;
}
string add(string a, string b){
   string result = "";
   int temp = 0;
   int size_a = a.size() - 1;
   int size_b = b.size() - 1;
   while (size_a >= 0 || size_b >= 0 || temp == 1){
      temp += ((size_a >= 0)? a[size_a] - '0': 0);
      temp += ((size_b >= 0)? b[size_b] - '0': 0);
      result = char(temp % 2 + '0') + result;
      temp /= 2;
      size_a--; size_b--;
   }
   while(result.length() > 32){ //effectively acts as mod 2^32
        result.erase(0,1);
   }
   return result;
};
int main(){
    //Step 0 Initialize
    string message = "hello world";
    string binaryMessage = convertStringToBinary(message);
    
    //Step 1 Preprocess
    string paddedBits = createPaddingBits(binaryMessage);
    string lengthedBits = createLengthBits(binaryMessage);
    string fullyPreprocessedMessage = binaryMessage + paddedBits + lengthedBits;
    //cout << fullyPreprocessedMessage << endl;

    //Step 2 Initialize Hash Values
    //first 32 bits of fractional(digits after decimal point) parts of square roots of the first 8 prime numbers in hexadecimal format
    string hashValue;
    string h0 = "0x6a09e667";
    string h1 = "0xbb67ae85";
    string h2 = "0x3c6ef372";
    string h3 = "0xa54ff53a";
    string h4 = "0x510e527f";
    string h5 = "0x9b05688c";
    string h6 = "0x1f83d9ab";
    string h7 = "0x5be0cd19";

    //Step 3 Initialize Round Constants
    //first 32 bits of the fractional parts of the cube roots of the first 64 prime numbers 
    string roundConstants[] = {
        "0x428a2f98", "0x71374491", "0xb5c0fbcf", "0xe9b5dba5", "0x3956c25b", "0x59f111f1", "0x923f82a4", "0xab1c5ed5", "0xd807aa98",
        "0x12835b01", "0x243185be", "0x550c7dc3", "0x72be5d74", "0x80deb1fe", "0x9bdc06a7", "0xc19bf174", "0xe49b69c1", "0xefbe4786",
        "0x0fc19dc6", "0x240ca1cc", "0x2de92c6f", "0x4a7484aa", "0x5cb0a9dc", "0x76f988da", "0x983e5152", "0xa831c66d", "0xb00327c8",
        "0xbf597fc7", "0xc6e00bf3", "0xd5a79147", "0x06ca6351", "0x14292967", "0x27b70a85", "0x2e1b2138", "0x4d2c6dfc", "0x53380d13",
        "0x650a7354", "0x766a0abb", "0x81c2c92e", "0x92722c85", "0xa2bfe8a1", "0xa81a664b", "0xc24b8b70", "0xc76c51a3", "0xd192e819",
        "0xd6990624", "0xf40e3585", "0x106aa070", "0x19a4c116", "0x1e376c08", "0x2748774c", "0x34b0bcb5", "0x391c0cb3", "0x4ed8aa4a",
        "0x5b9cca4f", "0x682e6ff3", "0x748f82ee", "0x78a5636f", "0x84c87814", "0x8cc70208", "0x90befffa", "0xa4506ceb", "0xbef9a3f7",
        "0xc67178f2", 
    };
    string messageSchedule[] = { // this is w
        fullyPreprocessedMessage.substr(0, 32), fullyPreprocessedMessage.substr(33, 32),
        fullyPreprocessedMessage.substr(64, 32), fullyPreprocessedMessage.substr(96, 32),
        fullyPreprocessedMessage.substr(128, 32), fullyPreprocessedMessage.substr(160, 32),
        fullyPreprocessedMessage.substr(192, 32), fullyPreprocessedMessage.substr(224, 32),
        fullyPreprocessedMessage.substr(256, 32), fullyPreprocessedMessage.substr(288, 32),
        fullyPreprocessedMessage.substr(320, 32), fullyPreprocessedMessage.substr(352, 32),
        fullyPreprocessedMessage.substr(384, 32), fullyPreprocessedMessage.substr(416, 32),
        fullyPreprocessedMessage.substr(448, 32), fullyPreprocessedMessage.substr(480, 32),

        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",

        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",

        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000",
        "00000000000000000000000000000000", "00000000000000000000000000000000"
    };
    //Step 4 Computations
    //perform on every 512 chunk of data from preproccessed message
    int chunks = floor((binaryMessage.length() + 63) / (512) ) + 1;
    for(int rounds = 0; rounds < chunks; rounds++){
        for(int j = 16; j < 64; j++){
            string temp = bitwise_xor(
                rotateRight(messageSchedule[j-15], 7),
                rotateRight(messageSchedule[j-15], 18)
            );
            string sigma0 = bitwise_xor(
                temp, 
                shiftRight(messageSchedule[j-15], 3)
            );
            temp = bitwise_xor(
                rotateRight(messageSchedule[j-2], 17),
                rotateRight(messageSchedule[j-2], 19)
            );
            string sigma1 = bitwise_xor(
                temp, 
                shiftRight(messageSchedule[j-2], 10)
            );
            messageSchedule[j] = add(
                add(messageSchedule[j-16], sigma0),
                add(messageSchedule[j-7], sigma1)
            );
        }// Step 5 Compression
        string a = h0;
        string b = h1;
        string c = h2;
        string d = h3;
        string e = h4;
        string f = h5;
        string g = h6;
        string h = h7;
        for(int i = 0; i < 64; i++){
            string s1 = bitwise_xor(
                bitwise_xor(rotateRight(e, 6), rotateRight(e, 11)),
                rotateRight(e, 25)
            );
            string ch = bitwise_xor(
                bitwise_and(e, f), bitwise_and(bitwise_not(e), g)
            );
            string temp1 = add(
                add(
                    add(h, s1),
                    add(ch, roundConstants[i])
                ), messageSchedule[i]
            );
            string s0 = bitwise_xor(
                bitwise_xor(
                    rotateRight(a, 2), 
                    rotateRight(a, 13)
                ),
                rotateRight(a, 22)
            );
            string maj = bitwise_xor(
                bitwise_xor(
                    bitwise_and(a, b), 
                    bitwise_and(a, c)
                ),
                bitwise_and(b, c)
            );
            string temp2 = add(s0, maj);
            h = g;
            g = f;
            f = e;
            e = add(d, temp1);
            d = c;
            c = b;
            b = a;
            a = add(temp1, temp2);

        }
        //Step 6 Modify Final Values
        h0 = add(h0,a);
        h1 = add(h1,b);
        h2 = add(h2,c);
        h3 = add(h3,d);
        h4 = add(h4,e);
        h5 = add(h5,f);
        h6 = add(h6,g);
        h7 = add(h7,h);
        hashValue = h0 + h1 + h2 + h3 + h4 + h5 + h6 + h7;
        /*
        cout << "Message Schedule Round " << rounds << ":" << endl;
        for(int i = 0; i < 64; i++){
            cout << i+1 << ": "  << messageSchedule[i] << endl;
    
        }
        //cout <<fullyPreprocessedMessage.substr(480, 32);
        //cout << rotateRight(fullyPreprocessedMessage.substr(480, 32), 2);
       */

    }
    cout << "Original Message: " << endl;
    cout << message << endl;
    cout << "Hash Value: " << endl;
    cout << hashValue << endl;
    
    return 0;
    
}










/*
string doubleModdedBitwiseAddition(string string1, string string2){
    string output = "";
    int a, b;
    for(int i = 0; i < 31; i++){
        if(string1[i] == '0'){
            a = 0;
        }else{
            a = 1;
        }
        if(string2[i] == '0'){
            b = 0;
        }else{
            b = 1;
        }
        int power = pow(2.0, 32.0);
        string nextNumber = to_string((a + b) % power);
        output += nextNumber; 
    }
    
    return output;
}
string trippleModdedBitwiseAddition(string rotated1, string rotated2, string shifted1){
    string output = "";
    int a, b, c;
    for(int i = 0; i < 31; i++){
        if(rotated1[i] == '0'){
            a = 0;
        }else{
            a = 1;
        }
        if(rotated2[i] == '0'){
            b = 0;
        }else{
            b = 1;
        }
        if(shifted1[i] == '0'){
            c = 0;
        }else{
            c = 1;
        }
        string nextNumber = to_string((a + b + c) % 2);
        output += nextNumber; 
    }
    
    return output;
}
*/


      /*
        string sigma0 = trippleModdedBitwiseAddition(
            rotateRight(messageSchedule[j-15], 7), 
            rotateRight(messageSchedule[j-15], 18), 
            shiftRight(messageSchedule[j-15], 3) 
        );
        string sigma1 = trippleModdedBitwiseAddition(
            rotateRight(messageSchedule[j-2], 17), 
            rotateRight(messageSchedule[j-2], 19), 
            shiftRight(messageSchedule[j-2], 10) 
        );
        messageSchedule[j] = doubleModdedBitwiseAddition(
            doubleModdedBitwiseAddition(messageSchedule[j-16], sigma0),
            doubleModdedBitwiseAddition(messageSchedule[j-7], sigma1)
        );
        */