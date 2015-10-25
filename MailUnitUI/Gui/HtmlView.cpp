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
    setFrameStyle(QFrame::StyledPanel);
    mp_webview->setContextMenuPolicy(Qt::ActionsContextMenu);
}

bool HtmlView::trySetContent(const MimeMessagePart & _source)
{
    if(_source.contentType().compare("text", Qt::CaseInsensitive) == 0 &&
       _source.contentSubtype().compare("html", Qt::CaseInsensitive) == 0)
    {
        setTextHtmlSource(_source);
    }
    else if(_source.contentType().compare("multipart", Qt::CaseInsensitive) == 0 &&
        _source.contentSubtype().compare("related", Qt::CaseInsensitive) == 0)
    {
        setMulitpartRelatedSource(_source);
    }
    else
    {
        return false;
    }
    return true;
}

void HtmlView::setTextHtmlSource(const MimeMessagePart & _source)
{
    mp_webview->setHtml(_source.content());
}

void HtmlView::setMulitpartRelatedSource(const MimeMessagePart & _source)
{
    for(const std::unique_ptr<const MimeMessagePart> & part : _source.parts())
    {
        if(part->contentType().compare("text", Qt::CaseInsensitive) == 0 &&
           part->contentSubtype().compare("html", Qt::CaseInsensitive) == 0)
        {
            mp_webview->setHtml(part->content()); // TODO: the "cid:" url handler
        }
        else
        {
            // TODO: map of cid-content
        }
    }
}

void HtmlView::clear()
{
    mp_webview->setHtml(nullptr);
}
