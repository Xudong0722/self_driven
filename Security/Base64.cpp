#include <iostream>
#include <string>
#include <algorithm>

namespace utility{
    static const char encode_table[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
    static const char BASE64_PADDING = '=';
    bool Base64Encode(const std::string& raw_string, std::string& out){
        auto raw_length = raw_string.size();
        if(0 == raw_length) return false;
        out = "";
        int equal_count{0};
        if((8 * raw_length) % 6 != 0) equal_count = (6 - (8 * raw_length) % 6) / 2;
        std::string binary_string;
        for(int i = 0; i < raw_length; ++i){
            std::string single_char{"00000000"};
            int offset = static_cast<int>(raw_string[i]);
            for(int j = 0; j < 8; ++j){
                if((offset >> j) & 1)
                    single_char[7-j] = '1';
            }
            binary_string += single_char;
        }
        
        for(int i = 0; i < raw_length * 8; i += 6){
            int start = i, end = std::min(i+6, static_cast<int>(raw_length * 8));
            int result{0};
            while(start < end){
                result |= (binary_string[start] - '0');
                if(++start < end) result <<= 1;
            }
            out += encode_table[result];
        }

        out += std::string(equal_count, '=');
        return true;
    }

    bool Base64Decode(const std::string& raw_string, std::string& out){

        return true;
    }
};

int main()
{
    std::string test_string{"My name is elvis"};
    
    std::string base64_encode_result;
    if(utility::Base64Encode(test_string, base64_encode_result)){
        std::cout << test_string << " base64 encode: " << base64_encode_result << '\n';
    }else{
        std::cout << "Base64 encode failed.\n";
    }

    std::string base64_decode_result;
    if(utility::Base64Decode(base64_encode_result, base64_decode_result)){
        std::cout << base64_encode_result << "base64 decode: " << base64_decode_result << '\n';
    }else{
        std::cout << "Base64 decode failed.\n";
    }
    return 0;
}