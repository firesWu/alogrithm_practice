#include <string>

namespace base58{
    std::string base58_encode( unsigned char const* bytes_to_encode, unsigned int in_len );
    inline std::string base58_encode( char const* bytes_to_encode, unsigned int in_len ){ return base58_encode( (unsigned char const*)bytes_to_encode, in_len); }
    std::string base58_encode( const std::string& enc );

    std::string base58_decode( unsigned char const* bytes_to_decode, unsigned int in_len );
    inline std::string base58_decode( char const* bytes_to_decode, unsigned int in_len ){ return base58_decode( (unsigned char const*)bytes_to_decode, in_len); }
    std::string base58_decode( const std::string& dec_str );
}