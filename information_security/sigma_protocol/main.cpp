#include <iostream>
#include "sha/mac.h"
#include "aes/aes_func.h"
#include "dsa/rsa.h"
#include "dsa/bigint.h"
#include "dsa/dsa.h"
#include "eliptic_binary/point.h"

using namespace std;




string points_to_str(Point a,Point b){
    string inp =  a.to_string() + "#" + b.to_string();
    return inp;
}

string get_mac_local(string val,string key){

}

string get_pseudo_rand_func(string val, string seed){

    return ((BigInt(val,16)*BigInt(seed,16) + BigInt(12345) ) / (BigInt(43321))).to_string();
}


string get_rand_seed1(string val){
    return val;
}

string get_rand_seed2(string val){
    return (BigInt(val) * BigInt("1325524", 16)).to_string();
}

string get_rand_seed3(string val){
    return (BigInt(val) * BigInt("1325524", 8)).to_string();
}

string get_rand_seed4(string val){
    return (BigInt(val) * BigInt("1325524", 7)).to_string();
}



string pad_mess_to_len(string s,int len = 256){
    while(s.length() < len){
        s = '0' + s;
    }
    return s;
}


Point point_from_str(string s){
    s = s.substr(s.length() - 2*BinaryPolynomial::n, 2 * BinaryPolynomial::n);
    BinaryPolynomial x(s.substr(0,BinaryPolynomial::n));
    BinaryPolynomial y(s.substr(0,BinaryPolynomial::n));
    return Point(x,y);
}


string get_aes_key(string input, int key_len = 48){
    if ( input.length() >= key_len){
        input  = input.substr(0,key_len);
    }
    while(input.length() < key_len){
        input += '0';
    }
    return input;
}


void sigma_basic(){
    BigInt A(string("34325f7edfe4354545435"),16);
    BigInt B(string("576f789785980958076544"),16);


    // eliptic curve point

    Point g = generate_point(); // A & B


    BigInt x = get_random_big_int(5); // Alice
    BigInt y = get_random_big_int(6); // Bob



    //Alice Block 1
    Point gx = g * x; // A -> B

    cerr << " send  : A -> B : g^x = \n" << gx <<"\n";
    // A -> B   { gx }



    //Bob Block 1
    Point gy = g * y; // B -> A
    string points_str = points_to_str(gx,gy); // B -> A
    string points_hash = SHA2::SHA2_main(points_str);
    BigInt coded_,r_,s_,p_,q_,g_,y_; // coded and public signature parametrs
    dsa_sign(points_hash, B.to_string(), coded_,r_,s_,p_,q_,g_,y_);
    string points_sign = coded_.to_string();
    Point gxy = gx * y;
    string ps_rand_seed_b = BigInt(gxy.to_string(),2).to_string();
    string km_b = get_pseudo_rand_func(B.to_string(),ps_rand_seed_b);
    string bmac = MAC::get_hex_mac(B.to_string(),km_b);

    cout << "Signature " << points_sign  << "\n";
    cout << "Seed = " << ps_rand_seed_b << "\n";
    cout << "Km = " << km_b <<"\n";

    // B -> A { gy, B, points_sign, bmac }
    cerr << " send  : B -> A : g^y = \n" << gy <<"\n";
    cerr << "B  = " << B.to_string() <<"\n";
    cerr << "Sig_b(gx,gy) = " << points_sign <<"\n";
    cerr << "mac _ km (B) = " << bmac << "\n";


    //Alice Block 2
    Point gyx = gy * x;
    cout << "A and B got the same point ? " << ((gxy == gyx) ? "True\n":"False\n");
    string ps_rand_seed_a = BigInt(gyx.to_string(),2).to_string();
    string km_a  = get_pseudo_rand_func(A.to_string(), ps_rand_seed_a);

    cout << "Alice check bmac : " << (bmac ==  MAC::get_hex_mac(B.to_string(),get_pseudo_rand_func(B.to_string(), ps_rand_seed_a)) ? "True\n":"False\n");
    cout << "Alice check sign : " << (dsa_check(BigInt(points_sign,16),r_,s_,p_,q_,g_,y_) ? "True\n":"False\n");


    points_str = points_to_str(gy,gx);
    points_hash = SHA2::SHA2_main(points_str);
    dsa_sign(points_hash, A.to_string(), coded_,r_,s_,p_,q_,g_,y_);
    points_sign = coded_.to_string();
    string amac = MAC::get_hex_mac(A.to_string(),km_a);
    cout << "Signature " << points_sign  << "\n";
    cout << "Seed = " << ps_rand_seed_a << "\n";
    cout << "Km = " << km_a <<"\n";


    cerr << " send  : A -> B : \n";
    cerr << "A  = " << A.to_string() <<"\n";
    cerr << "Sig_a(gy,gx) = " << points_sign <<"\n";
    cerr << "mac _ km (A) = " << amac << "\n";


    // A -> B { A, points_sign, amac }

    cout << "Bob check amac : " << (amac ==  MAC::get_hex_mac(A.to_string(),get_pseudo_rand_func(A.to_string(), ps_rand_seed_b)) ? "True\n":"False\n");
    cout << "Bob check sign : " << (dsa_check(BigInt(points_sign,16),r_,s_,p_,q_,g_,y_) ? "True\n":"False\n");
    //Bob Block 2

}

string isTrue(bool x){
    return (x?"True\n":"False\n");
}

void sigma_I(){
    BigInt A(string("34325f7edfe4354545435"),16);
    BigInt B(string("576f789785980958076544"),16);


    // eliptic curve point

    Point g = generate_point(); // A & B


    BigInt x = get_random_big_int(5); // Alice
    BigInt y = get_random_big_int(6); // Bob



    //Alice Block 1
    Point gx = g * x; // A -> B

    cerr << " send  : A -> B : g^x = \n" << gx <<"\n";
    // A -> B   { gx }


    //Bob Block 1
    Point gy = g * y; // B -> A
    string points_str = points_to_str(gx,gy); // B -> A
    string points_hash = SHA2::SHA2_main(points_str);
    BigInt coded_,r_,s_,p_,q_,g_,y_; // coded and public signature parametrs
    dsa_sign(points_hash, B.to_string(), coded_,r_,s_,p_,q_,g_,y_);
    string points_sign = coded_.to_string();
    Point gxy = gx * y;
    string ps_rand_seed_b = BigInt(gxy.to_string(),2).to_string();
    string ps_rand_seed_b2 = get_rand_seed2(BigInt(gxy.to_string(),2).to_string());

    string km_b = get_pseudo_rand_func(B.to_string(),ps_rand_seed_b);
    string bmac = MAC::get_hex_mac(B.to_string(),km_b);

    string ke_b = get_aes_key(get_pseudo_rand_func(gxy.to_string(),ps_rand_seed_b2));

    string concat = pad_mess_to_len(B.to_string()) + pad_mess_to_len(points_sign) + pad_mess_to_len(bmac);
    string coded_concat = AES_enc(concat,ke_b);

    cout << "Signature " << points_sign  << "\n";
    cout << "Seed = " << ps_rand_seed_b << ", " << ps_rand_seed_b2 << "\n";
    cout << "Km = " << km_b <<"\n";
    cout << "Ke_b = " << ke_b << "\n";
    cout << "Concat = " << concat << "\n";
    cerr << " send  : B -> A : g^y = \n" << gy <<"\n" << "coded = " << coded_concat;

    //Alice Block 2
    Point gyx = gy * x;
    cout << "A and B got the same point ? " << ((gxy == gyx) ? "True\n":"False\n");
    string ps_rand_seed_a = BigInt(gyx.to_string(),2).to_string();
    string ps_rand_seed_a2 = get_rand_seed2(BigInt(gxy.to_string(),2).to_string());
    string ke_a = get_aes_key(get_pseudo_rand_func(gyx.to_string(),ps_rand_seed_b2));
    string km_a  = get_pseudo_rand_func(A.to_string(), ps_rand_seed_a);
    string decoded_concat = AES_dec(coded_concat,ke_a);
    string B_a = BigInt(decoded_concat.substr(0,256),16).to_string();
    string points_sign_a = BigInt(decoded_concat.substr(256,256),16).to_string();
    string bmac_a = BigInt(decoded_concat.substr(512,256),16).to_string();

    cout << "Decoded concat = " << decoded_concat <<"\n";
    cout << "B decoded " << isTrue(B.to_string() == B_a) <<  "SIG decoded " << isTrue(points_sign == points_sign_a) << "bmac decoded " <<isTrue(bmac == bmac_a);
    cout << "Alice check bmac : " << (bmac_a ==  MAC::get_hex_mac(B.to_string(),get_pseudo_rand_func(B.to_string(), ps_rand_seed_a)) ? "True\n":"False\n");
    cout << "Alice check sign : " << (dsa_check(BigInt(points_sign_a,16),r_,s_,p_,q_,g_,y_) ? "True\n":"False\n");


    points_str = points_to_str(gy,gx);
    points_hash = SHA2::SHA2_main(points_str);
    dsa_sign(points_hash, A.to_string(), coded_,r_,s_,p_,q_,g_,y_);
    points_sign = coded_.to_string();
    string amac = MAC::get_hex_mac(A.to_string(),km_a);
    concat = pad_mess_to_len(A.to_string()) + pad_mess_to_len(points_sign) + pad_mess_to_len(amac);
    coded_concat = AES_enc(concat,ke_a);

    cout << "Signature " << points_sign  << "\n";
    cout << "Seed = " << ps_rand_seed_a << "\n";
    cout << "Km = " << km_a <<"\n";
    cerr << " send  : A -> B : \n";
    cerr << "Coded concat " << coded_concat <<"\n";
//    cerr << "A  = " << A.to_string() <<"\n";
//    cerr << "Sig_a(gy,gx) = " << points_sign <<"\n";
//    cerr << "mac _ km (A) = " << amac << "\n";


    // A -> B { coded_concat }

    //Bob Block 2
    decoded_concat = AES_dec(coded_concat,ke_a);
    string A_b = BigInt(decoded_concat.substr(0,256),16).to_string();
    string points_sign_b = BigInt(decoded_concat.substr(256,256),16).to_string();
    string amac_b = BigInt(decoded_concat.substr(512,256),16).to_string();
    cout << "A decoded " << isTrue(A.to_string() == A_b) <<  "SIG decoded " << isTrue(points_sign == points_sign_b) << "amac decoded " <<isTrue(amac == amac_b);
    cout << "Bob check amac : " << (amac_b ==  MAC::get_hex_mac(A.to_string(),get_pseudo_rand_func(A.to_string(), ps_rand_seed_b)) ? "True\n":"False\n");
    cout << "Bob check sign : " << (dsa_check(BigInt(points_sign_b,16),r_,s_,p_,q_,g_,y_) ? "True\n":"False\n");

}

void sigma_R(){
    BigInt A(string("34325f7edfeff4645354545435"),16);
    BigInt B(string("576f78ff5679785980958076544"),16);
    string kma_val = "314345256246";
    string kmb_val = "5678675345643";

    // eliptic curve point

    Point g = generate_point(); // A & B


    BigInt x = get_random_big_int(5); // Alice
    BigInt y = get_random_big_int(6); // Bob



    //Alice Block 0
    Point gx = g * x; // A -> B

    cerr << " send  : A -> B : g^x = \n" << gx <<"\n";
    // A -> B   { gx }


    //Bob Block 0
    Point gy = g * y; // B -> A


    //Alice Block 1
    Point gyx = gy * x;
    string ps_rand_seed_a_a = get_rand_seed1(BigInt(gyx.to_string(),2).to_string());
    string ps_rand_seed_a2_a = get_rand_seed2(BigInt(gyx.to_string(),2).to_string());
    string ps_rand_seed_b_a = get_rand_seed3(BigInt(gyx.to_string(),2).to_string());
    string ps_rand_seed_b2_a = get_rand_seed3(BigInt(gyx.to_string(),2).to_string());
    string ke_a_a = get_aes_key(get_pseudo_rand_func(gyx.to_string(),ps_rand_seed_a_a));
    string ke_b_a = get_aes_key(get_pseudo_rand_func(gyx.to_string(),ps_rand_seed_b_a));

    string km_a_a  = get_pseudo_rand_func(kma_val, ps_rand_seed_a2_a);
    string km_b_a  = get_pseudo_rand_func(kmb_val, ps_rand_seed_b2_a);


    //Bob Block 1

    Point gxy = gx * y;
    string ps_rand_seed_a_b = get_rand_seed1(BigInt(gxy.to_string(),2).to_string());
    string ps_rand_seed_a2_b = get_rand_seed2(BigInt(gxy.to_string(),2).to_string());
    string ps_rand_seed_b_b = get_rand_seed3(BigInt(gxy.to_string(),2).to_string());
    string ps_rand_seed_b2_b = get_rand_seed3(BigInt(gxy.to_string(),2).to_string());
    string ke_a_b = get_aes_key(get_pseudo_rand_func(gxy.to_string(),ps_rand_seed_a_b));
    string ke_b_b = get_aes_key(get_pseudo_rand_func(gxy.to_string(),ps_rand_seed_b_b));

    string km_a_b  = get_pseudo_rand_func(kma_val, ps_rand_seed_a2_b);
    string km_b_b  = get_pseudo_rand_func(kmb_val, ps_rand_seed_b2_b);


    string points_str = points_to_str(gx,gy); // B -> A
    string points_hash = SHA2::SHA2_main(points_str);
    BigInt coded_,r_,s_,p_,q_,g_,y_; // coded and public signature parametrs
    dsa_sign(points_hash, B.to_string(), coded_,r_,s_,p_,q_,g_,y_);
    string points_sign = coded_.to_string();

    string bmac = MAC::get_hex_mac(B.to_string(),km_b_b);

    string concat = pad_mess_to_len(B.to_string()) + pad_mess_to_len(points_sign) + pad_mess_to_len(bmac);
    string coded_concat = AES_enc(concat,ke_b_b);

    cout << "\nA and B got the same point ? " << ((gxy == gyx) ? "True\n":"False\n");
    cout << "Signature " << points_sign  << "\n";
    cout << "Seed = " << ps_rand_seed_b_b << ", " << ps_rand_seed_b2_b << "\n";
    cout << "Km = " << km_b_b <<"\n";
    cout << "Ke_b = " << ke_b_b << "\n";
    cout << "Concat = " << concat << "\n";
    cerr << " send  : B -> A : g^y = \n" << gy <<"\n" << "coded = " << coded_concat <<"\n";

    //Alice Block 2


    string decoded_concat = AES_dec(coded_concat,ke_b_a);
    string B_a = BigInt(decoded_concat.substr(0,256),16).to_string();
    string points_sign_a = BigInt(decoded_concat.substr(256,256),16).to_string();
    string bmac_a = BigInt(decoded_concat.substr(512,256),16).to_string();

    cout << "Decoded concat = " << decoded_concat <<"\n";
    cout << "B decoded " << isTrue(B.to_string() == B_a) <<  "SIG decoded " << isTrue(points_sign == points_sign_a) << "bmac decoded " <<isTrue(BigInt(bmac,16).to_string() == bmac_a);


    cout << "Alice check bmac : " << isTrue(bmac_a == BigInt(MAC::get_hex_mac(B.to_string(),km_b_a),16).to_string());
    cout << "Alice check sign : " << isTrue(dsa_check(BigInt(points_sign_a,16),r_,s_,p_,q_,g_,y_));


    points_str = points_to_str(gy,gx);
    points_hash = SHA2::SHA2_main(points_str);
    dsa_sign(points_hash, A.to_string(), coded_,r_,s_,p_,q_,g_,y_);
    points_sign = coded_.to_string();
    string amac = MAC::get_hex_mac(A.to_string(),km_a_a);
    concat = pad_mess_to_len(A.to_string()) + pad_mess_to_len(points_sign) + pad_mess_to_len(amac);
    coded_concat = AES_enc(concat,ke_a_a);

    cout << "Signature " << points_sign  << "\n";
    cout << "Seed = " << ps_rand_seed_a_a << " " << ps_rand_seed_a2_a << "\n";
    cout << "Km = " << km_a_a <<"\n";
    cerr << " send  : A -> B : \n";
    cerr << "Coded concat " << coded_concat <<"\n";
    //    cerr << "A  = " << A.to_string() <<"\n";
    //    cerr << "Sig_a(gy,gx) = " << points_sign <<"\n";
    //    cerr << "mac _ km (A) = " << amac << "\n";


    // A -> B { coded_concat }

    //Bob Block 2
    decoded_concat = AES_dec(coded_concat,ke_a_b);
    string A_b = BigInt(decoded_concat.substr(0,256),16).to_string();
    string points_sign_b = BigInt(decoded_concat.substr(256,256),16).to_string();
    string amac_b = BigInt(decoded_concat.substr(512,256),16).to_string();
    cout << "A decoded " << isTrue(A.to_string() == A_b) <<  "SIG decoded " << isTrue(points_sign == points_sign_b) << "amac decoded " <<isTrue( BigInt(amac,16).to_string() == amac_b);
    cout << "Bob check amac : " << isTrue(amac_b ==  BigInt(MAC::get_hex_mac(A.to_string(),km_a_b),16).to_string());
    cout << "Bob check sign : " << isTrue(dsa_check(BigInt(points_sign_b,16),r_,s_,p_,q_,g_,y_));

}



int main()
{

    sigma_R();

    return 0;
}
