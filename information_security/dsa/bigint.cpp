
#include "bigint.h"

void BigInt::init(int size){
    if(size < 1)
        size = 1;
    values = new Value_type[size];
    values_size = size;
    values[0] = 0;
    len = 1;
}

BigInt::BigInt(const Value_type val){
    values = nullptr;
    if(val >= mod_part){
        init(start_size);
        len = 2;
        values[0] = val & (mod_part -1);
        values[1] = val >> type_value_size;
    }
    else{
        init(start_size);
        len = 1;
        values[0] = val;
    }
}

BigInt::BigInt(const BigInt& old){
    len = old.len;
    values_size = len;
    values = new Value_type[len];
    if(old.values) // TODO why is this needed
        memcpy(values,old.values,len*sizeof(Value_type));
}

void BigInt::resize(int new_size){
    if(new_size < len){
        exit(2);
    }
    if(new_size <= 0){
        new_size = 1;
    }
    Value_type* new_arr = new Value_type[new_size];
    memcpy(new_arr,values,len*sizeof(Value_type));
    delete [] values;

    values = new_arr;
    if(len < new_size){
        memset(values+len,0,(new_size-len)*sizeof(Value_type));
    }
    values_size = new_size;
}

void BigInt::update_len(){
    while(len > 1 && values[len-1] == 0)
        --len;
}

void BigInt::upscale(int new_size){
    if(new_size > values_size){
        if(new_size > values_size + values_size)
            resize(new_size);
        else
            resize(values_size+values_size);
    }
}

void BigInt::print(ostream& str/* = cout*/, PrintType type /*= PrintType::DEC*/,int padding/* = 0*/)const{

    if(padding == 0){
        padding = len;
    }
    if(type == BIN){
        if(padding < len) padding = len;
        for(int i=padding-1;i>=0;--i)
            str << std::bitset<type_value_size>(get_value_at(i)) << " ";
        str<<"\n";
        return;
    }
    if(type == DEC){
        stringstream out;

        if(*this == zero){
            str << "0\n";
            return;
        }

        BigInt tmp(*this);
        while( tmp > BigInt(0)){
            out << (tmp%BigInt(10)).values[0];
            tmp /= BigInt(10);
        }
        string s;
        getline(out,s);
        while(s.length() < padding){
            s += " ";
        }
        std::reverse(s.begin(),s.end());
        str<<s<<"\n";
        return;
    }
    if(type == HEX) str <<  setfill('0') << setw(4) << std::hex;
    if(padding < len) padding = len;
    for(int i=padding-1;i>=0;--i)
        str <<  setfill('0') << setw(4) << get_value_at(i) << "";
    str<<"\n";
}

BigInt& BigInt::operator+=(const BigInt& a){
    int res_len = max(len,a.len) + 1;
    upscale(res_len);
    Value_type cur_sum = 0;
    for(int i=0;i<res_len;++i){
        cur_sum += get_value_at(i) + a.get_value_at(i);
        values[i] = cur_sum & (mod_part - 1);
        cur_sum >>= type_value_size;
    }
    len = res_len;
    update_len();
    return *this;
}

BigInt BigInt::operator+(const BigInt& a) const{
    BigInt res(*this);
    return res+=a;
}

BigInt& BigInt::operator=(const BigInt& old) {
    len = old.len;
    if(!values || old.len > values_size){
        if(values){
            delete [] values;
        }
        values = new Value_type[old.len];
        values_size = old.len;
    }
    memcpy(values,old.values,len*sizeof(Value_type));
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& a){

    int res_len = len+a.len+2;
    BigInt res = (Value_type)0;
    res.upscale(res_len);
    res.len = res_len;
    Value_type cur_sum = 0;
    for(int i=0;i<len;++i){
        for(int j=0;j<a.len || cur_sum > 0;++j){
            cur_sum += values[i] * a.get_value_at(j);
            res.values[i+j] += cur_sum & (mod_part - 1);
            cur_sum = ((cur_sum >> type_value_size) + (res.values[i+j] >> type_value_size));
            res.values[i+j] &= (mod_part-1);
        }
    }
    res.update_len();
    return *this = res;
}

BigInt BigInt::operator*(const BigInt& a) const{
    BigInt res(*this);
    return res*=a;
}


bool BigInt::operator==(const BigInt& a) const{
    if(len!=a.len){
        return false;
    }
    int i=0;
    while( i<len && values[i] == a.values[i]) ++i;
    return i>=len;
}

bool BigInt::operator<(const BigInt& a) const{
    if(len!=a.len){
        return len < a.len;
    }
    int i=len-1;
    while( i>=0 && values[i] == a.values[i]) --i;
    return i>=0 && (values[i] < a.values[i]);
}

bool BigInt::operator>(const BigInt& a) const{
    if(len!=a.len){
        return len > a.len;
    }
    int i=len-1;
    while( i>=0 && values[i] == a.values[i]) --i;
    return i>=0 && (values[i] > a.values[i]);
}

bool BigInt::operator<=(const BigInt& a) const{
    if(len!=a.len){
        return len < a.len;
    }
    int i=len-1;
    while( i>=0 && values[i] == a.values[i]) --i;
    return i<0 || (values[i] < a.values[i]);
}

bool BigInt::operator>=(const BigInt& a) const{
    if(len!=a.len){
        return len > a.len;
    }
    int i=len-1;
    while( i>=0 && values[i] == a.values[i]) --i;
    return i<0 || (values[i] > a.values[i]);
}

BigInt BigInt::div2(){
    BigInt res(*this);
    int bit = 0;
    for(int i=len-1;i>=0;--i){
        res.values[i] |= (bit << type_value_size);
        bit = res.values[i] & 1;
        res.values[i] >>= 1;
    }

    res.update_len();
    return res;
}

BigInt& BigInt::operator/=(const BigInt& a){
    BigInt l = 1,
            r = *this,
            m;
    BigInt one = BigInt(1);
    while( l + one < r){
        m = (l+r).div2();

        if ( m * a <= *this ){
            l = m+one;
        }else{
            r = m;
        }
    }
    if( l * a <= *this){
        return *this = l;
    }
    return *this = l - BigInt(1);
}

BigInt BigInt::operator/(const BigInt& a) const {
    BigInt res(*this);
    return res/=a;
}

BigInt& BigInt::operator-=(const BigInt& a){
    if(*this <= a){
        return *this = zero;
    }
    int minus_one = 0;
    for(int i=0;i< len;++i){
        if(values[i] < a.get_value_at(i) + minus_one){
            values[i] += mod_part-a.get_value_at(i) - minus_one;
            minus_one = 1;
        }else{
            values[i]-=a.get_value_at(i) + minus_one;
            minus_one = 0;
        }
    }
    update_len();
    return *this;
}

BigInt BigInt::operator-(const BigInt& a) const {
    BigInt res(*this);
    return res-=a;
}

BigInt BigInt::operator%(const BigInt& a){
    return *this - (*this/a)*a;
}

bool BigInt::operator==(const Value_type a) const{
    return (len == 1 && values[0] == a) || (len == 2 && values[0] + values[1] << type_value_size == a);
}



BigInt& BigInt::operator=(const string& old){
    *this = 0;
    for(int i=0;i<old.length();++i){
        *this = *this * BigInt(10) + BigInt(old[i] - '0');
    }
    return *this;
}

BigInt& BigInt::operator=(const Value_type old){
    return *this = BigInt(old);
}


int char_code(char c){
    if( c>='0'&& c<='9')return c-'0';
    if( c>='a'&& c<='f')return c-'a'+10;
    if( c>='A'&& c<='F')return c-'A'+10;
    return 0;
}
BigInt& BigInt::operator+=(const Value_type a){
    upscale(len+1);
    Value_type add = a;
    values[len] = 0;
    for(int i=0;i<len && add >= 0;++i){
        add+=get_value_at(i);
        values[i] = add & ( mod_part - 1);
        add >>= type_value_size;
    }
    ++len;
    update_len();
    return *this;
}
BigInt::BigInt(const string& old,int num_base/* = 10*/){
    init(old.length());
    BigInt tmp = 0;
    BigInt base =  BigInt(num_base);
    for(int i=0;i<old.length();++i){
        tmp = tmp * base + BigInt((int)char_code(old[i]));
    }
    *this = tmp;
}

BigInt::~BigInt(){
    if(values){
        delete [] values;
        len = 1;
        values_size = 1;
    }
}

BigInt extEclid(BigInt a,BigInt b, BigInt &x, BigInt & y , BigInt mod){
    if(a == zero){
        x = 0;y=1;
        return b;
    }
    BigInt x1,y1;
    BigInt d = extEclid(b%a,a,x1,y1,mod);
    x = ((mod*mod+y1) - (b/a)*x1)%mod;
    y = x1;
    return d;
}


BigInt Montgomeri(BigInt a,BigInt b,BigInt n ){

    if(n%two == 0){
        cout <<"n mBigIntst be odd\n";
        return 0;
    }
    BigInt r = 1;
    while(r<n)
        r*=2;

    BigInt r_inv , n_;
    extEclid(r,n,r_inv,n_,n);

    BigInt k = (r*r_inv - one)/n;

    BigInt a_ = (a*r)%n;
    BigInt b_  = (b*r)%n;

    BigInt x = (a_ * b_)%n;
    BigInt s = (x*k)%r;
    BigInt t = x+s*n;
    BigInt u = t/r;
    BigInt c_;
    if(u<n)c_ = u;
    else c_ = u - n;
    BigInt c = (c_  * r_inv)%n;
    return c;
}

BigInt REDC(BigInt T,BigInt R,BigInt N){
    // find t *r^-1 mod n
    BigInt n_ = R - rev(N,R);
    BigInt m = ((T % R)*n_)%R;
    BigInt t = (T+ m*N)/R;
    if(t>=N)
        t-=N;

    return t;
}


BigInt binpow(BigInt x,BigInt pow,BigInt mod){
    BigInt r = 1;

    //   cout <<"ST";

    while(pow>zero){
        if(pow%two==one)
            r = (r*x)%mod;


        //       cout << "\n";
        //       x.print();
        //       (x*x).print();
        //       ((x*x)%mod).print();
        //       cout << "\n";

        x = (x*x)%mod;
        pow=pow.div2();

        //        cout << "ZZZ";x.print();
        //        cout << "ZZZ";r.print();
        //        cout << "ZZZ";pow.print();cout<<"\n";

    }

    // cout <<"FIN\n";

    return r;
}


BigInt binpow_mont(BigInt x,BigInt pow,BigInt mod){
    BigInt r = 1;
    while(pow>zero){
        if(pow%two==one)
            r = Montgomeri(r,x,mod);
        x = Montgomeri(x,x,mod);
        pow=pow/two;
    }
    return r;
}

BigInt rev(BigInt x,BigInt mod){
    return binpow(x,mod-BigInt(2),mod);
}

BigInt get_random_big_int(int bytes){
    BigInt res = 0;
    for(int i=0;i<(bytes+ type_value_size/8 - 1)/(type_value_size/8);++i){
        res = res * BigInt((unsigned int)mod_part) + BigInt((unsigned int)rand()%mod_part);
    }
    return res;
}

bool is_prime(BigInt val){

    for(int i=0;i<10;++i){
        BigInt a = get_random_big_int(val.get_len());
        //a.print();
        if(a >= val || a == BigInt(0)){
            while(a>=val){
                a = a.div2();
            }
            if( a == BigInt(0)){
                --i;continue;
            }
        }
        if(! (binpow(a, val - one,val) == one)){
            return false;
        }
    }
    return true;
}

BigInt get_random_prime(int bytes){
    //"203391339108018642348853042914050827989603460621" one of primes found for 80 bit
    BigInt q = get_random_big_int(bytes);
    if(q%two == BigInt(0))
        q += one;

    while(!is_prime(q)){
        q += two;

    }
    return q;
}

BigInt get_random_lesser(const BigInt& x){
    int bytes = x.get_len();
    BigInt res(0);
    do{
        res = get_random_big_int(bytes);
    }while(res >= x);
    return res;
}

BigInt gcd(BigInt x,BigInt y){
    while( !(x == zero) && !(y == zero)){
        x = x%y;
        if( !(y== zero) ){
            y = y%x;
        }
    }
    return x+y;
}

BigInt extended_Euclid(BigInt a,BigInt b,BigInt &x,BigInt &y,BigInt& mod){
    if( a == zero ){
        x = zero;
        y = one;
        return b;
    }

    BigInt x1 = 0,
            y1 = 0;

    BigInt res = extended_Euclid(b%a,a,x1,y1,mod);
    x = (y1 + mod - ((b/a)*x1)%mod)%mod;
    y = x1;
    return res;
}

BigInt rev_Euclid(BigInt a,BigInt mod){
    BigInt x,y;

    BigInt z = extended_Euclid(a,mod,x,y,mod);
    if(!(z == one)){
        return zero;
    }
    return x;
}

