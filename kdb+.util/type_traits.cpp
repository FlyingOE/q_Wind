#include "stdafx.h"
#include "type_traits.h"

#include "types.h"

#include <cmath>

q::type_traits<void>::value_type const q::type_traits<void>::NULL_VAL = K_NIL;
q::type_traits<C>::value_type const q::type_traits<C>::NULL_VAL = ' ';
q::type_traits<G>::value_type const q::type_traits<G>::NULL_VAL = 0x00;
q::type_traits<H>::value_type const q::type_traits<H>::NULL_VAL = nh;
q::type_traits<I>::value_type const q::type_traits<I>::NULL_VAL = ni;
q::type_traits<J>::value_type const q::type_traits<J>::NULL_VAL = nj;
q::type_traits<E>::value_type const q::type_traits<E>::NULL_VAL = static_cast<E>(nf);
q::type_traits<F>::value_type const q::type_traits<F>::NULL_VAL = nf;
q::type_traits<S>::value_type const q::type_traits<S>::NULL_VAL = "";