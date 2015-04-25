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

#ifndef __LIBMU_MQP_MQPCLIENT_H__
#define __LIBMU_MQP_MQPCLIENT_H__

#include "../Memory.h"

MU_DECLARE_HANDEL(MU_MQPCLIENT);

MU_EXPORT MU_MQPCLIENT MU_CALL muMqpCreateClient(const char * _host, unsigned short _port);

#endif /* __LIBMU_MQP_MQPCLIENT_H__ */
