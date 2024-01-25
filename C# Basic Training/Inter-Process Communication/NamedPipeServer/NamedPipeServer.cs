using System.Diagnostics;
using System.IO.Pipes;
namespace ilrd
{

    public class NamedPipeServer
    {
        protected readonly static string serverMessage = "Pong";

        public static void Main(string[] args)
        {
            try
            {
                if (args.Length == 0)
                {
                    Process.Start(@"C:\Users\User\source\repos\tzvi-moskowitz\c#\IPC\NamedPipeClient\bin\Debug\net6.0\NamedPipeClient.exe");
                }

                StartServer();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

        }
        static void StartServer()
        {
            using NamedPipeServerStream server = new NamedPipeServerStream("myNamedPipe", PipeDirection.InOut);

            //wait for client to connect
            server.WaitForConnection();

            int rounds = 10;
            StartPingPong(server, rounds);
       
        }

        static void StartPingPong(NamedPipeServerStream server, int rounds)
        {
            using StreamReader reader = new StreamReader(server);
            using StreamWriter writer = new StreamWriter(server);
            writer.AutoFlush = true;

            for (int i = 0; i < rounds; i++)
            {
                Console.WriteLine("Server has recieved message -> {0}", reader.ReadLine());

                Console.WriteLine($"Server is now sending out message - {serverMessage}");

                writer.WriteLine(serverMessage);
            }

        }





    }
}


























