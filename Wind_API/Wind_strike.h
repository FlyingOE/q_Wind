#ifndef __WIND_STRIKE_H__
#define __WIND_STRIKE_H__
#pragma comment(lib, "WindQuantData.lib")

#include "Wind_API.h"
#include "WindEvent.h"

#include "WindQuantAPI.h"

#include <memory>	//C++11: std::shared_ptr<>, std::unique_ptr<>
#include <future>	//C++11

namespace Wind {
	namespace callback {

		// A manager to take care of memory management for Wind's asynchronous callbacks
		class Result {
		public:
			typedef std::shared_ptr<std::promise<Event*> > promise_ptr;

			Result();
			promise_ptr* dup() const;
			K waitFor(::WQID qid, std::chrono::milliseconds const& timeout = Wind::ASYNC_TIMEOUT);

		private:
			promise_ptr result_;
		};

		int WINAPI strike(::WQEvent* pEvent, LPVOID lpUserParam);

	}//namespace Wind::callback
}//namespace Wind

#endif//__WIND_STRIKE_H__