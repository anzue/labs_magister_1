
#include "bigint.h"



void BigInt::init(int size){
    if(DEBUG)cerr <<"init" << std::endl;

    if(values){
        delete values;
    }
    values = new Value_type[size];
    values_size = size;
    len = 1;
}

BigInt::BigInt(Value_type val){
    if(DEBUG)cerr <<"BigInt1 " << val<< std::endl;

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
    if(DEBUG)cerr <<"BigInt2 " << old.len << " "<<old.values_size<< std::endl;

    len = old.len;
    values_size = old.values_size;
    values = new Value_type[values_size];
    memcpy(values,old.values,len*sizeof(Value_type));
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
    if(DEBUG)cerr <<"upscale" << std::endl;

    if(new_size > values_size){
        if(new_size > values_size + values_size)
            resize(new_size);
        else
            resize(values_size+values_size);
    }
}

void BigInt::print(ostream& str/* = cout*/, PrintType type /*= PrintType::DEC*/,int padding/* = 0*/)const{
    //if(DEBUG)cerr <<"Print" << std::endl;


    if(DEBUG)str << "len = " << len << ", arr_size = " << values_size << " ";

    if(padding == 0){
        padding = len;
    }
    --padding;
    if(type == BIN){
        for(int i=padding;i>=0;--i)
            str << std::bitset<type_value_size>(get_value_at(i)) << " ";
        str<<"\n";
        return;
    }
    if(type == DEC){
        stringstream out;
        BigInt tmp(*this);
        while( tmp > BigInt(0)){
            out << (tmp%BigInt(10)).values[0];
//            cout << "ZZ" <<  (tmp%BigInt(10)).values[0] << "\n";
//            cout << "ZZZ";tmp.print(cout, PrintType::HEX,2);
            tmp /= BigInt(10);
//            cout << "ZZZ";tmp.print(cout, PrintType::HEX,2);
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
    for(int i=padding;i>=0;--i)
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
    len = old.len;
    values_size = old.values_size;
//    if(values){
//        delete values;
//    }
    values = new Value_type[values_size];
    memcpy(values,old.values,len*sizeof(Value_type));
    return *this;
}

BigInt BigInt::operator*=(const BigInt& a){
    if(DEBUG){cerr <<"*=" << std::endl;

        cout << "*=\n";
        print(std::cout,PrintType::HEX,2);
        a.print(std::cout,PrintType::HEX,2);
    }

    int res_len = ((len+1)*(a.len+1));
    BigInt res(0);
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
        l.print(cout, PrintType::HEX,2);
        m.print(cout, PrintType::HEX,2);
        r.print(cout, PrintType::HEX,2);
        a.print(cout, PrintType::HEX,2);
        (m*a).print(cout, PrintType::HEX,2);
        print(cout, PrintType::HEX,2);
        cout <<"\n";*/
        if ( m * a < *this ){
            l = m+one;
        }else{
            r = m;
        }
/*
        l.print(cout, PrintType::HEX,2);
        m.print(cout, PrintType::HEX,2);
        r.print(cout, PrintType::HEX,2);
        a.print(cout, PrintType::HEX,2);
        (m*a).print(cout, PrintType::HEX,2);
        print(cout, PrintType::HEX,2);
        cout <<"\n";*/

    }


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
        values[i]+=minus_one;
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

//    cout <<"V1 ";this->print(cout,PrintType::HEX,2);
//    cout <<"V2 ";(*this/a).print(cout,PrintType::HEX,2);
//    cout <<"V3 ";((*this/a)*a).print(cout,PrintType::HEX,2);
//    cout <<"V4 ";(*this - (*this/a)*a).print(cout,PrintType::HEX,2);
//    cout << "\n\n";
    return *this - (*this/a)*a;
}

bool BigInt::operator==(const int& a) const{
    return (len == 1 && values[0] == a) || (len == 2 && values[0] + values[1] << type_value_size == a);
}


BigInt BigInt::operator=(const string& old){
    *this = 0;
    for(int i=0;i<old.length();++i){
        *this = *this * BigInt(10) + BigInt(old[i] - '0');
    }
    return *this;
}

BigInt BigInt::operator=(const int& old){
    return *this = BigInt(old);
}


BigInt::BigInt(const string& old){
    *this = old;
}
