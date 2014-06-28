#ifndef __MU_DATABASE_SQL_H__
#define __MU_DATABASE_SQL_H__

namespace MailUnit {
namespace Database {

constexpr const char * const sql_init_db =
    "CREATE TABLE \"Message\"(\n"
    "    \"Id\"      INTEGER PRIMARY KEY AUTOINCREMENT,\n"
    "    \"Subject\" TEXT,\n"
    "    \"RawData\" TEXT\n"
    ");\n"
    "CREATE TABLE \"Header\"(\n"
    "    \"Id\"      INTEGER PRIMARY KEY AUTOINCREMENT,\n"
    "    \"Message\" INTEGER,\n"
    "    \"Name\"    VARCHAR(250),\n"
    "    \"Value\"   TEXT,\n"
    "    FOREIGN KEY(\"Message\") REFERENCES \"Message\"(\"Id\")\n"
    ");\n"
    "CREATE TABLE \"Exchange\"(\n"
    "    \"Id\"      INTEGER PRIMARY KEY AUTOINCREMENT,\n"
    "    \"Mailbox\" VARCHAR(250),\n"
    "    \"Message\" INTEGER,\n"
    "    \"Reason\"  INTEGER,\n"
    "    FOREIGN KEY(\"Message\") REFERENCES \"Message\"(\"Id\")\n"
    ");\n"
    "CREATE INDEX \"iMessageSubject\"  ON \"Message\"(\"Subject\");\n"
    "CREATE INDEX \"iHeaderMessage\"   ON \"Header\"(\"Message\");\n"
    "CREATE INDEX \"iHeaderName\"      ON \"Header\"(\"Name\");\n"
    "CREATE INDEX \"iExchangeMailbox\" ON \"Exchange\"(\"Mailbox\");\n"
    "CREATE INDEX \"iExchangeMessage\" ON \"Exchange\"(\"Message\");";

} // namespace Database
} // namespace MailUnit

#endif // __MU_DATABASE_SQL_H__
