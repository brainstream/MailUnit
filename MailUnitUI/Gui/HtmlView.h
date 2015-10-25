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

#ifndef __MUGUI_GUI_HTMLVIEW_H__
#define __MUGUI_GUI_HTMLVIEW_H__

#include <QFrame>
#include <QWebView>
#include <MailUnitUI/MqpClient/MimeMessage.h>

namespace MailUnit {
namespace Gui {

class HtmlView : public QFrame
{
    Q_OBJECT

public:
    explicit HtmlView(QWidget * _parent = nullptr);
    bool trySetContent(const MimeMessagePart & _source);
    void clear();

private:
    void setTextHtmlSource(const MimeMessagePart & _source);
    void setMulitpartRelatedSource(const MimeMessagePart & _source);

private:
    QWebView * mp_webview;
    const MimeMessagePart * mp_source;
}; // class HtmlView

} // namespace Gui
} // namespace MailUnit

#endif // __MUGUI_GUI_HTMLVIEW_H__
