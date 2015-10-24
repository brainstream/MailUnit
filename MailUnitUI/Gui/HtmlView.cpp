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

#include <QVBoxLayout>
#include <MailUnitUI/Gui/HtmlView.h>

using namespace MailUnit::Gui;

HtmlView::HtmlView(QWidget * _parent /*= nullptr*/) :
    QFrame(_parent)
{
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->setMargin(0);
    mp_webview = new QWebView(this);
    layout->addWidget(mp_webview);
    mp_webview->setHtml("TEST");
    setFrameStyle(QFrame::StyledPanel);
    mp_webview->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void HtmlView::setSource(const MimeMessagePart & _source)
{
    mp_webview->setHtml("TEST");
}

void HtmlView::clear()
{
    mp_webview->setHtml(nullptr);
}
