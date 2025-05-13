#include <iostream>
#include <cstdint>
#include <bitset>

int32_t i32_to_zigzag(int32_t n){
    return (n << 1) ^ (n >> 31);
}

int32_t zigzag_to_i32(int32_t n) {
    return ((uint32_t)n >> 1) ^ -(n & 1);
}

int write_to_buf(int32_t zigzag, uint8_t* buf, int size) {
  int res = 0;
  for(int i = 0; i<size; ++i) {
    if((zigzag & (~0x7f)) == 0) {
        buf[i] = (uint8_t)zigzag;
        res = i + 1;
        break;
    }else{
        buf[i] = (uint8_t)((zigzag & 0x7f) | 0x80);
        zigzag >>= 7;
    }
  }
  return res;
}

int32_t read_from_buf(uint8_t* buf, int max_size) {
    int32_t res = 0;
    int offset = 0;
    for(int i = 0; i < max_size; ++i, offset += 7) {
        uint8_t byte = buf[i];
        if((byte & 0x80) != 0x80) {
            res |= (byte << offset);
        }else{
            res |= ((byte & 0x7f) << offset);
        }
    }
    return res;
}

int main() {
  int32_t one = 1;
  int32_t zero = 0;
  int32_t neg_one = -1;

  int32_t zigzag_one = i32_to_zigzag(one);
  int32_t zigzag_zero = i32_to_zigzag(zero);
  int32_t zigzag_neg_one = i32_to_zigzag(neg_one);
  std::cout << "one = " << std::bitset<32>(one) << std::endl;
  std::cout << "zero = " << std::bitset<32>(zero) << std::endl;
  std::cout << "neg_one = " << std::bitset<32>(neg_one) << std::endl;

  std::cout << "\nAfter zigzag encoding:" << std::endl;
  std::cout << "zigzag(1) = " << std::bitset<32>(zigzag_one) << std::endl;
  std::cout << "zigzag(0) = " << std::bitset<32>(zigzag_zero) << std::endl;
  std::cout << "zigzag(-1) = " << std::bitset<32>(zigzag_neg_one) << std::endl;

  std::cout << "\nAfter zigzag decoding:" << std::endl;
  std::cout << "zigzag_to_i32(1) = " << std::bitset<32>(zigzag_to_i32(zigzag_one)) << std::endl;
  std::cout << "zigzag_to_i32(0) = " << std::bitset<32>(zigzag_to_i32(zigzag_zero)) << std::endl;
  std::cout << "zigzag_to_i32(-1) = " << std::bitset<32>(zigzag_to_i32(zigzag_neg_one)) << std::endl;


  std::cout << "\nAfter write to buffer:" << std::endl;
  uint8_t buf_one[10]{};
  uint8_t buf_zero[10]{};
  uint8_t buf_neg_one[10]{};

  int size_one = write_to_buf(zigzag_one, buf_one, sizeof(buf_one));
  int size_zero = write_to_buf(zigzag_zero, buf_zero, sizeof(buf_zero));
  int size_neg_one = write_to_buf(zigzag_neg_one, buf_neg_one, sizeof(buf_neg_one));

  std::cout << "size_one = " << size_one << std::endl;
  std::cout << "size_zero = " << size_zero << std::endl;
  std::cout << "size_neg_one = " << size_neg_one << std::endl;
  std::cout << "\nAfter read from buffer:" << std::endl;

  int32_t zigzag_one_from_buf = read_from_buf(buf_one, size_one);
  int32_t zigzag_zero_from_buf = read_from_buf(buf_zero, size_zero);
  int32_t zigzag_neg_one_from_buf = read_from_buf(buf_neg_one, size_neg_one);
  std::cout << "read_from_buf(buf_one) = " << zigzag_one_from_buf << ", zigzag one to i32: " << zigzag_to_i32(zigzag_one_from_buf) << std::endl;
  std::cout << "read_from_buf(buf_zero) = " << zigzag_zero_from_buf << ", zigzag zero to i32: " << zigzag_to_i32(zigzag_zero_from_buf) << std::endl;
  std::cout << "read_from_buf(buf_neg_one) = " << zigzag_neg_one_from_buf << ", zigzag neg one to i32: " << zigzag_to_i32(zigzag_neg_one_from_buf) << std::endl;
  return 0;
}