using System;
using System.Diagnostics;
using System.IO.Pipes;
using System.IO;

namespace ilrd
{
    public class UnnamedPipeServer
    {
        public static void Main()
        {
            Process clientProcess = new Process();
            try
            {
                using AnonymousPipeServerStream pipeToClient = new AnonymousPipeServerStream(PipeDirection.Out, HandleInheritability.Inheritable);
                using AnonymousPipeServerStream pipeFromClient = new AnonymousPipeServerStream(PipeDirection.In, HandleInheritability.Inheritable);

                string[] clientArgs = new string[2] { pipeToClient.GetClientHandleAsString(), pipeFromClient.GetClientHandleAsString() };

                clientProcess.StartInfo.FileName = @"C:\Users\User\source\repos\tzvi-moskowitz\c#\IPC\UnnamedPipeClient\bin\Debug\net6.0\UnnamedPipeClient";
                clientProcess.StartInfo.Arguments = pipeToClient.GetClientHandleAsString() + " " + pipeFromClient.GetClientHandleAsString();
                clientProcess.StartInfo.UseShellExecute = false;
                clientProcess.Start();


                pipeToClient.DisposeLocalCopyOfClientHandle();

                // Process.Start(@"C:\Users\User\source\repos\tzvi-moskowitz\c#\IPC\UnnamedPipeClient\bin\Debug\net6.0\UnnamedPipeClient", clientArgs);

                int rounds = 10;



                StartPingPong(pipeToClient, pipeFromClient, rounds);

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                clientProcess.WaitForExit();
                clientProcess.Close();

            }
        }
               

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Interoperability", "CA1416:Validate platform compatibility", Justification = "<I know what I'm doing!>")]
        public static void StartPingPong(PipeStream pipeToClient, PipeStream pipeFromClient, int rounds)
        {
            using StreamReader reader = new StreamReader(pipeFromClient);
            using StreamWriter writer = new StreamWriter(pipeToClient);
            writer.AutoFlush = true;

            for (int i = 0; i < rounds; i++)
            { 
                Console.WriteLine($"Server just Recieved message: - {reader.ReadLine()}");
                
                Console.WriteLine("Server is sending message \"Pong\" to Client");
                writer.WriteLine("Pong");
                
                pipeToClient.WaitForPipeDrain(); //no Warning because of Atribute
                
            }

        }

    }
}

           
                
                

     

