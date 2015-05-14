/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.Net Library.                                                  *
 *                                                                                             *
 * MailUnit.Net Library is free software: you can redistribute it and/or modify it under       *
 * the terms of the GNU Lesser General Public License as published by the Free Software ,      *
 * Foundation either version 3 of the License, or (at your option) any later version.          *
 *                                                                                             *
 * MailUnit.Net Library is distributed in the hope that it will be useful, but WITHOUT ANY     *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  *
 * PURPOSE. See the GNU Lesser General Public License for more details.                        *
 *                                                                                             *
 * You should have received a copy of the GNU License General Public License along with        *
 * MailUnit.Net Library. If not, see <http://www.gnu.org/licenses>.                            *
 *                                                                                             *
 ***********************************************************************************************/

using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MailUnit.Net.Private;

namespace MailUnit.Net {
    namespace Private {
        static class NetworkStreamExt {
            public static void ReadMqpHeader(this NetworkStream stream, Action<string> lineHandler) {
                var lineBuffer = new List<byte>(128);
                while(true) {
                    byte readedByte = unchecked((byte)stream.ReadByte());
                    if(readedByte != '\n') {
                        lineBuffer.Add(readedByte);
                        continue;
                    }
                    string line = Encoding.UTF8.GetString(lineBuffer.ToArray(), 0, lineBuffer.Count);
                    if(String.IsNullOrWhiteSpace(line)) {
                        return;
                    }
                    lineHandler(line);
                    lineBuffer.Clear();
                }
            }
        }
    }
    
    public enum MqpResponseStatus {
        Success       = 100,
        UnknownError  = 200,
        ParseError    = 201,
        StorageError  = 202,
        TimeOut       = 203
    }
    
    public enum MqpResponseAction {
        Error,
        Matched,
        Deleted
    }
    
    public class MqpResponseHeader {
        const string statusPrefix  = "STATUS: ";
        const string matchedPrefix = "MATCHED: ";
        const string deletedPrefix = "DELETED: ";
        MqpResponseStatus status = MqpResponseStatus.UnknownError;
        MqpResponseAction action = MqpResponseAction.Error;
        uint affectedCount = 0;

        public MqpResponseAction Action {
            get { return action; }
        }
        public MqpResponseStatus Status {
            get { return status; }
        }
        public uint AffectedCount {
            get { return affectedCount; }
        }        
        public bool ParseHeaderLine(string headerLine) {
            int length = headerLine.IndexOf('\r');
            if(length <= 0) {
                return false;
            }
            string trimmedLine = headerLine.Substring(0, length);
            if(trimmedLine.StartsWith(statusPrefix)) {
                MqpResponseStatus status;
                if(Enum.TryParse<MqpResponseStatus>(trimmedLine.Substring(statusPrefix.Length), out status)) {
                    this.status = status;
                    return true;
                }
                return false;
            }
            if(trimmedLine.StartsWith(matchedPrefix)) {
                action = MqpResponseAction.Matched;
                return UInt32.TryParse(trimmedLine.Substring(matchedPrefix.Length), out affectedCount);
            } else if(trimmedLine.StartsWith(deletedPrefix)) {
                action = MqpResponseAction.Deleted;
                return UInt32.TryParse(trimmedLine.Substring(deletedPrefix.Length), out affectedCount);
            } else {
                return false;
            }
        }        
    }
    
    public class MqpResponseMessageHeader {
        const string itemPrefix    = "ITEM: ";
        const string sizePrefix    = "SIZE: ";
        const string idPrefix      = "ID: ";
        const string subjectPrefix = "SUBJECT: ";
        const string fromPrefix    = "FROM: ";
        const string toPrefix      = "TO: ";
        const string ccPrefix      = "CC: ";
        const string bccPrefix     = "BCC: ";
        uint id;
        uint size;
        string subject;
        List<string> from = new List<string>();
        List<string> to = new List<string>();
        List<string> cc = new List<string>();
        List<string> bcc = new List<string>();
        
        public bool ParseHeaderLine(string line) {
            string trimmedLine = line.Trim();
            if(trimmedLine.StartsWith(itemPrefix)) {
                // TODO: parse
            } else if(trimmedLine.StartsWith(idPrefix)) {
                return UInt32.TryParse(trimmedLine.Substring(idPrefix.Length), out id);
            } else if(trimmedLine.StartsWith(sizePrefix)) {
                return UInt32.TryParse(trimmedLine.Substring(sizePrefix.Length), out size);
            } else if(trimmedLine.StartsWith(subjectPrefix)) {
                subject = trimmedLine.Substring(subjectPrefix.Length);
            } else if(trimmedLine.StartsWith(fromPrefix)) {
                from.Add(trimmedLine.Substring(fromPrefix.Length));
            } else if(trimmedLine.StartsWith(toPrefix)) {
                to.Add(trimmedLine.Substring(toPrefix.Length));
            } else if(trimmedLine.StartsWith(ccPrefix)) {
                cc.Add(trimmedLine.Substring(ccPrefix.Length));
            } else if(trimmedLine.StartsWith(bccPrefix)) {
                bcc.Add(trimmedLine.Substring(bccPrefix.Length));
            } else {
                return false;
            }
            return true;
        }
        public uint Id {
            get { return id; }
        }
        public uint Size {
            get { return size; }
        }
        public string Subject {
            get { return subject; }
        }
        public List<string> From {
            get { return from; }
        }
        public List<string> To {
            get { return to; }
        }
        public List<string> Cc {
            get { return cc; }
        }
        public List<string> Bcc {
            get { return bcc; }
        }
    }
    
    public class MqpResponseMessage {
        readonly MqpResponseMessageHeader header;
        readonly byte[] body;
        
        public MqpResponseMessage(MqpResponseMessageHeader header, byte[] body) {
            this.header = header;
            this.body = body;
        }
        public MqpResponseMessageHeader Header {
            get { return header; }
        }
        public byte[] Body {
            get { return body; }
        }
    }
    
    public class MqpResponse {
        readonly MqpResponseHeader header;
        readonly NetworkStream stream;
        long isCompleted;
        long receivedMessageCount;
        
        public event EventHandler Completed;
        
        public MqpResponse(MqpResponseHeader header, NetworkStream stream) {
            this.header = header;
            this.stream = stream;
            isCompleted = 
                (header.Status != MqpResponseStatus.Success ||
                header.Action != MqpResponseAction.Matched ||
                header.AffectedCount == 0) ? 1 : 0;
        }
        public MqpResponseHeader Header {
            get { return header; }
        }
        public bool IsCompleted {
            get { return Interlocked.Read(ref isCompleted) != 0; }
        }
        public Task<MqpResponseMessage> GetNextMessage() {
            if(Interlocked.Read(ref isCompleted) != 0) {
                return null;
            }
            var task = new Task<MqpResponseMessage>(() => {
                var messageHeader = new MqpResponseMessageHeader();
                stream.ReadMqpHeader(line => {
                    messageHeader.ParseHeaderLine(line);
                });
                byte[] body = new byte[messageHeader.Size];
                if(messageHeader.Size > 0) {
                    stream.Read(body, 0, body.Length);
                }
                return new MqpResponseMessage(messageHeader, body);
            });
            if(Interlocked.Increment(ref receivedMessageCount) == header.AffectedCount) {
                Interlocked.Exchange(ref isCompleted, 1);
                task.ContinueWith(completedTask => new Task(() => {
                    if(Completed != null) {
                        Completed(this, EventArgs.Empty);
                    }
                }));
            }
            task.Start();
            return task;
        }
    }
    
    public static class MqpClient {
        public static Task<MqpResponse> SendQueryAsync(string hostname, ushort port, string query) {
            return Task<MqpResponse>.Run(() => {
                var client = new TcpClient();
                NetworkStream stream = null;
                try {
                    client.Connect(hostname, port);
                    stream = client.GetStream();
                    SendQuery(stream, query);
                    var header = new MqpResponseHeader();
                    stream.ReadMqpHeader(line => {
                        header.ParseHeaderLine(line);
                    });
                    var response = new MqpResponse(header, stream);
                    response.Completed += (s, e) => {
                        SendQuitQuery(stream);
                        client.Close();
                        stream.Close();
                    };
                    return response;
                } catch {
                    client.Close();
                    if(stream != null) {
                        stream.Close();
                    }
                    throw;
                } 
            });
        }
        static void SendQuery(NetworkStream stream, string query) {
            string preparedQuery = query.Trim();
            if(!preparedQuery.EndsWith(";")) {
                preparedQuery += ';';
            }
            var queryBuffer = Encoding.UTF8.GetBytes(preparedQuery);
            stream.Write(queryBuffer, 0, queryBuffer.Length);
        }
        static void SendQuitQuery(NetworkStream stream) {
            byte[] quitQuery = Encoding.UTF8.GetBytes("q;");
            stream.Write(quitQuery, 0, quitQuery.Length);
        }
    }
}
