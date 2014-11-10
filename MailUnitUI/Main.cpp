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

#include <QtGui/QApplication>
#include <MailUnitUI//MainWindow.h>

using namespace MailUnit::Gui;

int main(int _argc, char ** _argv)
{
    QApplication app(_argc, _argv);
    MainWindow wnd;
    wnd.setWindowTitle("Mail Unit GUI");
    wnd.resize(800, 600); // TODO: from config
    wnd.show();
    app.exec();
    return 0;
}
