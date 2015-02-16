/*
 * Logger.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: leviathan
 */

#include "Logger.h"
#include <boost/log/core.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/utility/value_ref.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;

Logger::Logger() {
	// TODO Auto-generated constructor stub

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

void Logger::init(std::string fileName) {
	// Set up our sink and formatting
	logging::add_file_log(keywords::file_name = fileName,
			keywords::rotation_size = 10 * 1024 * 1024,
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(
					0, 0, 0),
			keywords::format = (expr::stream
					<< expr::format_date_time<boost::posix_time::ptime>(
							"TimeStamp", "%Y-%m-%d %H:%M:%S") << ": ["
					<< expr::attr<std::string>("ClassName") << "/"
					<< expr::attr<std::string>("File") << "]("
					<< expr::attr<int>("Line") << ") <"
					<< logging::trivial::severity << ">" << expr::smessage));

	logging::add_common_attributes();

	//logging::core::get()->set_filter(
	//		logging::trivial::severity >= logging::trivial::trace);
}

void Logger::initTermSink() {
	namespace bl = boost::log;
	typedef bl::sinks::synchronous_sink<bl::sinks::text_ostream_backend> text_sink;

	boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
	sink->locked_backend()->add_stream(
			boost::shared_ptr<std::ostream>(&std::cout,
					boost::log::empty_deleter()));

	sink->locked_backend()->auto_flush(true);

	sink->set_formatter(bl::expressions::stream << bl::expressions::message);

	bl::core::get()->add_sink(sink);
	bl::add_common_attributes();
}

