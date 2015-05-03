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

#ifndef __MUGUI_GUI_MAINWINDOW_H__
#define __MUGUI_GUI_MAINWINDOW_H__

#include <QVector>
#include <QMainWindow>
#include <MailUnitUI/Config.h>
#include <ui_MainWindow.h>

namespace MailUnit {
namespace Gui {

class ServerListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ServerListModel(ServerConfigList & _servers, QObject * _parent);

    QModelIndex index(int _row, int _column, const QModelIndex & _parent) const override
    {
        Q_UNUSED(_parent)
        return createIndex(_row, _column);
    }

    QModelIndex parent(const QModelIndex & _child) const override
    {
        Q_UNUSED(_child)
        return QModelIndex();
    }

    int rowCount(const QModelIndex & _parent) const override
    {
        Q_UNUSED(_parent)
        return mr_servers.size();
    }

    int columnCount(const QModelIndex & _parent) const override
    {
        Q_UNUSED(_parent)
        return 1;
    }

    QVariant data(const QModelIndex & _index, int _role) const override;

    const ServerConfig & at(const QModelIndex & _index) const
    {
        return *m_sorted_servers[_index.row()];
    }

signals:
    void serverAdded(const QModelIndex & _index);

private:
    void resort();

private slots:
    void onServerAdded(const ServerConfig & _server);
    void onServerRemoved(const ServerConfig & _server);
    void onServerUpdated(const ServerConfig & _server);

private:
    ServerConfigList & mr_servers;
    QVector<const ServerConfig *> m_sorted_servers;
}; // class ServerListModel


class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Config & _config);
    ~MainWindow() override;

protected:
    void moveEvent(QMoveEvent * _event) override;
    void resizeEvent(QResizeEvent * _event) override;

private:
    void setupLayout();

private slots:
    void addServer();
    void about();
    void aboutQt();
    void serverSelected(const QModelIndex & _index);
    void serverAddedToListView(const QModelIndex & _index);
    void currentTabChanged(int _index);
    void closeTab(int _index);
    void execQuery();

private:
    Config & mr_config;
    ServerListModel * mp_model;
}; // class MainWindow

} // namespace Gui
} // namespace MailUnit

#endif // __MUGUI_GUI_MAINWINDOW_H__
