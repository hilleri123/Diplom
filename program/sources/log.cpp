
#include "log.h"


#ifdef USE_BOOST_LOG
namespace logging=boost::log;
namespace src=boost::log::sources;
namespace trivial=boost::log::trivial;
namespace keywords=boost::log::keywords;
namespace expr=boost::log::expressions;



void my_log::init()
{
	static bool init_called = false;
	if (init_called)
		return;
	init_called = true;
	std::string file_name_template = "imit_%N.log";
#ifdef LOG_DIR
	file_name_template = std::string(LOG_DIR)+"/"+file_name_template;
#endif
	//std::cout << "FILE_NAME_TEMPLATE '" << file_name_template << "'" << std::endl;
	logging::add_file_log(
		keywords::file_name = file_name_template,
		keywords::rotation_size = 1024*1024*10/* 10Mb */,
		keywords::format = (
			expr::stream
			//<< std::hex   //To print the LineID in Hexadecimal format
			<< std::setw(8) << std::setfill('0') 
			<< expr::attr< unsigned int >("LineID")
			<< "  "
			<< expr::format_date_time<boost::posix_time::ptime>("TimeStamp","%d/%m/%Y %H:%M:%S.%f")
			<< "\t: <" << logging::trivial::severity
			<< "> \t" << expr::smessage
			)
		);
	logging::add_common_attributes();
}

#else

#endif


void my_log::log_it(my_log::level lvl, std::string sender, std::string message) {

	message = "( "+sender+" ) \t"+message;

#ifdef USE_BOOST_LOG
	my_log::init();

	trivial::severity_level boost_lvl = trivial::severity_level::info;
#else
	std::string str_lvl = "";
#endif

#ifdef USE_BOOST_LOG 
	#define MY_CASE(for_boost, for_bycicle) \
		boost_lvl = for_boost; \
		break;
#else 
	#define MY_CASE(for_boost, for_bycicle) \
		str_lvl = for_bycicle; \
		break; 
#endif

	
	switch (lvl) {
	case my_log::level::trace:
		MY_CASE(trivial::severity_level::trace, "trace")
	case my_log::level::debug:
		MY_CASE(trivial::severity_level::debug, "debug")
	case my_log::level::info:
		MY_CASE(trivial::severity_level::info, "info")
	case my_log::level::warning:
		MY_CASE(trivial::severity_level::warning, "warning")
	case my_log::level::error:
		MY_CASE(trivial::severity_level::error, "error")
	case my_log::level::fatal:
		MY_CASE(trivial::severity_level::fatal, "fatal")
	}
#undef MY_CASE

#ifdef USE_BOOST_LOG
	static boost::log::sources::severity_logger<trivial::severity_level> lg;

	BOOST_LOG_SEV(lg, boost_lvl) << message;
#else
        static std::ofstream file(LOG_DIR+std::string("/imit.log"));
	file << "[" << str_lvl << "] " << message << std::endl;
#endif
}


