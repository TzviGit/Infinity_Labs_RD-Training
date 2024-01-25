using System.Net;
using System.Net.Sockets;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;


namespace ilrd
{
    public class ChatServer
    {
        public TcpListener Listener { get; private set; }
        private readonly HashSet<NetworkStream> sockets = new HashSet<NetworkStream>();
        private static readonly int PORT = 2000;
        private object _lock = new object();
        public ChatServer(TcpListener listener) => Listener = listener;
        public static void Main()
        {
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, PORT);
            ChatServer server = new ChatServer(new TcpListener(endPoint));

            server.Listener.Start(100);

            Thread handler = new Thread(server.StartConnectionHandler);
            handler.Start();

            //do other work ......
            //
            //  MAKE SOME COFFE !!
            ///////////////////
            ///
            handler.Join();

        }

        private void StartConnectionHandler()
        {
            while (true)
            {

                TcpClient client = Listener.AcceptTcpClient();
                Task.Run(() => AcceptRequests(client));


            }
        }

        private void AcceptRequests(TcpClient client)
        {
            lock (_lock) { sockets.Add(client.GetStream()); }

            string UserName = HandleFirstPacketAndSaveNameId(client);

            /****** HANDLE REANAINDER OF CHAT ********/
            HandleChat(client);

            BroadcastMessage(new MessagePacket("SERVER", $"{UserName} just Left the Server"));
        }

        private string HandleFirstPacketAndSaveNameId(TcpClient client)
        {
            string userName = "NoName";
            try
            {
                MessagePacket firstPacket = ReceivePacket(client);

                /************  SAVE USER NAME  *********/

                userName = firstPacket.Id;

                /*********  Anounce users arrival **********/
                MessagePacket anounceArrival = new("SERVER", $"{userName} just Entered the server!");
                BroadcastMessage(anounceArrival);


                BroadcastMessage(firstPacket);

            }
            catch (Exception)
            {
                lock (_lock) { sockets.RemoveWhere(x => (!x.Socket.Connected)); }

            }

            return userName;
        }


        private void HandleChat(TcpClient client)
        {
            IFormatter formatter = new BinaryFormatter();
            try
            {
                while (true)
                {
                    MessagePacket packet = ReceivePacket(client);
                    BroadcastMessage(packet);
                }
            }
            catch (Exception)
            {
                lock (_lock) { sockets.RemoveWhere(x => (!x.Socket.Connected)); }

            }


        }

        private MessagePacket ReceivePacket(TcpClient client)
        {
            IFormatter formatter = new BinaryFormatter();
            return (MessagePacket)formatter.Deserialize(client.GetStream());
        }
        private void BroadcastMessage(MessagePacket packet)
        {
            IFormatter formatter = new BinaryFormatter();
            lock (_lock)
            {
                sockets.RemoveWhere(x => (!x.Socket.Connected));

                foreach (var socket in sockets)
                {
                    formatter.Serialize(socket, packet);
                }
            }
        }

    }

}























