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

#include <sstream>
#include <MailUnit/Application.h>
#include <MailUnit/Storage/Repository.h>
#include <boost/filesystem/operations.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace MailUnit::Storage;
namespace fs = boost::filesystem;

namespace {
    static const MailUnit::PathString tmp_file_ext = MU_PATHSTR(".tmp");

    inline MailUnit::PathString generateUniqueFilename()
    {
        thread_local static boost::uuids::random_generator generator;
#ifdef MU_PATHISWIDECHAR
        return boost::uuids::to_wstring(generator());
#else
        return boost::uuids::to_string(generator());
#endif
    }
} // namespace


Repository::Repository(const fs::path & _storage_direcotiry) :
    m_storage_direcotiry(_storage_direcotiry.is_relative() ?
        fs::absolute(_storage_direcotiry, MailUnit::app().startDir()) : _storage_direcotiry)
{
    initStorageDirectory();
}

void Repository::initStorageDirectory()
{
    try
    {
        fs::create_directories(m_storage_direcotiry);
    }
    catch(const fs::filesystem_error & error)
    {
        std::stringstream message;
        message << "An error occurred during a storage direcorty initialization." << std::endl <<
                   "Path: " << error.path1() << std::endl <<
                   "Error: " << error.what();
        throw(StorageException(message.str()));
    }
}

boost::filesystem::path Repository::makeNewFileName(const MailUnit::PathString & _base, bool _temp)
{
    return m_storage_direcotiry / boost::filesystem::path(_temp ? _base + tmp_file_ext : _base);
}

std::unique_ptr<RawEmail> Repository::createRawEmail()
{
    return std::make_unique<RawEmail>(makeNewFileName(generateUniqueFilename(), true));
}

void Repository::storeEmail(const RawEmail & _email)
{
    // TODO: save
    std::cout << "Storing an e-mail\n";
    std::cout.flush();
}

void Repository::storeEmail(const Email & _email)
{
    // TODO: save
}
