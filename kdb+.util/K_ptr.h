#ifndef __K_PTR_H__
#define __K_PTR_H__
#pragma comment(lib, "kdb+.util.lib")

#include "types.h"

#include <type_traits>
#include <memory>	//C++11: std::unique_ptr<>

namespace q {

	struct K_delete {
		void operator()(K& k) const {
			if (k != K_NIL) {
				r0(k);
				k = K_NIL;
			}
		}
	};

	typedef std::unique_ptr<std::remove_pointer<K>::type, K_delete> K_ptr;

}//namespace q

#endif//__K_PTR_H__