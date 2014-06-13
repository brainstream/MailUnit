#ifndef __STUBMTP_EMAIL_DATETIME_H__
#define __STUBMTP_EMAIL_DATETIME_H__

#include <memory>
#include <string>
#include <boost/date_time.hpp>

namespace StubMTP {
namespace Email {

typedef boost::local_time::local_date_time DateTime;
typedef std::shared_ptr<DateTime> DateTimePtr;

DateTimePtr parseDateTime(const std::string & _input);

} // namespace Email
} // namespace StubMTP


#endif // __STUBMTP_EMAIL_DATETIME_H__
