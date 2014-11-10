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

#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QIntValidator>
#include <QtGui/QMessageBox>
#include <MailUnitUI/MainWindow.h>

using namespace MailUnit::Gui;

MainWindow::MainWindow() :
    mp_mqp_client(nullptr),
    mp_edit_host(nullptr),
    mp_edit_port(nullptr),
    mp_edit_query(nullptr),
    mp_edit_result(nullptr)
{
    setupUI();

    // TODO: to config
    mp_edit_host->setText("localhost");
    mp_edit_port->setText("5880");
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupUI()
{
    QWidget * root = new QWidget(this);
    setCentralWidget(root);
    QGridLayout * layout = new QGridLayout(root);

    QWidget * host_group = new QWidget(root);
    QHBoxLayout * host_group_layout = new QHBoxLayout(host_group);
    mp_edit_host = new QLineEdit(host_group);
    mp_edit_port = new QLineEdit(host_group);
    mp_edit_port->setValidator(new QIntValidator(1, UINT16_MAX, this));
    host_group_layout->addWidget(new QLabel(tr("Host"), host_group));
    host_group_layout->addWidget(mp_edit_host);
    host_group_layout->addWidget(new QLabel(tr("Port"), host_group));
    host_group_layout->addWidget(mp_edit_port);
    layout->addWidget(host_group, 0, 0, 1, 2);

    mp_edit_query = new QPlainTextEdit(root);
    layout->addWidget(mp_edit_query, 1, 0, 1, 2);
    QPushButton * btn_exec = new QPushButton(tr("&Execute"), root);
    layout->addWidget(btn_exec, 2, 1);
    mp_edit_result = new QTextEdit(root);
    mp_edit_result->setAcceptRichText(false);
    mp_edit_result->setAutoFormatting(QTextEdit::AutoNone);
    mp_edit_result->setReadOnly(true);
    layout->addWidget(mp_edit_result, 3, 0, 1, 2);

    QMenu * menu_help = menuBar()->addMenu(tr("&Help"));
    QAction * action_about_qt = new QAction(tr("About &Qt"), this);
    menu_help->addAction(action_about_qt);

    connect(btn_exec, SIGNAL(clicked()), this, SLOT(execute()));
    connect(action_about_qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
    mp_edit_query->setFocus();
}

void MainWindow::execute()
{
    if(setupMqpClient())
    {
        mp_edit_result->clear();
        mp_mqp_client->query(mp_edit_query->toPlainText());
    }
}

bool MainWindow::setupMqpClient()
{
    if(nullptr != mp_mqp_client && mp_mqp_client->busy())
    {
        QMessageBox::critical(this, this->windowTitle(), "Anoter MQP operation still in progress");
        return false;
    }
    QString host = mp_edit_host->text();
    if(host.isEmpty())
    {
        QMessageBox::critical(this, this->windowTitle(), "A host name is not specified");
        return false;
    }
    QString port_str = mp_edit_port->text();
    if(port_str.isEmpty())
    {
        QMessageBox::critical(this, this->windowTitle(), "A port is not specified");
        return false;
    }
    quint16 port = mp_edit_port->text().toUShort();
    if(nullptr == mp_mqp_client)
    {
        mp_mqp_client = new MqpClient(host, port, this);
        connect(mp_mqp_client, SIGNAL(messageReceived(QString)), this, SLOT(onMessageReceived(QString)));
        return true;
    }
    bool result = mp_mqp_client->trySetHostName(host) && mp_mqp_client->trySetPort(port);
    if(!result)
    {
        QMessageBox::critical(this, this->windowTitle(), "Unable to change a server address");
    }
    return result;
}

void MainWindow::onMessageReceived(const QString & _data)
{
    mp_edit_result->append(_data);
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}


