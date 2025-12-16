#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        std::size_t totalLength = k.size();
        std::size_t numGroups = 0; //total number of w[i] we'll need
        std::size_t numRemainder = totalLength%6; //track the leftover size to make later indexing easier
        bool LeftOver = 0;
        if(totalLength%6!=0){
            numGroups = (totalLength/6)+1; //we'll have an extra one that's not full
            LeftOver = 1; //extra signal to tell us that we have an extra leftover block
        }
        else{
            numGroups = totalLength/6; //whole number of full ones
        }
        unsigned long long w[5]; //our w[i] array that stores long long values 
        for(int i=0; i<5; i++){
            w[i]=0; //initialize all to 0
        }
        HASH_INDEX_T hk = 0; //our final value h(k) to be returned 
        for(int i=4; i>=0; i--){
            if((4-i+1)>(int)numGroups){
                break; //we have already filled all the blocks that we'll need. Break. 
            }
            if((4-i+1)==(int)numGroups && numRemainder!=0){ //if we're in the remainder block
                for(int j=(int)numRemainder-1; j>=0; j--){
                    w[i]*=36;
                    w[i]+=letterDigitToNumber(k[numRemainder-1-j]); //the approach on our slide
                }
            }
            else{
                for(int j=0; j<6; j++){
                    if(LeftOver==0){
                        w[i]*=36;
                        w[i]+=letterDigitToNumber(k[(numGroups-(4-i)-1)*6+j]); //most perfect case
                    }
                    else{
                        w[i]*=36;
                        w[i]+=letterDigitToNumber(k[totalLength-((4-i)+1)*6+j]); //if we're in full blocks but we do have remainders in the original string
                    }
                }
            }
        }
        for(int i=0;i<5;i++){
            hk+=rValues[i]*w[i]; //calculate hk
        }
        return hk;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if(letter>='a' && letter <='z'){
            return letter-'a'; //so that 0 starts from 'a'
        }
        else if (letter>='A' && letter<='Z'){
            return letter-'A';
        }
        //we may assume that upon this point, we would only meet 0-9 digits. 
        else{
            return letter-'0'+26; //26 starts from '0'
        }
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
