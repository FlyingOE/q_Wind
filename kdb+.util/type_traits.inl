#ifndef __TYPE_TRAITS_H__
static_assert(0, "Include kdb+.util/type_traits.h instead!");
#endif

template <>
struct q::type_traits<void> {
	typedef K value_type;
	static TypeNum const TYPE_NUM = 0;
	static value_type const NULL_VAL;
	static value_type* index(K k) { return kK(k); }
};

template <>
struct q::type_traits<C> {
	typedef C value_type;
	static TypeNum const TYPE_NUM = KC;
	static value_type const NULL_VAL;
	static value_type* index(K k) { return reinterpret_cast<C*>(kC(k)); }
};

template <>
struct q::type_traits<G> {
	typedef G value_type;
	static TypeNum const TYPE_NUM = KG;
	static value_type const NULL_VAL;
	static value_type* index(K k) { return kG(k); }
};

template <>
struct q::type_traits<H> {
	typedef H value_type;
	static TypeNum const TYPE_NUM = KH;
	static value_type const NULL_VAL;
	static value_type* index(K k) { return kH(k); }
};

template <>
struct q::type_traits<I> {
	typedef I value_type;
	static TypeNum const TYPE_NUM = KI;
	static value_type const NULL_VAL;
	static value_type* index(K k) { return kI(k); }
};

template <>
struct q::type_traits<J> {
	typedef J value_type;
	static TypeNum const TYPE_NUM = KJ;
	static value_type const NULL_VAL;
	static value_type* index(K k) { return kJ(k); }
};

template <>
struct q::type_traits<E> {
	typedef E value_type;
	static TypeNum const TYPE_NUM = KE;
	static value_type const NULL_VAL;
	static value_type* index(K k) { return kE(k); }
};

template <>
struct q::type_traits<F> {
	typedef F value_type;
	static TypeNum const TYPE_NUM = KF;
	static value_type const NULL_VAL;
	static value_type* index(K k) { return kF(k); }
};

template <>
struct q::type_traits<S> {
	typedef S value_type;
	static TypeNum const TYPE_NUM = KS;
	static value_type const NULL_VAL;
	static value_type* index(K k) { return kS(k); }
};