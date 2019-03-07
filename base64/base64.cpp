#include <iostream>
#include "base64.hpp"
using namespace std;

namespace base64{

inline const std::string& base64_chars(){
    static const std::string m_base64_chars = 
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "abcdefghijklmnopqrstuvwxyz"
                 "0123456789+/";
    return m_base64_chars;
}

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

// 1111 1100 = 0xFC
// 0000 0011 = 0x03
// 1111 0000 = 0xF0
// 0000 1111 = 0x0F
// 1100 0000 = 0xC0
// 0011 1111 = 0x3F

/**
 * | i | char | = |
 * | 1 | 2    | 2 |
 * | 2 | 3    | 1 |
 */
std::string base64_encode( unsigned char const* bytes_to_encode, unsigned int in_len ){
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    while( in_len-- ){
        char_array_3[i++] = *(bytes_to_encode++);
        if( i== 3 ){
            char_array_4[0] = (char_array_3[0] & 0xFC)>>2;
            char_array_4[1] = ((char_array_3[0] & 0x03)<<4) + ((char_array_3[1] & 0xF0)>>4);
            char_array_4[2] = ((char_array_3[1] & 0x0F)<<2) + ((char_array_3[2] & 0xC0)>>6);
            char_array_4[3] = char_array_3[2] & 0x3F;

            for( i = 0; i < 4; i++){
                ret += base64_chars()[char_array_4[i]];
            }
            i = 0;
        }
    }
    if(i){
        for( j=i; j<3; j++)
            char_array_3[j] = '\0';
        char_array_4[0] = (char_array_3[0] & 0xFC)>>2;
        char_array_4[1] = ((char_array_3[0] & 0x03)<<4) + ((char_array_3[1] & 0xF0)>>4);
        char_array_4[2] = ((char_array_3[1] & 0x0F)<<2) + ((char_array_3[2] & 0xC0)>>6);
        char_array_4[3] = char_array_3[2] & 0x3F;

        for( j = 0; j <= i; j++){
            ret += base64_chars()[char_array_4[j]];
        }
        
        while( i++<3 ){
            ret += "=";
        }
    }
    return ret;
}

std::string base64_encode( const std::string& enc ){
    auto bytes_to_encode = enc.c_str();
    return base64_encode( bytes_to_encode, enc.size() );
}

// 0011 0000 = 0x30
// 0000 1111 = 0x0F
// 0011 1100 = 0x3C
// 0000 0011 = 0x03
/**
 * | i | char |
 * | 2 | 1    |
 * | 3 | 2    |
 */
std::string base64_decode( const std::string& encode_string ){
    
    int in_len = encode_string.size();
    int i = 0;
    int j = 0;
    int str_in = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    std::string ret;
    while( in_len-- && ( encode_string[str_in]!='=' ) && is_base64(encode_string[str_in]) ){
        char_array_4[i++] = base64_chars().find(encode_string[str_in]); str_in++;
        if( i==4 ){
            char_array_3[0] = (char_array_4[0]<<2) + ((char_array_4[1]&0x30)>>4);
            char_array_3[1] = ((char_array_4[1] & 0x0F)<<4) + ((char_array_4[2] & 0x3C)>>2);
            char_array_3[2] = ((char_array_4[2] & 0x03)<<6) + char_array_4[3];

            for( i=0; i<3; i++)
                ret += char_array_3[i];

            i=0;
        }
    }
    
    if(i){
        for( j=i; j<4; j++)
            char_array_4[j] = '\0';
        char_array_3[0] = (char_array_4[0]<<2) + ((char_array_4[1]&0x30)>>4);
        char_array_3[1] = ((char_array_4[1] & 0x0F)<<4) + ((char_array_4[2] & 0x3C)>>2);
        char_array_3[2] = ((char_array_4[2] & 0x03)<<6) + char_array_4[3];
    
        for( j=0; j<(i-1); j++)
            ret += char_array_3[j];
    }
    return ret;
}

}

int main(){

    string str;
    while(1){
        cin>>str;
        auto base64_str = base64::base64_encode(str);
        cout<<base64_str<<endl;
        cout<<base64::base64_decode(base64_str)<<endl;
    }
    

    return 0;
}