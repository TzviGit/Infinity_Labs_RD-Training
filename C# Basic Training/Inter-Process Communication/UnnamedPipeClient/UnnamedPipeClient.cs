using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO.Pipes;

namespace ilrd
{
    public class UnnamedPipeReciever
    {
        public static void Main(string[] args)
        {
            try
            {
                if (args.Length > 2)
                {
                    throw new ArgumentException("need to give pipe handle as argument");
                }

                Console.WriteLine(args[0]);
                Console.WriteLine(args[1]);

                using AnonymousPipeClientStream pipeFromServer = new AnonymousPipeClientStream(PipeDirection.In, args[0]);
                using AnonymousPipeClientStream pipeToServer = new AnonymousPipeClientStream(PipeDirection.Out, args[1]);

                
                int rounds = 10;

                
                StartPingPong( pipeToServer,  pipeFromServer, rounds);

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Interoperability", "CA1416:Validate platform compatibility", Justification = "<I know what I'm doing!>")]
        public static void StartPingPong( PipeStream pipeToServer,  PipeStream pipeFromServer, int rounds)
        {
            using StreamWriter writer = new StreamWriter(pipeToServer);
            writer.AutoFlush = true;
            using StreamReader reader = new StreamReader(pipeFromServer);

            for (int i = 0; i < rounds; i++)
            {
                Console.WriteLine("Client is sending message \"Ping\" to server");
                writer.WriteLine("Ping");
               
                pipeToServer.WaitForPipeDrain(); //no Warning because of Atribute
               
                Console.WriteLine($"Client just Recieved message: - {reader.ReadLine()}");
                
            }

        }
    }
}
                

                

