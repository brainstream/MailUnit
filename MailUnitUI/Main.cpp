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

#include <boost/preprocessor/stringize.hpp>
#include <QDir>
#include <QApplication>
#include <QDesktopServices>
#include <MailUnit/OS/FileSystem.h>
#include <MailUnitUI/Config.h>
#include <MailUnitUI/Gui/MainWindow.h>

using namespace MailUnit::Gui;

Config * loadConfig()
{
    QDir config_dir(MailUnit::OS::userConfigDirectory().string().c_str());
    config_dir.cd(BOOST_PP_STRINGIZE(_MU_CONFIG_DIRECTORY));
    return new Config(config_dir.filePath(BOOST_PP_STRINGIZE(_MU_GUI_BINARY_NAME) ".xml"));
}

int main(int _argc, char ** _argv)
{
    QApplication app(_argc, _argv);
    Config * config = loadConfig();
    MainWindow wnd(*config);
    wnd.setWindowTitle("Mail Unit GUI");
    wnd.move(config->windowPosition());
    wnd.resize(config->windowSize());
    wnd.show();
    app.exec();
    config->save();
    delete config;
    return 0;
}
