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

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtXml/QXmlSimpleReader>
#include <QtXml/QXmlStreamWriter>
#include <MailUnitUI/Config.h>

using namespace MailUnit::Gui;

namespace {

static const QString xml_element_root("MailUnit");
static const QString xml_element_servers("Servers");
static const QString xml_element_window("Window");
static const QString xml_element_geometry("Geometry");
static const QString xml_element_server("Server");
static const QString xml_attr_server_name("name");
static const QString xml_attr_server_host("host");
static const QString xml_attr_server_port("port");
static const QString xml_attr_window_geometry_x("x");
static const QString xml_attr_window_geometry_y("y");
static const QString xml_attr_window_geometry_width("width");
static const QString xml_attr_window_geometry_height("height");

class ConfigPartReader
{
public:
    explicit ConfigPartReader(Config & _config) :
        mr_config(_config)
    {
    }

    virtual ~ConfigPartReader()
    {
    }

    virtual void read(const QString & _elemen_name, const QXmlAttributes & _atts) = 0;

protected:
    Config & mr_config;
}; // class ConfigPartReader


class ServerConfigPartReader : public ConfigPartReader
{
public:
    explicit ServerConfigPartReader(Config & _config) :
        ConfigPartReader(_config)
    {
    }

    void read(const QString & _elemen_name, const QXmlAttributes & _atts) override;
}; // class ServerConfigPartReader


class WindowConfigPartReader : public ConfigPartReader
{
public:
    explicit WindowConfigPartReader(Config & _config) :
        ConfigPartReader(_config)
    {
    }

    void read(const QString & _elemen_name, const QXmlAttributes & _atts) override;
}; // class WindowConfigPartReader


class XmlContentHandler : public QXmlDefaultHandler
{
public:
    explicit XmlContentHandler(Config & _config);
    ~XmlContentHandler() override;
    bool startElement(const QString &, const QString & _local_name, const QString &,
        const QXmlAttributes & _atts) override;
    bool endElement(const QString &, const QString &, const QString &) override;

private:
    static const quint16 s_level_doc      = 0;
    static const quint16 s_level_section  = 1;
    static const quint16 s_level_data     = 2;
    ConfigPartReader * mp_reader;
    Config & mr_config;
    quint16 m_level;
}; // class XmlContentHandler

} // namespace

XmlContentHandler::XmlContentHandler(Config & _config) :
    mp_reader(nullptr),
    mr_config(_config),
    m_level(s_level_doc)
{
}

XmlContentHandler::~XmlContentHandler()
{
    delete mp_reader;
}

bool XmlContentHandler::startElement(const QString &, const QString & _local_name,
    const QString &, const QXmlAttributes & _atts)
{
    switch(m_level++)
    {
    case s_level_section:
        delete mp_reader;
        if(xml_element_servers == _local_name)
            mp_reader = new ServerConfigPartReader(mr_config);
        else if(xml_element_window == _local_name)
            mp_reader = new WindowConfigPartReader(mr_config);
        else
            mp_reader = nullptr;
        break;
    case s_level_data:
        if(nullptr != mp_reader)
            mp_reader->read(_local_name, _atts);
        break;
    default:
        break;
    }
    return true;
}

bool XmlContentHandler::endElement(const QString &, const QString &, const QString &)
{
    --m_level;
    return true;
}

void ServerConfigPartReader::read(const QString & _elemen_name, const QXmlAttributes & _atts)
{
    if(_elemen_name != xml_element_server) return;
    QString name = _atts.value(xml_attr_server_name);
    if(name.isEmpty()) return;
    QString host = _atts.value(xml_attr_server_host);
    if(host.isEmpty()) return;
    QString port_str = _atts.value(xml_attr_server_port);
    if(port_str.isEmpty()) return;
    bool converted = false;
    quint16 port = port_str.toUShort(&converted);
    if(!converted) return;
    mr_config.servers().add(ServerConfig(name, host, port));
}

void WindowConfigPartReader::read(const QString & _elemen_name, const QXmlAttributes & _atts)
{
    if(xml_element_geometry != _elemen_name) return;
    QString x_str = _atts.value(xml_attr_window_geometry_x);
    QString y_str = _atts.value(xml_attr_window_geometry_y);
    QString width_str = _atts.value(xml_attr_window_geometry_width);
    QString height_str = _atts.value(xml_attr_window_geometry_height);
    bool converted = false;
    QPoint position = mr_config.windowPosition();
    if(!x_str.isEmpty())
    {
        int x = x_str.toInt(&converted);
        if(converted) position.setX(x);
    }
    if(!y_str.isEmpty())
    {
        int y = y_str.toInt(&converted);
        if(converted) position.setY(y);
    }
    mr_config.setWindowPosition(position);
    QSize size = mr_config.windowSize();
    if(!width_str.isEmpty())
    {
        int width = width_str.toInt(&converted);
        if(converted) size.setWidth(width);
    }
    if(!height_str.isEmpty())
    {
        int heigth = height_str.toInt(&converted);
        if(converted) size.setHeight(heigth);
    }
    mr_config.setWindowSize(size);
}

Config::Config(const QString & _config_file_path) :
    m_config_file_path(_config_file_path),
    mp_servers(nullptr),
    m_wnd_pos(QPoint(0, 0)),
    m_wnd_size(QSize(800, 600))
{
    reload();
}

Config::~Config()
{
    delete mp_servers;
}

void Config::reload()
{
    delete mp_servers;
    mp_servers = new ServerConfigList();

    QFile file(m_config_file_path);
    if(!file.exists())
    {
        return;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QXmlSimpleReader reader;
    XmlContentHandler handler(*this);
    reader.setContentHandler(&handler);
    reader.parse(&file);
    file.close();
}

bool Config::save()
{
    QFile file(m_config_file_path);
    QFileInfo file_info(file);
    if(!file_info.dir().mkpath(file_info.dir().path()))
    {
        return false;
    }
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(!file.isOpen())
    {
        return false;
    }
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement(xml_element_root);
    {
        writer.writeStartElement(xml_element_servers);
        int srever_count = mp_servers->size();
        for(int i = 0; i < srever_count; ++i)
        {
            const ServerConfig & server = mp_servers->at(i);
            writer.writeStartElement(xml_element_server);
            writer.writeAttribute(xml_attr_server_name, server.name());
            writer.writeAttribute(xml_attr_server_host, server.host());
            writer.writeAttribute(xml_attr_server_port, QString::number(server.port()));
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
    {
        writer.writeStartElement(xml_element_window);
        writer.writeStartElement(xml_element_geometry);
        writer.writeAttribute(xml_attr_window_geometry_x, QString::number(m_wnd_pos.x()));
        writer.writeAttribute(xml_attr_window_geometry_y, QString::number(m_wnd_pos.y()));
        writer.writeAttribute(xml_attr_window_geometry_width, QString::number(m_wnd_size.width()));
        writer.writeAttribute(xml_attr_window_geometry_height, QString::number(m_wnd_size.height()));
        writer.writeEndElement();
        writer.writeEndElement();
    }
    writer.writeEndElement();
    file.close();
    return true;
}
