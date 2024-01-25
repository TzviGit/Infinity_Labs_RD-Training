using System.IO;


namespace ilrd
{
    public class FileComparer
    {
        public static void Main(string[] args)
        {
            string path1, path2;

            // If no args paths are given then Create an Example usage.
            ConfigurePathSubstance(args, out path1, out path2);

            CompareFiles(path1, path2);

        }

        private static void ConfigurePathSubstance(string[] args, out string path1, out string path2)
        {
            if (args.Length < 2)
            {
                path1 = "example1.txt";
                path2 = "example2.txt";

                using StreamWriter sw1 = new StreamWriter(new FileStream(path1, FileMode.Truncate, FileAccess.ReadWrite));
                using StreamWriter sw2 = new StreamWriter(new FileStream(path2, FileMode.Truncate, FileAccess.ReadWrite));

                for (int i = 0; i < 10; i++)
                {
                    sw1.WriteLine("I Love Infinity # {0}", i);
                    if ((i % 2) == 0)
                    {
                        sw2.WriteLine("I Love Infinity # {0}", i);
                    }
                    else
                    {
                        sw2.WriteLine("hello");
                    }
                }
            } // end scope of "using"
            else
            {
                path1 = args[0];
                path2 = args[1];
            }
        }

        public static void CompareFiles(string file1, string file2)
        {
            string[] lines1 = File.ReadAllLines(file1);
            string[] lines2 = File.ReadAllLines(file2);
            var res = lines1.Zip(lines2, (n, w) => new { first = n, second = w });


            foreach (var line in res)
            {
                if (line.first != line.second)
                {
                    Console.WriteLine(line.first);
                }
            }
           
            
        }

    }
}
            


                

           
           
