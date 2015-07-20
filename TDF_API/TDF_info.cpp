#include "stdafx.h"
#include "TDF_API.h"

#include "TDF_API_helper.h"

TDF_API K K_DECL TDF_index_fields(K _) {
	return TDF::traits::Index::accessor_map::getInstance()->getFields();
}

/*
TDF_API K K_DECL TDF_market_fields(K _) {
	return TDF::traits::Market::accessor_map::getInstance()->getFields();
}

TDF_API K K_DECL TDF_future_fields(K _) {
	return TDF::traits::Future::accessor_map::getInstance()->getFields();
}
*/