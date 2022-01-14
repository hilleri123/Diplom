
#pragma once


#define INIT_START(result, init_class, ...) \
do { \
	auto* __macro_tmp = new init_class(__VA_ARGS__);\
	if ( __macro_tmp->init() ) \
		result = __macro_tmp; \
	else {\
		delete __macro_tmp;\
		result = nullptr;
#define INIT_END \
	} \
} while (0)
#define INIT(result, init_class, ...) INIT_START(result, init_class, __VA_ARGS__) INIT_END


class base_init
{
public:
	virtual bool init()
	{
		return true;
	}

	virtual ~base_init() {}

};
