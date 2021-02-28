#include <vector>

template <unsigned int bits, typename elem_t = unsigned long long > 
struct bit_array{
    static constexpr auto elem_w = sizeof(elem_t) << 3;
    static constexpr auto elem_r = elem_w-bits;
    static constexpr elem_t mask = ~ ((~elem_t{}) << bits) ; 

    std::vector<elem_t> v;
    std::size_t last_elem{0};
    unsigned short last_bit_pos{0};

    elem_t operator [](std::size_t j) const {
        auto byte_pos = (j * bits) / 8;
        auto bit_pos = (j*bits) % 8;
        auto p = (unsigned char*)v.data();
        elem_t e = *(elem_t*) ( p + byte_pos);
        e >>= bit_pos; 
        e &= mask;        
        auto r = elem_w - bit_pos;
        if ( r < bits){
            r = bits - r;
            elem_t mask = ~(~elem_t{} << r);
            elem_t e_n = *(elem_t*)(p+byte_pos + sizeof(elem_t));
            e_n &= mask;
            e_n <<= bits - r;
            return e | e_n;
        }
        return e;
    }

    void write_at(std::size_t j, elem_t value){
        auto t = value;
        auto byte_pos = (j * bits) / 8;
        auto bit_pos = (j*bits) % 8;
        auto m = mask;
        m <<= bit_pos; m = ~m;
        auto p = (unsigned char*)v.data();
        *(elem_t*)(p + byte_pos) &= m;
        t <<= bit_pos;
        *(elem_t*)(p + byte_pos) |= t;
        if(bit_pos + bits > elem_w){
            auto remain =  bits - (elem_w - bit_pos); 
            elem_t u = value >> (bits-remain);
            *(elem_t*)(p + byte_pos + sizeof(elem_t)) &= (~elem_t{1} << remain);
            *(elem_t*)(p + byte_pos + sizeof(elem_t)) |= ( ~(~elem_t{1} << remain) & u); 
        }
    }
};