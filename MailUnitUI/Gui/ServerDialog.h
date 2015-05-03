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

#ifndef __MUGUI_GUI_SERVERDIALOG_H__
#define __MUGUI_GUI_SERVERDIALOG_H__

#include <QDialog>
#include <ui_ServerDialog.h>

namespace MailUnit {
namespace Gui {

class ServerDialog : public QDialog, private Ui::ServerDialog
{
    Q_OBJECT

public:
    explicit ServerDialog(QWidget * _parent = nullptr) :
        QDialog(_parent)
    {
        setupUi(this);
    }

    const QString name() const
    {
        return mp_edit_name->text();
    }

    const QString host() const
    {
        return mp_edit_host->text();
    }

    quint16 port() const
    {
        return static_cast<quint16>(mp_spinbox_port->value());
    }

}; // class ServerDialog

} // namespace Gui
} // namespace MailUnit

#endif // __MUGUI_GUI_SERVERDIALOG_H__
