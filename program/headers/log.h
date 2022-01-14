
#pragma once

#include "config.h"
#include <string>
#include <utility>
#include <iostream>

#ifdef USE_BOOST_LOG
#include <iomanip>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>

#else

#include <fstream>

#endif

namespace my_log {
	enum level {
		trace,
		debug,
		info,
		warning,
		error,
		fatal
	};
#ifdef USE_BOOST_LOG
	void init();
#else

#endif

	void log_it(level lvl, std::string sender, std::string message);
}


#ifndef __FUNCTION_NAME__
	#ifdef WIN32
		#ifdef __FUNCTION__
			#define __FUNCTION_NAME__ __FUNCTION__
                #endif
	#endif
	#ifndef __FUNCTION_NAME__
		#define __FUNCTION_NAME__ __func__
	#endif
#endif

