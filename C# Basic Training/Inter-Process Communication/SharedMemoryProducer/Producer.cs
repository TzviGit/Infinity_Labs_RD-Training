
using System.IO.MemoryMappedFiles;
using System.IO.Pipes;
using System.Diagnostics;


namespace ilrd
{
    public class Producer
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Interoperability", "CA1416:Validate platform compatibility", Justification = "<Pending>")]
        public static void Main()
        {
            try
            {
                using NamedPipeServerStream syncronizerPipe = new NamedPipeServerStream("myNamedPipe", PipeDirection.InOut); 

                using MemoryMappedFile memMapFile = MemoryMappedFile.CreateOrOpen("myFile", 1000, MemoryMappedFileAccess.ReadWrite);
                
                using MemoryMappedViewAccessor writer = memMapFile.CreateViewAccessor();

                Process.Start(@"C:\Users\User\source\repos\tzvi-moskowitz\c#\IPC\SharedMemoryConsumer\bin\Debug\net6.0\SharedMemoryConsumer");
                syncronizerPipe.WaitForConnection();

                using  StreamWriter pipeWriter = new StreamWriter(syncronizerPipe);
                
                pipeWriter.AutoFlush = true;

                for (int i = 0; i < 10; i++)
                {
                    writer.Write(sizeof(int) * i, (int)i + 1);

                    Console.WriteLine($"Pruducer just produced the number - {i + 1}");
                    pipeWriter.WriteLine("your turn");

                    syncronizerPipe.WaitForPipeDrain();
                }

                
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }


        }
    }
}
               
                
                 
