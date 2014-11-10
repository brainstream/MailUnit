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

#ifndef __MUGUI_MAINWINDOW_H__
#define __MUGUI_MAINWINDOW_H__

#include <QtGui/QMainWindow>
#include <QtGui/QLineEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QPlainTextEdit>
#include <MailUnitUI/MqpClient.h>

namespace MailUnit {
namespace Gui {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() override;

private slots:
    void execute();
    void aboutQt();
    void onMessageReceived(const QString & _data);

private:
    void setupUI();
    bool setupMqpClient();

private:
    MqpClient * mp_mqp_client;
    QLineEdit * mp_edit_host;
    QLineEdit * mp_edit_port;
    QPlainTextEdit * mp_edit_query;
    QTextEdit * mp_edit_result;
}; // class MainWindow

} // namespace Gui
} // namespace MailUnit

#endif // __MUGUI_MAINWINDOW_H__
