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

#ifndef __MU_SMTP_STATEMACHINE_DATASTATE_H__
#define __MU_SMTP_STATEMACHINE_DATASTATE_H__

#include <MailUnit/Smtp/StateMachine/StartState.h>

namespace MailUnit {
namespace Smtp {


class DataState : public State
{
public:
    DataState() :
        m_end_of_data(false)
    {
    }

    boost::optional<ResponseCode> processInput(const char * _data, Protocol & _protocol) override;

protected:
    void reset() override
    {
        m_data.clear();
        m_end_of_data = false;
    }

private:
    bool m_end_of_data;
    std::string m_data;
}; // class DataState


} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_STATEMACHINE_DATASTATE_H__
