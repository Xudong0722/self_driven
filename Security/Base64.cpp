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
        int padding_count{0};
        if((8 * raw_length) % 6 != 0) padding_count = (6 - (8 * raw_length) % 6) / 2;
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
        
        binary_string += std::string(padding_count * 2, '0');
        if(binary_string.size() < 24){
            binary_string += std::string(24 - binary_string.size(), '0');
        }
        auto binary_string_length = binary_string.length();
        //std::cout << binary_string << " " << binary_string.size() << '\n';

        for(int i = 0; i < binary_string_length; i += 6){
            int start = i, end = std::min(i+6, static_cast<int>(binary_string_length));
            int result{0};
            while(start < end){
                result |= (binary_string[start] - '0');
                if(++start < end) result <<= 1;
            }
            std::cout << i << " " << end << " " << result << " " << encode_table[result] << '\n';
            out += encode_table[result];
        }

        out += std::string(padding_count, BASE64_PADDING);
        return true;
    }

    bool Base64Decode(const std::string& raw_string, std::string& out){
        auto raw_string_length = raw_string.length();
        if(0 == raw_string_length) return false;
        
        out = "";
        for(int i = 0; i<raw_string_length; ++i){
            
        }
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