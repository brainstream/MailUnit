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

template<typename Item>
class AsyncSequenceItemOperation :
    public std::enable_shared_from_this<AsyncSequenceItemOperation<Item>>
{
private:
    AsyncSequenceItemOperation(const Item & _item, size_t _index) :
        mr_item(_item),
        m_item_index(_index)
    {
        m_current_operation = m_operations.end();
    }

public:
    static std::shared_ptr<AsyncSequenceItemOperation<Item>> create(const Item & _item, size_t _index)
    {
        return std::shared_ptr<AsyncSequenceItemOperation<Item>>(
            new AsyncSequenceItemOperation<Item>(_item, _index));
    }

    const Item & item() const
    {
        return mr_item;
    }

    size_t itemIndex() const
    {
        return m_item_index;
    }

    void addStep(std::unique_ptr<AsyncOperation> && _operation)
    {
        m_operations.push_back(std::move(_operation));
    }

    bool empty() const
    {
        return m_operations.empty();
    }

    void run(AsyncWriter & _writer, AsioCallback _callbak)
    {
        m_current_operation = m_operations.begin();
        execute(_writer, _callbak);
    }

private:
    void execute(AsyncWriter & _writer, AsioCallback _callbak);

private:
    const Item & mr_item;
    const size_t m_item_index;
    std::list<std::unique_ptr<AsyncOperation>> m_operations;
    typename std::list<std::unique_ptr<AsyncOperation>>::iterator m_current_operation;
}; // class AsyncSequenceItemOperation

template<typename Item>
void AsyncSequenceItemOperation<Item>::execute(AsyncWriter & _writer, AsioCallback _callbak)
{
    if(m_operations.end() == m_current_operation)
    {
        callAsioCallback(_callbak);
        return;
    }
    auto self = this->shared_from_this();
    AsyncOperation * operation = m_current_operation->get();
    ++m_current_operation;
    operation->run(_writer, [self, &_writer, _callbak](const boost::system::error_code & error_code) {
        if(error_code && !callAsioCallback(_callbak, error_code))
            return false;
        self->execute(_writer, _callbak);
        return true;
    });
}

template<typename Sequence>
class AsyncSequenceOperation :
    public std::enable_shared_from_this<AsyncSequenceOperation<Sequence>>
{
public:
    typedef typename Sequence::value_type ValueType;
    typedef std::function<void(AsyncSequenceItemOperation<ValueType> &)> ItemOperatiton;
    typedef std::function<const Sequence & ()> SequenceHolder;

private:
    AsyncSequenceOperation(SequenceHolder _sequence_holder, const ItemOperatiton & _item_operation) :
        m_sequence_holder(_sequence_holder),
        m_item_operation(_item_operation),
        m_current_item(_sequence_holder().cbegin())
    {
    }

public:
    static std::shared_ptr<AsyncSequenceOperation<Sequence>> create(
        SequenceHolder _sequence_holder, const ItemOperatiton & _item_operation)
    {
        return std::shared_ptr<AsyncSequenceOperation<Sequence>>(
            new AsyncSequenceOperation<Sequence>(_sequence_holder, _item_operation));
    }

    void run(AsyncWriter & _writer, AsioCallback _complete_callback)
    {
        m_current_item = m_sequence_holder().cbegin();
        execute(_writer, _complete_callback);
    }

private:
    void execute(AsyncWriter & _writer, AsioCallback _complete_callback);

private:
    SequenceHolder m_sequence_holder;
    ItemOperatiton m_item_operation;
    typename Sequence::const_iterator m_current_item;
    AsioCallback m_complete_callback;
}; // class AsyncSequenceOperation

template<typename Sequence>
void AsyncSequenceOperation<Sequence>::execute(AsyncWriter & _writer, AsioCallback _complete_callback)
{
    const Sequence & sequence = m_sequence_holder();
    if(sequence.end() == m_current_item)
    {
        callAsioCallback(_complete_callback);
        return;
    }
    auto operation = AsyncSequenceItemOperation<ValueType>::create(
        *m_current_item, m_current_item - sequence.begin());
    ++m_current_item;
    m_item_operation(*operation);
    auto self = this->shared_from_this();
    operation->run(_writer,
        [self, &_writer, _complete_callback](const boost::system::error_code & error_code) {
            if(error_code && !callAsioCallback(_complete_callback, error_code))
                return false;
            self->execute(_writer, _complete_callback);
            return true;
        }
    );
}

} // namespace IO
} // namespace MailUnit

#endif // __MU_IO_ASYNCSEQUENCEOPERATION_H__
