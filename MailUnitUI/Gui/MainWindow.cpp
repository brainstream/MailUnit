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

#include <QtCore/QtAlgorithms>
#include <QtGui/QMessageBox>
#include <MailUnitUI/Gui/MainWindow.h>
#include <MailUnitUI/Gui/QueryWidget.h>
#include <MailUnitUI/Gui/ServerDialog.h>
#include <MailUnitUI/Gui/AboutDialog.h>

using namespace MailUnit::Gui;

ServerListModel::ServerListModel(ServerConfigList & _servers, QObject * _parent) :
    QAbstractItemModel(_parent),
    mr_servers(_servers)
{
    resort();
    connect(&_servers, SIGNAL(added(ServerConfig)), this, SLOT(onServerAdded(ServerConfig)));
    connect(&_servers, SIGNAL(removed(ServerConfig)), this, SLOT(onServerRemoved(ServerConfig)));
    connect(&_servers, SIGNAL(updated(ServerConfig)), this, SLOT(onServerUpdated(ServerConfig)));
}

void ServerListModel::resort()
{
    const int length = mr_servers.size();
    m_sorted_servers.clear();
    m_sorted_servers.reserve(length);
    for(int i = 0; i < length; ++i)
        m_sorted_servers.append(&mr_servers[i]);
    qSort(m_sorted_servers.begin(), m_sorted_servers.end(),
        [](const ServerConfig * left, const ServerConfig *right) {
            return left->name() < right->name();
        });
}

QVariant ServerListModel::data(const QModelIndex & _index, int _role) const
{
    switch(_role)
    {
    case Qt::DisplayRole:
        return m_sorted_servers[_index.row()]->name();
    default:
        return QVariant();
    }
}

void ServerListModel::onServerAdded(const ServerConfig & _server)
{
    resort();
    int position = m_sorted_servers.indexOf(&_server);
    QModelIndex start_index = createIndex(position, 0);
    emit dataChanged(start_index, createIndex(position + 1, 0));
    emit serverAdded(start_index);
}

void ServerListModel::onServerRemoved(const ServerConfig & _server)
{
    // TODO: to implement
    QMessageBox::information(nullptr, QString(), "Not implemented yet");
}

void ServerListModel::onServerUpdated(const ServerConfig & _server)
{
    // TODO: to implement
    QMessageBox::information(nullptr, QString(), "Not implemented yet");
}


MainWindow::MainWindow(Config & _config) :
    mr_config(_config),
    mp_model(nullptr)
{
    setupLayout();
//    m_servers.add(ServerConfig("C Local 5880", "localhost", 5880));
//    m_servers.add(ServerConfig("A Local 5881", "localhost", 5881));
//    m_servers.add(ServerConfig("D Local 5882", "localhost", 5882));
//    m_servers.add(ServerConfig("B Local 5883", "localhost", 5883));
    mp_model = new ServerListModel(mr_config.servers(), this);
    mp_list_servers->setModel(mp_model);
    connect(mp_model, SIGNAL(serverAdded(QModelIndex)), this, SLOT(serverAddedToListView(QModelIndex)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupLayout()
{
    setupUi(this);
    //mp_splitter->setSizes({ mp_list_servers->minimumWidth(), mp_tabs->maximumWidth() });
}

void MainWindow::moveEvent(QMoveEvent * _event)
{
    QMainWindow::moveEvent(_event);
    if(nullptr != _event)
        mr_config.setWindowPosition(_event->pos());
}

void MainWindow::resizeEvent(QResizeEvent * _event)
{
    QMainWindow::resizeEvent(_event);
    if(nullptr != _event)
        mr_config.setWindowSize(_event->size());
}

void MainWindow::addServer()
{
    ServerDialog dlg(this);
    if(dlg.exec() == QDialog::Accepted)
    {
        mr_config.servers().add(ServerConfig(dlg.name(), dlg.host(), dlg.port()));
        mr_config.save();
    }
}

void MainWindow::about()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::serverSelected(const QModelIndex & _index)
{
    const ServerConfig & server = mp_model->at(_index);
    int index = mp_tabs->addTab(new QueryWidget(server), server.name());
    mp_tabs->setCurrentIndex(index);
}

void MainWindow::serverAddedToListView(const QModelIndex & _index)
{
    mp_list_servers->setCurrentIndex(_index);
}

void MainWindow::currentTabChanged(int _index)
{
    QueryWidget * query_widget = static_cast<QueryWidget *>(mp_tabs->widget(_index));
    mp_action_exec_query->setEnabled(nullptr != query_widget);
    if(nullptr == query_widget)
    {
        mp_statusbar->clearMessage();
    }
    else
    {
        mp_statusbar->showMessage(QString("%2:%3")
            .arg(query_widget->server().host())
            .arg(query_widget->server().port()));
    }

}

void MainWindow::closeTab(int _index)
{
    delete mp_tabs->widget(_index);
}

void MainWindow::execQuery()
{
    QueryWidget * query_widget = static_cast<QueryWidget *>(mp_tabs->currentWidget());
    if(nullptr != query_widget)
    {
        query_widget->execute();
    }
}
