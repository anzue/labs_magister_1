
#include "bigint.h"

void BigInt::init(int size){
    if(DEBUG)cerr <<"init" << std::endl;
    if(size < 1)
        size = 1;
    values = new Value_type[size];
    values_size = size;
    values[0] = 0;
    len = 1;
}

BigInt::BigInt(const Value_type val){
    if(DEBUG)cerr <<"BigInt1 " << val << " " << this << std::endl;

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
    if(DEBUG){
        cerr <<"BigInt2 " << old.len << " "<<old.values_size<< std::endl;
        cerr << this <<"\n";
        //cerr << "old : ";old.print(std::cerr,PrintType::HEX,3);

    }
    len = old.len;
    values_size = len;
    values = new Value_type[len];
    if(old.values) // TODO why is this needed
        memcpy(values,old.values,len*sizeof(Value_type));

    if(DEBUG){
        cerr <<"BigInt2 " << old.len << " "<<old.values_size<< std::endl;
        cerr <<"BigInt2 " << values << " "<<values+len<< std::endl;
    }
}

void BigInt::resize(int new_size){
    if(DEBUG)cerr <<"resize" << std::endl;

    if(new_size < len){
        exit(2);
    }
    if(new_size <= 0){
        new_size = 1;
    }
    Value_type* new_arr = new Value_type[new_size];
    memcpy(new_arr,values,len*sizeof(Value_type));
    delete values;

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
    if(DEBUG){
        cerr <<"upscale" << std::endl;
        cerr <<"old arr" << values << " " << values + values_size << "\n";
    }
    if(new_size > values_size){
        if(new_size > values_size + values_size)
            resize(new_size);
        else
            resize(values_size+values_size);
    }
    if(DEBUG){
        cerr <<"new arr" << values << " " << values + values_size << "\n";
    }
}

void BigInt::print(ostream& str/* = cout*/, PrintType type /*= PrintType::DEC*/,int padding/* = 0*/)const{
    //if(DEBUG)cerr <<"Print" << std::endl;


    if(DEBUG)str << "len = " << len << ", arr_size = " << values_size << " ";

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
        str <<  setfill('0') << setw(4) << get_value_at(i) << " ";
    str<<"\n";
}

BigInt BigInt::operator+=(const BigInt& a){
    if(DEBUG)cerr <<"+=" << std::endl;
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

BigInt BigInt::operator=(const BigInt& old) {
    if(values){
        delete values;
    }
    len = old.len;
    values_size = len;
    values = new Value_type[len];
    if(old.values)
        memcpy(values,old.values,len*sizeof(Value_type));
    return *this;
}

BigInt BigInt::operator*=(const BigInt& a){
    if(DEBUG){cerr <<"*=" << std::endl;

        cout << "*=\n";
        print(std::cout,PrintType::HEX,2);
        a.print(std::cout,PrintType::HEX,2);
    }

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
    //res.print(std::cout, PrintType::HEX);std::cout << std::endl;
    res.update_len();

    if(DEBUG){
        std::cout << std::endl;print(std::cout, PrintType::HEX,2);
        cout << "*=";
        a.print(std::cout, PrintType::HEX,2);
        res.print(std::cout, PrintType::HEX,2);std::cout << std::endl;
    }
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
    if(DEBUG)cerr <<"Div2" << std::endl;
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

BigInt BigInt::operator/=(const BigInt& a){
    if(DEBUG){cerr <<"/=" << std::endl;
        cerr <<"/=" << std::endl;
        cout << "ww";this->print(std::cout,PrintType::HEX,2);
        cout << "ww";a.print(std::cout,PrintType::HEX,2);
    }
    BigInt l = 1,
            r = *this,
            m;
    BigInt one = BigInt(1);
    while( l + one < r){
        m = (l+r).div2();

        /*
        l.print(cout, PrintType::HEX,8);
        m.print(cout, PrintType::HEX,8);
        r.print(cout, PrintType::HEX,8);
        a.print(cout, PrintType::HEX,8);
        (m*a).print(cout, PrintType::HEX,8);
        print(cout, PrintType::HEX,8);
        cout <<"\n";
*/
        if ( m * a <= *this ){
            l = m+one;
        }else{
            r = m;
        }
        /*
        l.print(cout, PrintType::HEX,8);
        m.print(cout, PrintType::HEX,8);
        r.print(cout, PrintType::HEX,8);
        a.print(cout, PrintType::HEX,8);
        (m*a).print(cout, PrintType::HEX,8);
        print(cout, PrintType::HEX,8);
        cout <<"\n";
*/
    }
    /*
    l.print(cout, PrintType::HEX,8);
    a.print(cout, PrintType::HEX,8);
    (l*a).print(cout, PrintType::HEX,8);
    this->print(cout, PrintType::HEX,8);
    cout<<"\n";
*/
    if( l * a <= *this){

        //cout << "QQ";r.print(std::cout,PrintType::HEX,2);
        return *this = l;
    }
    //update_len();
    //cout << "QQ";l.print(std::cout,PrintType::HEX,2);
    return *this = l - BigInt(1);
}

BigInt BigInt::operator/(const BigInt& a) const {
    BigInt res(*this);
    return res/=a;
}

BigInt BigInt::operator-=(const BigInt& a){
    if(DEBUG) cerr <<"-=" << std::endl;

    if(*this <= a){
        return BigInt(0);
    }
    int minus_one = 0;
    for(int i=0;i< len;++i){
        values[i]-=minus_one;
        if(values[i] < a.get_value_at(i)){
            values[i]+= mod_part-a.get_value_at(i);
            minus_one = 1;
        }else{
            values[i]-=a.get_value_at(i);
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

    /*if (  (*this - (*this/a)*a) >= a){

        this->print(cout,PrintType::DEC);
        a.print(cout,PrintType::DEC);

        (*this/a).print(cout,PrintType::DEC);
        (*this/a*a).print(cout,PrintType::DEC);
        (*this - (*this/a)*a).print(cout,PrintType::DEC);

        cout << (*this - (*this/a)*a).len << " " << a.len <<"\n";

        cout<<"tt";  (*this - (*this/a)*a).print(cout,PrintType::DEC);
        cout<<"gg"; a.print(cout,PrintType::DEC);cout<<"\n";}*/

    return *this - (*this/a)*a;
}

bool BigInt::operator==(const Value_type a) const{
    return (len == 1 && values[0] == a) || (len == 2 && values[0] + values[1] << type_value_size == a);
}



BigInt BigInt::operator=(const string& old){
    *this = 0;
    for(int i=0;i<old.length();++i){
        *this = *this * BigInt(10) + BigInt(old[i] - '0');
    }
    return *this;
}

BigInt BigInt::operator=(const Value_type old){
    return *this = BigInt(old);
}


int char_code(char c){
    if( c>='0'&& c<='9')return c-'0';
    if( c>='a'&& c<='f')return c-'a'+10;
    if( c>='A'&& c<='F')return c-'A'+10;
    return 0;
}
BigInt BigInt::operator+=(const Value_type a){
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

        // BigInt v1 = BigInt(num_base);
        // BigInt v2 = BigInt(char_code(old[i]));
        tmp = tmp * base + BigInt((int)char_code(old[i]));
        // *this*=v1;
        // *this+=v2;
    }
    *this = tmp;
}

BigInt::~BigInt(){
    if(DEBUG){
        cerr << "~BigInt\n";
        cerr << len << " " << values_size << " " << values << " "<< values + 1 << " " <<"\n";
        if(values)print(std::cerr, PrintType::HEX,3);
        cerr << std::endl;

    }
    if(values){
        delete values;
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
    while(pow>zero){
        if(pow%two==1)
            r = (r*x)%mod;
        x = (x*x)%mod;
        pow=pow.div2();
        /*
        cout << "ZZZ";x.print();
        cout << "ZZZ";r.print();
        cout << "ZZZ";mod.print();cout<<"\n";*/

    }
    return r;
}


BigInt binpow_mont(BigInt x,BigInt pow,BigInt mod){
    BigInt r = 1;
    while(pow>zero){
        if(pow%two==1)
            r = Montgomeri(r,x,mod);
        x = Montgomeri(x,x,mod);
        pow=pow/two;
    }
    return r;
}

BigInt rev(BigInt x,BigInt mod){
    return binpow(x,mod-BigInt(2),mod);
}

