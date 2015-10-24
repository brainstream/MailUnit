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

#ifndef __MUGUI_GUI_QUERYWIDGET_H__
#define __MUGUI_GUI_QUERYWIDGET_H__

#include <QWidget>
#include <ui_QueryWidget.h>
#include <MailUnitUI/MqpClient/MqpClient.h>
#include <MailUnitUI/Gui/MessageListView.h>
#include <MailUnitUI/Gui/HtmlView.h>

namespace MailUnit {
namespace Gui {

class QueryWidget : public QWidget, private Ui::QueryWidget
{
    Q_OBJECT

private:
    struct LoadingState
    {
        MqpResponseHeader header;
        quint32 loaded_count;
    }; // struct LoadingState

public:
    explicit QueryWidget(const ServerConfig & _server, QWidget * _parent = nullptr);
    ~QueryWidget() override;

public slots:
    void execute();

private slots:
    void onClientConnected(const MqpResponseHeader & _header);
    void onMessageReceived(const MqpRawMessage & _message);
    void onRequestFinished();
    void onMessageSelected(const MqpMessage * _message);

private:
    void setPlainTextContent(const MimeMessage & _message);
    void setHtmlContent(const MimeMessage & _message);

private:
    MqpClient * mp_client;
    ServerConfig m_server;
    LoadingState * mp_state;
    QList<const MqpMessage *> * mp_messages;
    MessageListView * mp_listview_result;
    HtmlView * mp_html_view;
    MimeMessage * mp_selected_message;
};

} // namespace Gui
} // namespace MailUnit

#endif // __MUGUI_GUI_QUERYWIDGET_H__
