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
