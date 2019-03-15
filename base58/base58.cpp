#include <iostream>
#include <vector>
#include <cmath>
#include "base58.hpp"
using namespace std;

namespace base58{

inline const std::string& base58_chars(){
    static const std::string m_base58_chars = 
                 "123456789"
                 "ABCDEFGHJKLMNPQRSTUVWXYZ"
                 "abcdefghijkmnopqrstuvwxyz";
    return m_base58_chars;
}

static inline bool is_base58(unsigned char c) {
  return (isalnum(c) && (c != '0') && (c != 'O') && (c != 'I') && (c != 'l'));
}

std::string base58_encode( unsigned char const* bytes_to_encode, unsigned int in_len ){
    string ret;
    int zero_counter = 0;
    int length = 0;

    while(*bytes_to_encode==0 && in_len !=0 ){
        zero_counter++;
        bytes_to_encode++;
        in_len--;
    }

    int size = in_len * log(256) / log(58) + 4;
    vector<unsigned char> b58(size, 0);
    while( in_len-- ){
        int carry = *(bytes_to_encode++);
        int i = 0;
        for( vector<unsigned char>::reverse_iterator itr = b58.rbegin(); itr != b58.rend() && (carry != 0 || i < length ); itr++, i++ ){
            carry += 256 * (*itr);
            *itr = carry % 58;
            carry = carry / 58;
        }

        assert( carry == 0 );
        length = i;
    }

    vector<unsigned char>::iterator itr = b58.begin();
    while( itr != b58.end() && *itr == 0 ) itr++;
    while( zero_counter-- ) ret += base58_chars()[0];
    while( itr != b58.end() ) ret += base58_chars()[*(itr++)];

    return ret;
}

std::string base58_encode( const std::string& enc ){
    auto bytes_to_encode = enc.c_str();
    return base58_encode( bytes_to_encode, enc.size() );
}

std::string base58_decode( unsigned char const* bytes_to_decode, unsigned int in_len ){
    string ret;
    int zero_counter = 0;
    int length = 0;

    while( *bytes_to_decode == base58_chars()[0] ){
        zero_counter++;
        bytes_to_decode++;
        in_len--;
    }

    int size = in_len * log(58) / log(256) + 1;
    vector<unsigned char> b256(size, 0);

    while( in_len-- && is_base58(*bytes_to_decode) ){
        int carry = base58_chars().find(*bytes_to_decode);bytes_to_decode++;
        int i = 0;
        for( vector<unsigned char>::reverse_iterator itr = b256.rbegin(); itr != b256.rend() && (carry != 0 || i < length ); itr++, i++ ){
            carry += 58 * (*itr);
            *itr = carry % 256;
            carry = carry / 256;
        }
        assert( carry == 0 );
        length = i;
    }
    vector<unsigned char>::iterator itr = b256.begin();
    while( itr != b256.end() && *itr == 0 ) itr++;
    while( zero_counter-- ) ret += (char)0;
    while( itr != b256.end() ) ret += *(itr++);

    return ret;
}
    
std::string base58_decode( const std::string& dec_str ){
    auto bytes_to_decode = dec_str.c_str();
    return base58_decode( bytes_to_decode, dec_str.size() );
}

}
int main(){

    vector<unsigned char> bignum;
    bignum.push_back(0);
    bignum.push_back(0);
    bignum.push_back('a');
    bignum.push_back('d');
    bignum.push_back('f');
    auto enc_str = base58::base58_encode(bignum.data(),bignum.size());
    cout<<enc_str<<endl;
    cout<<base58::base58_decode(enc_str)<<endl;

    string str;
    while(1){
        cin>>str;
        auto base58_str = base58::base58_encode(str);
        cout<<base58_str<<endl;
        cout<<base58::base58_decode(base58_str)<<endl;
    }

    return 0;
}