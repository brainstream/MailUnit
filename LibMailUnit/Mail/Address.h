/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit Library.                                                      *
 *                                                                                             *
 * MailUnit Library is free software: you can redistribute it and/or modify it under the terms *
 * of the GNU Lesser General Public License as published by the Free Software Foundation,      *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit Library is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  *
 * PURPOSE. See the GNU Lesser General Public License for more details.                        *
 *                                                                                             *
 * You should have received a copy of the GNU License General Public License along with        *
 * MailUnit Library. If not, see <http://www.gnu.org/licenses>.                                *
 *                                                                                             *
 ***********************************************************************************************/

/**
 * @file
 * @brief The address mail header formats parser and coresponding data types.
 *
 * @anchor rfc-address-id
 * @htmlinclude RFC/AddressSpec.html
*/

#include "../Memory.h"

typedef MU_HANDLE MU_MAILBOXGROUP;
typedef MU_HANDLE MU_MAILBOX;

/**
 * @brief Parses string described in @ref rfc-address-id "RFC"
 *
 * @param _raw_address_group
 *     String that describes either a mailbox or a mailbox group.
 * @return
 *     If string parsed successfully function returns a handle of the mailbox group object.
 *     In failure case the function returns @a MU_INVALID_HANDLE.
 * @remarks
 *     Returned handle must be destroyed by calling the @ref muFree function.
 */
MUAPI MU_MAILBOXGROUP muMailboxGroupParse(const char * _raw_address_group);

/**
 * @brief Returns a count of mailboxes in @a _mailbox_group.
 */
MUAPI size_t muMailboxCount(MU_MAILBOXGROUP _mailbox_group);

/**
 * @brief Returns a mailbox at @a _index position in @a _mailbox_group or @a MU_INVALID_HANDLE.
 * @remarks
 *     Returned handle does not require destruction.
 */
MUAPI MU_MAILBOX muMailbox(MU_MAILBOXGROUP _mailbox_group, size_t _index);

/**
 * @brief Returns the optional name of the mailbox group.
 *
 * If @a _mailbox_group does not contain a name, function returns @a NULL.
 */
MUAPI const char * muMailboxGroupName(MU_MAILBOXGROUP _mailbox_group);

/**
 * @brief Returns the optional name of the mailbox.
 *
 * If @a _mailbox does not contain a name, function returns @a NULL.
 */
MUAPI const char * muMailboxName(MU_MAILBOX _mailbox);

/**
 * @brief Returns an address from @a _mailbox or @a NULL.
 */
MUAPI const char * muMailboxAddress(MU_MAILBOX _mailbox);