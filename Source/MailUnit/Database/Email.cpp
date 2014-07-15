#include <MailUnit/Database/Email.h>
#include <SQLite/sqlite3.h>

using namespace MailUnit::Database;

void MailUnit::Database::saveEmail(const Email & _email)
{
    sqlite3_initialize();
    sqlite3 * sqlite;
    sqlite3_open("/home/brainstream/temp/mailunit.sqlite3", &sqlite);
    sqlite3_close(sqlite);
    sqlite3_shutdown();
}
