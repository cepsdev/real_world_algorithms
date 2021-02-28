#include <vector>

template <unsigned int bits, typename elem_t = unsigned long long > 
struct bit_array{
    static constexpr auto elem_w = sizeof(elem_t) << 3;
    static constexpr auto elem_r = elem_w-bits;
    static constexpr elem_t mask = ~ ((~elem_t{}) << bits) ; 

    std::vector<unsigned long long> v;
    std::size_t last_elem{0};
    unsigned short last_bit_pos{0};

    elem_t operator [](std::size_t j) const {
        auto byte_pos = (j * bits) / 8;
        auto bit_pos = (j*bits) % 8;
        
        elem_t r = *( ((unsigned char*)v.data())+byte_pos);
        r >>= bit_pos; r &= mask;
        auto rr = (byte_pos % sizeof(elem_t))*8 + bit_pos + bits;
        std::cout << "-- " << rr <<" ";
        if (rr > elem_w){
            std::cout << "!!!\n";

        }
        return r;
    }



};