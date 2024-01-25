using System;
using System.Net;
using System.Net.Http;
using System.IO;

namespace Downloader
{
    public class Downloader
    {
        static void Main()
        {
           FileStream fd = new FileStream("text.html",FileMode.Create);
           fd.Close();
           DownloadFile(@"https://docs.microsoft.com/en-us/dotnet/standard/threading/the-managed-thread-pool", "text.HTML");

            Console.WriteLine(new StreamReader(new FileStream("text.html",FileMode.Open)).ReadToEnd());
        }


        public static void DownloadFile(string remoteFilename, string localFilename)
        {
            WebClient client = new WebClient();
            client.DownloadFile(remoteFilename, localFilename);
        }
    }



}

