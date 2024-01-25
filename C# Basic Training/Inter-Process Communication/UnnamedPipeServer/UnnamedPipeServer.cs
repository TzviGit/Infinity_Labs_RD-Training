
using System.Diagnostics;
using System.IO.Pipes;

namespace ilrd
{
    public class UnnamedPipeServer
    {
        public static void Main(string[] args)
        {
            try
            { 
                using AnonymousPipeServerStream pipeServer = new AnonymousPipeServerStream(PipeDirection.Out, HandleInheritability.Inheritable);
                

                Process pipeClient = new Process();

                pipeClient.StartInfo.FileName = @"C:\Users\User\source\repos\tzvi-moskowitz\c#\IPC\UnnamedPipeClient\bin\Debug\net6.0\UnnamedPipeClient.exe";
               
                Console.WriteLine("[SERVER] Current TransmissionMode: {0}.", pipeServer.TransmissionMode);

                // Pass the client process a handle to the server.
                pipeClient.StartInfo.Arguments = pipeServer.GetClientHandleAsString();
                pipeClient.StartInfo.UseShellExecute = false;
                pipeClient.Start();

               // pipeServer.DisposeLocalCopyOfClientHandle();

                int rounds = 10;
                StartServer(pipeServer, rounds);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

        }

        public static void StartServer(AnonymousPipeServerStream pipeServer, int rounds)
        {
            using StreamReader reader = new StreamReader(pipeServer);
            using StreamWriter writer = new StreamWriter(pipeServer);

            for (int i = 0; i < rounds; i++)
            {
                Console.WriteLine($"Server just got request - {reader.ReadLine()} - from Client");

                writer.WriteLine("Ping");
                Console.WriteLine("Server just sent Message \"Pong\" to Client");
            }


        }
    }
}