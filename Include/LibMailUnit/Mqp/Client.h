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

#ifndef __LIBMU_PUBAPI_MQP_CLIENT_H__
#define __LIBMU_PUBAPI_MQP_CLIENT_H__

#include "../Def.h"
#include "../IO.h"

#ifdef __cplusplus
extern "C" {
#endif

MU_DECLARE_API_TYPE(MU_MqpClient)

typedef enum
{
    mu_mqp_matched,
    mu_mqp_deleted,
    mu_mqp_error
} MU_MqpResponseType;

typedef struct
{
    MU_MqpResponseType response_type;
    unsigned int status_code;
    unsigned int affected_count;
} MU_MqpResponseHeader;

typedef struct
{
    unsigned long id;
    char * subject;
    char * from;
    char * to;
    char * cc;
    char * bcc;
} MU_MqpMessageHeader;

typedef enum
{
    mu_mqp_evt_error,
    mu_mqp_evt_response_header,
    mu_mqp_evt_message_header,
    mu_mqp_evt_message_body
} MU_MqpEvent;

typedef void MU_CALL (* MU_MqpProc)(MU_MqpEvent _event, const void * _arg, void * _user_data);

MU_API MU_MqpClient * MU_CALL muMqpCreateClient(const char * _host, unsigned short _port, MU_MqpProc _proc, void * _user_data);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __LIBMU_PUBAPI_MQP_CLIENT_H__ */
