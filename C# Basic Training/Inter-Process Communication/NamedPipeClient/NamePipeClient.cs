using System.IO.Pipes;


namespace ilrd
{
    public class NamePipeClient
    {
        private static readonly string _clientMessage = "Ping";

        public static void Main()
        {
            try
            {
                //Process.Start("http://www.c-sharpcorner.com");


                StartClient();

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

        }
        static void StartClient()
        {
            using NamedPipeClientStream client = new NamedPipeClientStream(".", "MyNamedPipe", PipeDirection.InOut);


            client.Connect(timeout: 30000);

            int rounds = 10;
            StartPingPong(client, rounds);

        }

        static void StartPingPong(NamedPipeClientStream client, int rounds)
        {
            using StreamReader reader = new StreamReader(client);
            using StreamWriter writer = new StreamWriter(client);
            writer.AutoFlush = true;

            for (int i = 0; i < rounds; i++)
            {
                Console.WriteLine($"CLient is now sending out message - {_clientMessage}");
                writer.WriteLine(_clientMessage);

#pragma warning disable CA1416 // Validate platform compatibility
                client.WaitForPipeDrain();
#pragma warning restore CA1416 // Validate platform compatibility

                Console.WriteLine($"Client has recieved message -> {reader.ReadLine()}");
            }

        }

    }
}
