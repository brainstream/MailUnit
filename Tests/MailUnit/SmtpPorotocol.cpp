/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                              *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of      *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;      *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.      *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#include <boost/test/unit_test.hpp>
#include <boost/optional.hpp>
#include <MailUnit/Smtp/Protocol.h>
#include <MailUnit/OS/FileSystem.h>

using namespace MailUnit::Smtp;
using namespace MailUnit::Storage;

namespace MailUnit {
namespace Test {

namespace {

struct TestContext
{
    TestContext()
    {
        repository_path = MailUnit::OS::tempFilepath();
    }

    ~TestContext()
    {
        if(boost::filesystem::is_directory(repository_path))
        {
            boost::filesystem::remove_all(repository_path);
        }
    }

    boost::filesystem::path repository_path;
}; // struct TestContext

class TestProtocolTransport : public ProtocolTransport
{
public:
    TestProtocolTransport() :
        read_count(0),
        write_count(0),
        switch_to_tls_count(0),
        exit_count(0)
    {
    }

    void requestForRead()
    {
        ++read_count;
    }

    void requestForWrite(const Response &_response)
    {
        ++write_count;
        latest_response = _response;
    }

    void requestForSwitchToTls()
    {
        ++switch_to_tls_count;
    }

    void requestForExit()
    {
        ++exit_count;
    }

    void performNextAction()
    {
        callNextAction();
    }

public:
    size_t read_count;
    size_t write_count;
    size_t switch_to_tls_count;
    size_t exit_count;
    boost::optional<Response> latest_response;
}; // class TestProtocolTransport

} // namespace

BOOST_AUTO_TEST_SUITE(SmtpProtocolTests)

BOOST_AUTO_TEST_CASE(simpleSequanceTest)
{
    TestContext context;
    Repository repository(context.repository_path);
    TestProtocolTransport transport;
    Protocol protocol(repository, transport);
    BOOST_CHECK_EQUAL(0, transport.read_count);
    BOOST_CHECK_EQUAL(0, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);
    BOOST_CHECK(!transport.latest_response.is_initialized());

    protocol.start();
    transport.performNextAction();
    BOOST_CHECK_EQUAL(0, transport.read_count);
    BOOST_CHECK_EQUAL(1, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);
    BOOST_CHECK(transport.latest_response.is_initialized());
    BOOST_CHECK(ResponseCode::ready == transport.latest_response->code());
    transport.performNextAction();
    BOOST_CHECK_EQUAL(1, transport.read_count);
    BOOST_CHECK_EQUAL(1, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);

    protocol.processInput("EHLO example.com\r\n", 18);
    transport.performNextAction();
    BOOST_CHECK_EQUAL(1, transport.read_count);
    BOOST_CHECK_EQUAL(2, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);
    BOOST_CHECK(ResponseCode::ok == transport.latest_response->code());
    transport.performNextAction();
    BOOST_CHECK_EQUAL(2, transport.read_count);
    BOOST_CHECK_EQUAL(2, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);

    protocol.processInput("MAIL FROM:from@example.com\r\n", 28);
    transport.performNextAction();
    BOOST_CHECK_EQUAL(2, transport.read_count);
    BOOST_CHECK_EQUAL(3, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);
    BOOST_CHECK(ResponseCode::ok == transport.latest_response->code());
    transport.performNextAction();
    BOOST_CHECK_EQUAL(3, transport.read_count);
    BOOST_CHECK_EQUAL(3, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);

    protocol.processInput("RCPT TO:to@example.com\r\n", 24);
    transport.performNextAction();
    BOOST_CHECK_EQUAL(3, transport.read_count);
    BOOST_CHECK_EQUAL(4, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);
    BOOST_CHECK(ResponseCode::ok == transport.latest_response->code());
    transport.performNextAction();
    BOOST_CHECK_EQUAL(4, transport.read_count);
    BOOST_CHECK_EQUAL(4, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);

    protocol.processInput("DATA\r\n", 6);
    transport.performNextAction();
    BOOST_CHECK_EQUAL(4, transport.read_count);
    BOOST_CHECK_EQUAL(5, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);
    BOOST_CHECK(ResponseCode::intermediate == transport.latest_response->code());
    transport.performNextAction();
    BOOST_CHECK_EQUAL(5, transport.read_count);
    BOOST_CHECK_EQUAL(5, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);

    protocol.processInput("some data", 9);
    transport.performNextAction();
    BOOST_CHECK_EQUAL(6, transport.read_count);
    BOOST_CHECK_EQUAL(5, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);

    protocol.processInput("tail\r\n.\r\n", 9);
    transport.performNextAction();
    BOOST_CHECK_EQUAL(6, transport.read_count);
    BOOST_CHECK_EQUAL(6, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);
    BOOST_CHECK(ResponseCode::ok == transport.latest_response->code());
    transport.performNextAction();
    BOOST_CHECK_EQUAL(7, transport.read_count);
    BOOST_CHECK_EQUAL(6, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);

    protocol.processInput("QUIT\r\n", 6);
    transport.performNextAction();
    BOOST_CHECK_EQUAL(7, transport.read_count);
    BOOST_CHECK_EQUAL(7, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(0, transport.exit_count);
    BOOST_CHECK(ResponseCode::closing == transport.latest_response->code());
    transport.performNextAction();
    BOOST_CHECK_EQUAL(7, transport.read_count);
    BOOST_CHECK_EQUAL(7, transport.write_count);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    BOOST_CHECK_EQUAL(1, transport.exit_count);
}

BOOST_AUTO_TEST_CASE(startTlsTest)
{
    TestContext context;
    Repository repository(context.repository_path);
    TestProtocolTransport transport;
    Protocol protocol(repository, transport);
    protocol.enableStartTls(true);
    BOOST_CHECK_EQUAL(0, transport.switch_to_tls_count);
    protocol.start();
    transport.performNextAction();
    transport.performNextAction();
    protocol.processInput("EHLO example.com\r\n", 18);
    transport.performNextAction();
    transport.performNextAction();
    protocol.processInput("STARTTLS\r\n", 10);
    transport.performNextAction();
    transport.performNextAction();
    BOOST_CHECK_EQUAL(1, transport.switch_to_tls_count);
}

BOOST_AUTO_TEST_CASE(unrecognisedCommandTest)
{
    TestContext context;
    Repository repository(context.repository_path);
    TestProtocolTransport transport;
    Protocol protocol(repository, transport);
    protocol.start();
    transport.performNextAction();
    transport.performNextAction();
    BOOST_CHECK(ResponseCode::ready == transport.latest_response->code());
    protocol.processInput("ERROR example.com\r\n", 19);
    transport.performNextAction();
    transport.performNextAction();
    BOOST_CHECK(transport.latest_response.is_initialized());
    BOOST_CHECK(ResponseCode::unrecognizedCommand == transport.latest_response->code());
}

BOOST_AUTO_TEST_CASE(badCommandsSequenceTest)
{
    TestContext context;
    Repository repository(context.repository_path);
    TestProtocolTransport transport;
    Protocol protocol(repository, transport);
    protocol.start();
    transport.performNextAction();
    transport.performNextAction();
    protocol.processInput("EHLO example.com\r\n", 18);
    transport.performNextAction();
    transport.performNextAction();
    BOOST_CHECK(ResponseCode::ok == transport.latest_response->code());
    protocol.processInput("RCPT TO:to@example.com\r\n", 24);
    transport.performNextAction();
    transport.performNextAction();
    BOOST_CHECK(ResponseCode::badCommandsSequence == transport.latest_response->code());
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test
} // namespace MailUnit
