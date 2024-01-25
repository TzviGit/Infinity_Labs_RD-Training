using System.IO.MemoryMappedFiles;
using System.IO.Pipes;

namespace ilrd
{
    public class Consumer
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Interoperability", "CA1416:Validate platform compatibility", Justification = "I know what Im doing")]
        public static void Main()
        {
           try
            {
                using NamedPipeClientStream syncronizerPipe = new NamedPipeClientStream(".", "myNamedPipe", PipeDirection.InOut);
                syncronizerPipe.Connect();
                
                using StreamReader pipeReader = new StreamReader(syncronizerPipe);
                

                using  MemoryMappedFile memMapFile = MemoryMappedFile.OpenExisting("myFile", MemoryMappedFileRights.ReadWrite);
                using MemoryMappedViewAccessor reader = memMapFile.CreateViewAccessor();


                for (int i = 0; i < 10; i++)
                { 
                    pipeReader.ReadLine();
                    reader.ReadInt32(sizeof(int) * i);
                    Console.WriteLine($"Consumer just consumed the number - {reader.ReadInt32(sizeof(int) * i)}");
                }
                   
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
           
        }
    }
}

           
