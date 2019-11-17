#include "point.h"

void diffie_hellman(){
    Point base = generate_point();
    Point m = generate_point();


    cout << "base = \n" << base << "\n";
    cout << "m = \n" << m << "\n";

    const size_t a_len = 100;
    const size_t b_len = 200;

    std::bitset<a_len> alice_key;
    std::bitset<b_len>   bob_key;

    get_random_bits<a_len>(alice_key);
    get_random_bits<b_len>(bob_key);


    Point a_base = (base.operator*<a_len>(alice_key));
    Point b_base = (base.operator*<b_len>(bob_key));

    cout << "Alice key = " << alice_key;
    cout << "\nAlice point = \n" << a_base;

    cout << "  Bob key = " << bob_key;
    cout << "\n  Bob point = \n" << b_base;


    Point b_a_base = (a_base.operator*<b_len>(  bob_key));
    Point a_b_base = (b_base.operator*<a_len>(alice_key));

    cout << "  Bob's coded point = \n" << b_a_base <<"\n";

    cout << "Alice's coded point = \n" << a_b_base <<"\n";


}
