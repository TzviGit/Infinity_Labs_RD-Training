using System.Net;
using System.Net.Sockets;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

namespace ilrd
{
    [Serializable]
    public class MessagePacket
    {

        public string Id { get; private set; }
        public string Message { get; private set; }

        public MessagePacket(string id, string mesg)
        {
            Id = id;
            Message = mesg;
        }

    }
    public class ChatClient : IDisposable
    {
        static readonly int PORT = 2000;

        static readonly string SERVER_NAME = Dns.GetHostName();

        private TcpClient _tcpClient;
        private string _id;

        public ChatClient(string id, TcpClient client)
        {
            _id = id;
            _tcpClient = client;
        }
        public static void Main()
        {
            Console.WriteLine("Please enter your name id\n\n");
            string id = Console.ReadLine() ?? "noName";

            //TcpClient client = new TcpClient(SERVER_NAME, PORT);
            using (ChatClient client = new ChatClient(id, new TcpClient(SERVER_NAME, PORT)))
            {
                client.RunClient();
            }




        }

        private void RunClient()
        {
            Thread t1 = new Thread(() => WaitToSendMessages());
            Thread t2 = new Thread(() => WaitForMessages());
            t2.Start();
            t1.Start();

            t1.Join();
            t2.Join();
        }

        public void Dispose()
        {
            _tcpClient.Close();
            GC.SuppressFinalize(this);
        }

        private void WaitToSendMessages()
        {
            NetworkStream ns;
            lock (_tcpClient) { ns = _tcpClient.GetStream(); }


            IFormatter formatter = new BinaryFormatter();
            while (true)
            {

                Console.WriteLine("--- Please enter a message, then enter to send ---  to stop write 'exit'");

                string msg = Console.ReadLine() ?? "nothing to say...";

                if (msg.Equals("exit"))
                {
                    _tcpClient.Client.Close();
                    lock (_tcpClient) { _tcpClient.Client.Close(); }
                    break;
                }

                MessagePacket packet = new MessagePacket(_id, msg);



                formatter.Serialize(ns, packet);



            }


        }


        private void WaitForMessages()
        {
            NetworkStream ns;
            lock (_tcpClient) { ns = _tcpClient.GetStream(); }

            IFormatter formatter = new BinaryFormatter();
            while (true)
            {

                {
                    if (!_tcpClient.Connected)
                    {
                        Console.WriteLine($"WTETETEET");
                        break;
                    }
                    try
                    {
                        MessagePacket packet = (MessagePacket)formatter.Deserialize(ns);
                        string id = packet.Id;
                        string msg = packet.Message;



                        Console.WriteLine($"New message from - {id}:  {msg}");
                    }
                    catch (Exception ex) { break; }






                }
            }

            ns.Close();
        }




    }
}
