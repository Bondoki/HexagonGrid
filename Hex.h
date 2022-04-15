// Generated code -- CC0 -- No Rights Reserved -- http://www.redblobgames.com/grids/hexagons/
// Based on https://www.redblobgames.com/grids/hexagons/implementation.html

#include <cassert>
#include <cstdlib>

struct Hex { // Axial storage, cube constructor
    const int q_, r_;
    Hex(int q, int r, int s): q_(q), r_(r) {
        assert (q + r + s == 0);
    }
    
    virtual ~Hex()
	{
        }
    
    inline int q() { return q_; }
    inline int r() { return r_; }
    inline int s() { return - q_ - r_; }
    
    inline int get_q() const { return q_; }
    inline int get_r() const { return r_; }
    inline int get_s() const { return - q_ - r_; }
    
    public:
    bool operator == (const Hex& h) const { return ((q_ == h.get_q()) && (r_ == h.get_r())); }
    
}; 

Hex hex_add(Hex a, Hex b)
{
    return Hex(a.q() + b.q(), a.r() + b.r(), a.s() + b.s());
}


Hex hex_subtract(Hex a, Hex b)
{
    return Hex(a.q() - b.q(), a.r() - b.r(), a.s() - b.s());
}


Hex hex_scale(Hex a, int k)
{
    return Hex(a.q() * k, a.r() * k, a.s() * k);
}


Hex hex_rotate_left(Hex a)
{
    return Hex(-a.s(), -a.q(), -a.r());
}


Hex hex_rotate_right(Hex a)
{
    return Hex(-a.r(), -a.s(), -a.q());
}

int hex_length(Hex hex)
{
    return int((std::abs(hex.q()) + std::abs(hex.r()) + std::abs(hex.s())) / 2);
}


int hex_distance(Hex a, Hex b)
{
    return hex_length(hex_subtract(a, b));
}


auto hexhash=[](const Hex& h){
    //return (size_t)(pt.x*100 + pt.y);
    //hash<int> int_hash;
    size_t hq = h.get_q();
    size_t hr = h.get_r();
    return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
};

auto hexequal=[](const Hex& hex1, const Hex& hex2){
    return ((hex1.get_q() == hex2.get_q()) && (hex1.get_r() == hex2.get_r()));
};

auto hexcomp = [](const Hex& hex1, const Hex& hex2){
  //return !((hex1.get_r() > hex2.get_r()) && (hex1.get_q() > hex2.get_q()) && (hex1.get_s() > hex2.get_s())); 
  int dist1=(std::abs(hex1.get_q())+ std::abs(hex1.get_q() + hex1.get_r()) + std::abs(hex1.get_r())) / 2;
  int dist2=(std::abs(hex2.get_q())+ std::abs(hex2.get_q() + hex2.get_r()) + std::abs(hex2.get_r())) / 2;
  
  
  //return ((hex1.get_r() > hex2.get_r()) && (dist1 < dist2) && (hex1.get_q() > hex2.get_q()));
  return !((hex1.get_r() > hex2.get_r()) ); 
  
};

auto hexcomp_r = [](const Hex& hex1, const Hex& hex2){
  return ((hex1.get_r() < hex2.get_r()) ); 
  
};

auto hexcomp_q = [](const Hex& hex1, const Hex& hex2){
  return !((hex1.get_q() > hex2.get_q()) ); 
  
};

