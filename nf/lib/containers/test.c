//@ #include "modulo.gh"
//@ #include "stdex.gh"
//@ #include "listexex.gh"
//@ #include <bitops.gh>
//@ #include <nat.gh>
//@ #include <listex.gh>

/*@

    // ------------- arithmetic -------------

    lemma void div_minus_one(int a, int b)
        requires    0 < a &*& 0 < b;
        ensures     (a*b - 1) / b == a - 1;
    {
        div_exact(a, b);
        div_exact(a - 1, b);
        div_lt(a*b - 1, a, b);
        div_ge((a-1)*b, a*b - 1, b);
    } 

    lemma void loop_fp_pop(int k, int capacity)
        requires    0 <= k &*& 0 < capacity;
        ensures     loop_fp(k, capacity) == k % capacity;
    {
        div_mod_gt_0(k%capacity, k, capacity);
        mod_rotate(k%capacity, capacity);
        mod_bijection(k%capacity, capacity);
    }

    lemma void mod_add_zero(int a, int b, int k)
        requires    0 <= a &*& 0 < b &*& 0 <= k;
        ensures     (a + b*k) % b == a % b;
    {
        loop_injection_n(a, b, k);
        loop_fp_pop(a + b*k, b);
        loop_fp_pop(a, b);
    }

    lemma void mod_rotate_mul(int a, int b)
        requires    0 <= a &*& 0 < b;
        ensures     ((a * b) % b) == 0;
    {
        div_exact(a, b);
        div_rem_nonneg(a * b, b);
    }

    lemma void pow_nat_div_rem(int x, nat n)
        requires    0 < x &*& n != zero;
        ensures     pow_nat(x, n) / x == pow_nat(x, nat_predecessor(n)) &*& pow_nat(x, n) % x == 0;
    {
        switch(n) {
            case zero:
            case succ(n_pred):
                mod_rotate_mul(pow_nat(x, n_pred), x);
                div_rem_nonneg(pow_nat(x, n), x);
        }
    }

    lemma void pow_nat_bounds(int x, nat n)
        requires    0 < x;
        ensures     0 < pow_nat(x, n);
    {
        switch(n) {
            case zero:
            case succ(n_pred): pow_nat_bounds(x, n_pred);
        }
    }

    // ------------- pow2 -------------

    fixpoint int pow2(nat m) { 
        return pow_nat(2, m); 
    }

    // ------------- sum_pow2 -------------


    fixpoint int sum_pow2(nat n) {
        switch(n) {
            case zero: return 1;
            case succ(n_pred): return pow_nat(2, n) + sum_pow2(n_pred);
        }
    }

    lemma void sum_pow2_pred(nat n)
        requires    n != zero;
        ensures     sum_pow2(n) == pow_nat(2, n) + sum_pow2(nat_predecessor(n));
    {
        switch(n) {
            case zero:
            case succ(n_pred): 
        }
    }

    lemma void sum_pow2_val(nat m)
        requires    m != zero;
        ensures     sum_pow2(nat_predecessor(m)) == pow_nat(2, m) - 1;
    {
        switch(m) {
            case zero:
            case succ(m_pred):
                if (m_pred == zero) {
                    assert (sum_pow2(m_pred) == 1);
                    assert (pow_nat(2, m) == 2);
                } else {
                    sum_pow2_val(m_pred);
                    sum_pow2_pred(m_pred);
                }
        }
    }

    // ------------- bitwise representation of capacity -------------

    lemma void bits_of_int_zero(nat n)
        requires    true;
        ensures     true == forall(snd(bits_of_int(0, n)), (eq)(false)) &*& fst(bits_of_int(0, n)) == 0;
    {
        switch(n) {
            case zero:
            case succ(n_pred):
                mod_rotate_mul(0, 2);
                div_rem_nonneg(0, 2);
                bits_of_int_zero(n_pred);
        }
    }

    lemma void int_of_bits_zero(list<bool> bits)
        requires    true == forall(bits, (eq)(false));
        ensures     int_of_bits(0, bits) == 0;
    {
        switch (bits) {
            case nil: 
            case cons(b, bs0): int_of_bits_zero(bs0);
        }
    }

    lemma void bits_of_int_remainder(int x, nat n)
        requires    0 <= x &*& x < pow2(n);
        ensures     fst(bits_of_int(x, n)) == 0;
    {
        switch (n) {
            case zero:
            case succ(n_pred): 
                assert (pow2(n) == 2 * pow2(n_pred));
                div_lt(x, pow2(n_pred), 2);
                div_exact(pow2(n_pred), 2);
                assert (x/2 < pow2(n_pred));
                div_ge(0, x, 2);
                division_round_to_zero(0, 2);
                bits_of_int_remainder(x/2, n_pred);
        }
    }

    lemma void bits_of_int_pow2_mask(nat n, nat m)
        requires    
            int_of_nat(m) <= int_of_nat(n);
        ensures     
            true == forall(take(int_of_nat(m), snd(bits_of_int(pow2(m)-1, n))), (eq)(true)) &*&
            true == forall(drop(int_of_nat(m), snd(bits_of_int(pow2(m)-1, n))), (eq)(false)) &*&
            0 == fst(bits_of_int(pow2(m)-1, n));
    {
        switch(m) {
            case zero: 
                bits_of_int_zero(n);
            case succ(m_pred):
                switch(n) {
                    case zero:
                    case succ(n_pred):
                        bits_of_int_pow2_mask(n_pred, m_pred);
                        assert (snd(bits_of_int(pow2(m)-1, n)) == cons( (pow2(m)-1)%2==1 , snd(bits_of_int((pow2(m)-1)/2, n_pred)) ) );

                        pow_nat_div_rem(2, m);
                        pow_nat_bounds(2, m_pred);
                        div_minus_one(pow2(m_pred), 2);
                        assert((pow2(m)-1)/2 == pow2(m_pred) - 1);

                        div_rem_nonneg(pow2(m), 2);
                        div_rem_nonneg(pow2(m) - 1, 2);
                        assert((pow2(m)-1)%2==1);
                }
        }
    }

    // ------------- k & (capacity - 1) -------------

    fixpoint list<bool> bits_of_int_and(list<bool> x_bits, list<bool> y_bits) {
        switch(x_bits) {
            case nil: return y_bits;
            case cons(x0, xs0): return switch(y_bits) {
                case nil: return x_bits;
                case cons(y0, ys0): return cons(x0 && y0, bits_of_int_and(xs0, ys0));
            };
        }
    }


    lemma void length_bits_of_int_and(list<bool> x_bits, list<bool> y_bits)
        requires    true;
        ensures     length(bits_of_int_and(x_bits, y_bits)) == ((length(x_bits) < length(y_bits)) ? length(y_bits) : length(x_bits));
    {
        switch(x_bits) {
            case nil: 
            case cons(x0, xs0):
                switch(y_bits) {
                    case nil: 
                    case cons(y0, ys0): length_bits_of_int_and(xs0, ys0);
                }
        }
    }

    lemma_auto(length(snd(bits_of_int(x, n)))) void length_bits_of_int(int x, nat n)
        requires    true;
        ensures     length(snd(bits_of_int(x, n))) == int_of_nat(n);
    {
        switch(n) {
            case zero:
            case succ(n_pred): length_bits_of_int(x/2, n_pred);
        }
    }

    lemma void bits_of_int_and_zero(list<bool> x_bits, list<bool> y_bits)
        requires    length(x_bits) == length(y_bits) &*& true == forall(y_bits, (eq)(false));
        ensures     true == forall(bits_of_int_and(x_bits, y_bits), (eq)(false));
    {
        switch(x_bits) {
            case nil:
            case cons(x0, xs0):
                switch(y_bits) {
                    case nil:
                    case cons(y0, ys0): bits_of_int_and_zero(xs0, ys0);
                }
        }
    }

    lemma void bits_of_int_and_mask(list<bool> k_bits, list<bool> mask_bits, int m)
        requires    
            length(k_bits) == length(mask_bits) &*&
            true == forall(take(m, mask_bits), (eq)(true)) &*&
            true == forall(drop(m, mask_bits), (eq)(false)) &*&
            0 <= m &*& m < length(k_bits);
        ensures
            take(m, k_bits) == take(m, bits_of_int_and(k_bits, mask_bits)) &*&
            true == forall(drop(m, bits_of_int_and(k_bits, mask_bits)), (eq)(false));
    {
        switch(k_bits) {
            case nil:
            case cons(k0, ks0):
                switch(mask_bits) {
                    case nil:
                    case cons(m0, ms0):
                        if (m > 0) {
                            bits_of_int_and_mask(ks0, ms0, m - 1);
                        } else {
                            bits_of_int_and_zero(k_bits, mask_bits);
                        }
                }
        }
    }

    lemma void int_of_Z_of_bits(list<bool> bits)
        requires    true;
        ensures     int_of_Z(Z_of_bits(Zsign(false), bits)) == int_of_bits(0, bits);
    {
        switch(bits) {
            case nil:
            case cons(b, bs0): int_of_Z_of_bits(bs0);
        }
    }

    lemma void bits_of_int_of_bits(list<bool> bits, nat n)
        requires    int_of_nat(n) == length(bits);
        ensures     bits == snd(bits_of_int(int_of_bits(0, bits), n));
    {
        switch(bits) {
            case nil:
            case cons(b, bs0):
                switch(n) {
                    case zero:
                    case succ(n_pred): 

                        int int_bits = int_of_bits(0, bits);
                        int int_bs0 = int_of_bits(0, bs0);
                        assert (int_bits == 2 * int_bs0 + (b ? 1 : 0));
                        
                        list<bool> bits_int_bits = snd(bits_of_int(int_bits, n));
                        list<bool> bits_int_bs0 = snd(bits_of_int(int_bs0, n_pred));
                        bits_of_int_of_bits(bs0, n_pred);
                        assert (bs0 == bits_int_bs0);

                        if (!b) {
                            assume (int_bs0 == int_bits/2);
                            assert (bits_int_bits == cons(int_bits%2==1, bits_int_bs0));
                            assume (int_bits%2==0);
                        } else {
                            assume (int_bs0 == int_bits/2);
                            assert (bits_int_bits == cons(int_bits%2==1, bits_int_bs0));   
                            assume (int_bits%2==1);                   
                        }
                }
        }
    }

    lemma void Z_bits_of_int_and_equiv(list<bool> xs, list<bool> ys)
        requires    length(xs) == length(ys);
        ensures     Z_and(Z_of_bits(Zsign(false), xs), Z_of_bits(Zsign(false), ys)) == Z_of_bits(Zsign(false), bits_of_int_and(xs, ys));
    {
        switch(xs) {
            case nil: length_0_nil(ys);
            case cons(x0, xs0):
                switch(ys) {
                    case nil:
                    case cons(y0, ys0): Z_bits_of_int_and_equiv(xs0, ys0);
                }
        }
    }

    lemma void bits_of_int_and_def(int x, list<bool> x_bits, int y, list<bool> y_bits, nat n)
        requires 
            bits_of_int(x, n) == pair(0, x_bits) &*& 
            bits_of_int(y, n) == pair(0, y_bits) &*& 
            0 <= x &*& x < pow2(n) &*& 0 <= y &*& y < pow2(n); 
        ensures 
            (x & y) == int_of_bits(0, bits_of_int_and(x_bits, y_bits)); 
    {
        Z_of_uintN(x, n);
        Z_of_uintN(y, n);
        bitand_def(x, Z_of_bits(Zsign(false), x_bits), y, Z_of_bits(Zsign(false), y_bits));

        length_bits_of_int(x, n);
        length_bits_of_int(y, n);
        Z_bits_of_int_and_equiv(x_bits, y_bits);

        int_of_Z_of_bits(bits_of_int_and(x_bits, y_bits));
    }

    lemma void bits_of_int_apply_mask(int k, list<bool> k_bits, int mask, list<bool> mask_bits, int m, nat n)
        requires   
            bits_of_int(k, n) == pair(0, k_bits) &*& 
            bits_of_int(mask, n) == pair(0, mask_bits) &*& 
            true == forall(take(m, mask_bits), (eq)(true)) &*&
            true == forall(drop(m, mask_bits), (eq)(false)) &*&
            0 <= k &*& k < pow2(n) &*& 0 <= mask &*& mask < pow2(n) &*&
            0 <= m &*& m < int_of_nat(n);
        ensures
            take(m, snd(bits_of_int(k & mask, n))) == take(m, k_bits) &*&
            true == forall(drop(m, snd(bits_of_int(k & mask, n))), (eq)(false));
    {
    	bits_of_int_and_def(k, k_bits, mask, mask_bits, n);
    	assert ( (k & mask) == int_of_bits(0, bits_of_int_and(k_bits, mask_bits)) );

        length_bits_of_int(k, n);
        length_bits_of_int(mask, n);
        bits_of_int_and_mask(k_bits, mask_bits, m); 

        length_bits_of_int_and(k_bits, mask_bits);
        bits_of_int_of_bits(bits_of_int_and(k_bits, mask_bits), n);
    }

    // ------------- k % capacity -------------

    lemma void bits_of_int_split(int k, nat n, int m, list<bool> k_bits, list<bool> l_bits, list<bool> r_bits)
        requires
            snd(bits_of_int(k, n)) == k_bits &*&
            0 <= k &*& k < pow2(n) &*&
            0 <= m &*& m < int_of_nat(n) &*&
            length(l_bits) == length(r_bits) &*& length(r_bits) == length(k_bits) &*&
            take(m, k_bits) == take(m, l_bits) &*& true == forall(drop(m, l_bits), (eq)(false)) &*&
            drop(m, k_bits) == drop(m, r_bits) &*& true == forall(take(m, r_bits), (eq)(false));
        ensures
            k == int_of_bits(0, l_bits) + int_of_bits(0, r_bits);
    {
        switch (n) {
            case zero:
            case succ(n_pred):
                switch(k_bits) {
                    case nil:
                    case cons(k0, ks0):
                        switch(l_bits) {
                            case nil:
                            case cons(l0, ls0):
                                switch(r_bits) {
                                    case nil:
                                    case cons(r0, rs0):
                                        if (m > 0) {
                                            div_lt(k, pow2(n_pred), 2);
                                            div_exact(pow2(n_pred), 2);
                                            assert(k/2 < pow2(n_pred));
                                            div_ge(0, k, 2);
                                            division_round_to_zero(0, 2);
                                            bits_of_int_split(k/2, n_pred, m-1, ks0, ls0, rs0);

                                            assert (r0 == false);
                                            assert (int_of_bits(0, r_bits) == 2 * int_of_bits(0, rs0));
                                            div_rem_nonneg(k, 2);
                                        } else {
                                            assert (true == forall(l_bits, (eq)(false)));
                                            assert (k_bits == r_bits);
                                            int_of_bits_zero(l_bits);
                                            bits_of_int_remainder(k, n);
                                            int_of_bits_of_int(k, n);
                                        }
                                }
                        }
                }
        }
    }

    lemma void int_of_bits_bounds(list<bool> bits)
        requires    true;
        ensures     0 <= int_of_bits(0, bits);
    {
        switch (bits) {
            case nil: 
            case cons(b, bs0): int_of_bits_bounds(bs0);
        }
    }

    lemma void int_of_bits_lt(list<bool> bits, nat m)
        requires    
            0 <= int_of_nat(m) &*& int_of_nat(m) < length(bits) &*& 
            true == forall(drop(int_of_nat(m), bits), (eq)(false));
        ensures     
            int_of_bits(0, bits) < pow2(m);
    {
        switch(bits) {
            case nil:
            case cons(b, bs0):
                switch(m) {
                    case zero:
                        pow_nat_bounds(2, m);
                        int_of_bits_zero(bits);
                    case succ(m_pred):
                        int_of_bits_lt(bs0, m_pred);
                } 
        }
    }

    lemma void int_of_bits_mul(list<bool> bits, nat m)
        requires
            0 <= int_of_nat(m) &*& int_of_nat(m) < length(bits) &*& 
            true == forall(take(int_of_nat(m), bits), (eq)(false));
        ensures
            int_of_bits(0, bits) % pow2(m) == 0;
    {
        switch(bits) {
            case nil:
            case cons(b, bs0):
                switch(m) {
                    case zero:
                        int_of_bits_bounds(bits);
                        assert (pow2(zero) == 1);
                        div_mod_gt_0(int_of_bits(0, bits) % pow2(m), int_of_bits(0, bits), pow2(m));
                    case succ(m_pred):
                        int_of_bits_mul(bs0, m_pred);
                        assert (b == false);
                        assert (int_of_bits(0, bits) == 2 * int_of_bits(0, bs0));
                        assert (pow2(m) == 2 * pow2(m_pred));
                        assert (int_of_bits(0, bs0) % pow2(m_pred) == 0);                        
                } 
        }
    }

@*/


unsigned loop(unsigned k, unsigned capacity)
//@     requires 0 < capacity &*& capacity < INT_MAX;
//@     ensures 0 <= result &*& result < capacity &*& result == loop_fp(k, capacity);
{
    //@ nat m;
    //@ int m_int = int_of_nat(m);
    //@ assume (m_int < 32);
    //@ assume (capacity == pow2(m));
    //@ assert (capacity < pow2(N32));
    //@ assert (k < pow2(N32));

    //@ list<bool> k_bits = snd(bits_of_int(k, N32));
    //@ list<bool> capacity_bits = snd(bits_of_int(capacity - 1, N32));

    // Proof for k & (capacity - 1)
    //@ bits_of_int_pow2_mask(N32, m);

    //@ bits_of_int_remainder(k, N32);
    //@ bits_of_int_remainder(capacity - 1, N32);
    //@ bits_of_int_apply_mask(k, k_bits, capacity - 1, capacity_bits, m_int, N32);

    //@ bitand_limits(k, capacity - 1, N32);
    //@ bits_of_int_remainder(k & (capacity - 1), N32);

    // Proof for k % capacity

    //@ assert ((k % capacity) == (k & (capacity - 1)) );

    //@ loop_fp_pop(k, capacity);
    //@ assert ((k % capacity) == loop_fp(k, capacity));

    return k & (capacity - 1);
}