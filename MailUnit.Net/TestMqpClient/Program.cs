using System;
using System.Text;
using MailUnit.Net;
using System.Threading.Tasks;

namespace TestMqpClient {
    class TestClient {
        string hostname;
        ushort port;
        
        public TestClient(string hostname, ushort port) {
            this.hostname = hostname;
            this.port = port;
        }
        public async Task ExecuteQuery(string query) {
            Console.Write("Executing query: \"");
            Console.Write(query);
            Console.WriteLine("\"");
            var response = await MqpClient.SendQueryAsync(hostname, port, query);
            Console.WriteLine("RESPONSE: ");
            Console.Write("Status: ");
            Console.WriteLine(response.Header.Status);
            Console.Write("Action: ");
            Console.WriteLine(response.Header.Action);
            Console.Write("Affected: ");
            Console.WriteLine(response.Header.AffectedCount);
            while(!response.IsCompleted) {
                var message = await response.GetNextMessage();
                Console.WriteLine("\r\n___________________________________\r\n");
                Console.Write("ID: ");
                Console.WriteLine(message.Header.Id);
                Console.Write("Subject: ");
                Console.WriteLine(message.Header.Subject);
                Console.Write("Size: ");
                Console.WriteLine(message.Header.Size);
                foreach(string @from in message.Header.From) {
                    Console.Write("From: ");
                    Console.WriteLine(@from);
                }
                foreach(string to in message.Header.To) {
                    Console.Write("To: ");
                    Console.WriteLine(to);
                }
                foreach(string cc in message.Header.Cc) {
                    Console.Write("CC: ");
                    Console.WriteLine(cc);
                }
                foreach(string bcc in message.Header.Bcc) {
                    Console.Write("BCC: ");
                    Console.WriteLine(bcc);
                }
                Console.WriteLine();
                Console.WriteLine(Encoding.UTF8.GetString(message.Body));
            }
        }
    }
    
    class MainClass {
        public static void Main(string[] args) {
            if(args.Length < 3) {
                Console.WriteLine("Too few arguments");
                return;
            }
            string hostname = args[0];
            ushort port;
            if(!UInt16.TryParse(args[1], out port)) {
                Console.WriteLine("Invalid port number");
                return;
            }
            string query = args[2];
            try {
                var client = new TestClient(hostname, port);
                client.ExecuteQuery(query).Wait();
            } catch(Exception error) {
                Console.WriteLine(error.ToString());
            }
            switch(Environment.OSVersion.Platform) {
                case PlatformID.Win32NT:
                case PlatformID.Win32S:
                case PlatformID.Win32Windows:
                case PlatformID.WinCE:
                    Console.WriteLine();
                    Console.Write("Press any key...");
                    Console.ReadKey();
                    break;
            }
        }
    }
}
