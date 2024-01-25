using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ilrd
{
    internal class CBTester
    {
        static void Main()
        {
            Console.WriteLine("STARTING TEST");
            int iterations = 100;
            CircleBuffer<int> cb = new CircleBuffer<int>(10);

            Func<int, bool> pusher = (int i) => cb.TryPush(i);
            Func<int,bool> popper = (int i) => cb.TryPop(out i);


            //Func<bool> peeker = () =>
            //{

            //    bool success = cb.Front(out int p);
            //    if (success) { Console.WriteLine(p); }
            //    return success;
            //};

            Parallel.Invoke(
                () =>
                {
                    for (int i = 0; i < iterations; i++)
                    {
                        Thread.Sleep(1);
                        cb.TryPush(i); //pusher(i);
                    }
                },
                () =>
                {
                    for (int i = 0; i < iterations; i++)
                    {
                        Thread.Sleep(1);
                        bool success = cb.TryPop(out int p);
                        if (success)
                        {
                            Console.WriteLine( p); 
                            
                        }
                    }
                });
        }
    }
}
