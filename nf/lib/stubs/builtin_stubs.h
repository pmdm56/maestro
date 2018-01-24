#pragma once

// Pretend that everything is known to be a compile-time constant, so DPDK uses less fancy tricks
//#define __builtin_constant_p(x) 1


// Standard CAS (but of course we don't need atomicity)
#define __sync_bool_compare_and_swap(ptr, oldval, newval) ((*ptr == oldval) ? (*ptr = newval, 1) : 0)


// DPDK only uses it as an atomic add, no fetch necessary
// TODO make it decent anyway, we shouldn't rely on that
#define __sync_fetch_and_add(ptr, value) (*(ptr) += (value))

// As the name implies, add then fetch
//#define __sync_add_and_fetch(ptr, value) (*(ptr) += (value))


// Despite it being called test_and_set, GCC docs describe it as "not a traditional test-and-set operation, but rather an atomic exchange operation"
static inline int stub_test_and_set(volatile int* ptr, int value)
{
	int prev = *ptr;
	*ptr = value;
	return prev;
}
#define __sync_lock_test_and_set(ptr, value) (stub_test_and_set(ptr,value))
