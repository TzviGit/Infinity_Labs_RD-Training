
using System.Diagnostics;


namespace AsyncMain
{
    class Program
    {
        static async Task Main(string[] args)
        {
            var sw = new Stopwatch();
            sw.Start();

            Console.WriteLine("task 1");
            Task task1 = doWork();

            Console.WriteLine("task 2");
            Task task2 = doWork();

            Console.WriteLine("task 3");
            Task task3 = doWork();

            await Task.WhenAll(task1, task2, task3);

            Console.WriteLine("Tasks finished");

            sw.Stop();

            var elapsed = sw.ElapsedMilliseconds;
            Console.WriteLine($"elapsed: {elapsed} ms");
        }

        static async Task doWork()
        {
            await Task.Delay(1500);
        }
    }
}