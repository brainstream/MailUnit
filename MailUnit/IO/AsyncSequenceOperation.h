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

#ifndef __MU_IO_ASYNCSEQUENCEOPERATION_H__
#define __MU_IO_ASYNCSEQUENCEOPERATION_H__

#include <memory>
#include <list>
#include <MailUnit/IO/AsyncOperation.h>

namespace MailUnit {
namespace IO {

template<typename Item, typename Socket>
class AsyncSequenceItemOperation :
    public std::enable_shared_from_this<AsyncSequenceItemOperation<Item, Socket>>
{
private:
    AsyncSequenceItemOperation(const Item & _item, size_t _index) :
        mr_item(_item),
        m_item_index(_index)
    {
        m_current_operation = m_operations.end();
    }

public:
    static std::shared_ptr<AsyncSequenceItemOperation<Item, Socket>> create(const Item & _item, size_t _index)
    {
        return std::shared_ptr<AsyncSequenceItemOperation<Item, Socket>>(
            new AsyncSequenceItemOperation<Item, Socket>(_item, _index));
    }

    const Item & item() const
    {
        return mr_item;
    }

    size_t itemIndex() const
    {
        return m_item_index;
    }

    void addStep(std::unique_ptr<AsyncOperation<Socket>> && _operation)
    {
        m_operations.push_back(std::move(_operation));
    }

    bool empty() const
    {
        return m_operations.empty();
    }

    void run(Socket & _socket, AsioCallback _callbak)
    {
        m_current_operation = m_operations.begin();
        execute(_socket, _callbak);
    }

private:
    void execute(Socket & _socket, AsioCallback _callbak);

private:
    const Item & mr_item;
    const size_t m_item_index;
    std::list<std::unique_ptr<AsyncOperation<Socket>>> m_operations;
    typename std::list<std::unique_ptr<AsyncOperation<Socket>>>::iterator m_current_operation;
}; // class AsyncSequenceItemOperation

template<typename Item, typename Socket>
void AsyncSequenceItemOperation<Item, Socket>::execute(Socket & _socket, AsioCallback _callbak)
{
    if(m_operations.end() == m_current_operation)
    {
        callAsioCallback(_callbak);
        return;
    }
    auto self = this->shared_from_this();
    AsyncOperation<Socket> * operation = m_current_operation->get();
    ++m_current_operation;
    operation->run(_socket, [self, &_socket, _callbak](const boost::system::error_code & error_code) {
        if(error_code && !callAsioCallback(_callbak, error_code))
            return false;
        self->execute(_socket, _callbak);
        return true;
    });
}

template<typename Sequence, typename Socket>
class AsyncSequenceOperation :
    public std::enable_shared_from_this<AsyncSequenceOperation<Sequence, Socket>>
{
public:
    typedef typename Sequence::value_type ValueType;
    typedef std::function<void(AsyncSequenceItemOperation<ValueType, Socket> &)> ItemOperatiton;

private:
    AsyncSequenceOperation(std::shared_ptr<Sequence> _sequence, const ItemOperatiton & _item_operation) :
        m_sequence_ptr(_sequence),
        m_item_operation(_item_operation),
        m_current_item(m_sequence_ptr->cbegin())
    {
    }

public:
    static std::shared_ptr<AsyncSequenceOperation<Sequence, Socket>> create(
        std::shared_ptr<Sequence> _sequence, const ItemOperatiton & _item_operation)
    {
        return std::shared_ptr<AsyncSequenceOperation<Sequence, Socket>>(
            new AsyncSequenceOperation<Sequence, Socket>(_sequence, _item_operation));
    }

    void run(Socket & _socket, AsioCallback _complete_callback)
    {
        m_current_item = m_sequence_ptr->begin();
        execute(_socket, _complete_callback);
    }

private:
    void execute(Socket & _socket, AsioCallback _complete_callback);

private:
    std::shared_ptr<Sequence> m_sequence_ptr;
    ItemOperatiton m_item_operation;
    typename Sequence::const_iterator m_current_item;
    AsioCallback m_complete_callback;
}; // class AsyncSequenceOperation

template<typename Sequence, typename Socket>
void AsyncSequenceOperation<Sequence, Socket>::execute(Socket & _socket, AsioCallback _complete_callback)
{
    if(m_sequence_ptr->end() == m_current_item)
    {
        callAsioCallback(_complete_callback);
        return;
    }
    auto operation = AsyncSequenceItemOperation<ValueType, Socket>::create(
        *m_current_item, m_current_item - m_sequence_ptr->begin());
    ++m_current_item;
    m_item_operation(*operation);
    auto self = this->shared_from_this();
    operation->run(_socket,
        [self, &_socket, _complete_callback](const boost::system::error_code & error_code) {
            if(error_code && !callAsioCallback(_complete_callback, error_code))
                return false;
            self->execute(_socket, _complete_callback);
            return true;
        }
    );
}

} // namespace IO
} // namespace MailUnit

#endif // __MU_IO_ASYNCSEQUENCEOPERATION_H__
