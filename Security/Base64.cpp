#include <iostream>
#include <string>

namespace utility{
    static const char encode_table[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
    static const char BASE64_PADDING = '=';
    bool Base64Encode(const std::string& raw_string, std::string& out){
        auto raw_length = raw_string.size();
        if(0 == raw_length) return false;
        out = "";
        int equal_count{0};
        if((8 * raw_length) % 6 != 0) equal_count = (8 - (8 * raw_length) % 6) / 2;
        
        for(int i = 0; i < raw_length; ++i){
            
        }
        return true;
    }

    bool Base64Decode(const std::string& raw_string, std::string& out){

        return true;
    }
};

int main()
{
    std::string test_string{"HelloWorld"};
    
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