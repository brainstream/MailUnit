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

#ifndef __MUGUI_CONFIG_H__
#define __MUGUI_CONFIG_H__

#include <QFile>
#include <QPoint>
#include <QSize>
#include <MailUnitUI/ServerConfig.h>

namespace MailUnit {
namespace Gui {

class Config final
{
public:
    explicit Config(const QString & _config_file_path);

    ~Config();

    void reload();

    bool save();

    ServerConfigList & servers()
    {
        return *mp_servers;
    }

    const QSize & windowSize() const
    {
        return m_wnd_size;
    }

    void setWindowSize(const QSize & _size)
    {
        m_wnd_size = _size;
    }

    const QPoint & windowPosition() const
    {
        return m_wnd_pos;
    }

    void setWindowPosition(const QPoint & _pos)
    {
        m_wnd_pos = _pos;
    }

private:
    QString m_config_file_path;
    ServerConfigList * mp_servers;
    QPoint m_wnd_pos;
    QSize m_wnd_size;
}; // class Config

} // namespace Gui
} // namespace MailUnit

#endif // __MUGUI_CONFIG_H__
