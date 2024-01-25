
using System.Threading;



namespace ilrd
{
    public class Producer_Consumer
    {
        //int g_intContainer = 0;
        ////SpinLock spinLock = new SpinLock();
        //GenericList<int> g_list = new GenericList<int>();

        //SemaphoreSlim consumerSem = new SemaphoreSlim(0);

        //object _lock = new object();

        //ReaderWriterLockSlim readerWriterLock = new ReaderWriterLockSlim();

        //bool consume = false;

        //public void GlobalIntConcurrency()
        //{
        //    object roundsToProduce = 10000;
        //    Thread producer = new Thread(ProduceInt);
        //    Thread consumer = new Thread(ConsumeInt);
        //    producer.Start(roundsToProduce);
        //    consumer.Start(roundsToProduce);
        //    producer.Join();
        //    consumer.Join();
        //}

        //public void ProduceInt(object? rounds)
        //{
        //    int rounds_ = (int)(rounds ?? 10);

        //    for (int i = 0; i < rounds_; i++)
        //    {
        //        while (consume)
        //        {
        //            //Thread.Sleep(1);
        //        }

        //        g_intContainer++;

        //       Interlocked.MemoryBarrierProcessWide();

        //        consume = true;
        //    }

        //}

        //public void ConsumeInt(object? rounds)
        //{
        //    int rounds_ = (int)(rounds ?? 10);
        //    int localRes = 0;
        //    for (int i = 0; i < rounds_; i++)
        //    {
        //        while (!consume)
        //        {
        //           // Thread.Sleep(1);
        //        }

        //        Interlocked.MemoryBarrierProcessWide();
        //        if (g_intContainer != (localRes + 1))
        //        {
        //            Console.WriteLine("error!!!!");
        //        }
        //        localRes = g_intContainer;
        //       // Console.WriteLine(localRes);

        //        consume = false;
        //    }

        //}

        //public void ProducerConsumerListNoSemaphore()
        //{
        //    Thread consumer = new Thread(ConsumeFromListNoSem);
        //    Thread producer = new Thread(ProduceToListNoSem);
        //    consumer.Start();
        //    producer.Start();
        //    Parallel.Invoke(ProduceToListNoSem, ConsumeFromListNoSem, ProduceToListNoSem,
        //        ProduceToListNoSem, ProduceToListNoSem, ConsumeFromListNoSem, ConsumeFromListNoSem, ConsumeFromListNoSem);


        //    consumer.Join();
        //    producer.Join();

        //}

        //public void ProduceToListNoSem()
        //{
        //    for (int i = 0; i < 100; i++)
        //    {
        //        readerWriterLock.EnterWriteLock();
        //        {
        //            g_list.Push(i);
        //        }
        //        readerWriterLock.ExitWriteLock();
        //    }
        //}

        //public void ConsumeFromListNoSem()
        //{
        //    for (int i = 0; i < 100; i++)
        //    {
        //        readerWriterLock.EnterReadLock();
        //        {
        //            if (!g_list.IsEmpty())
        //            {
        //                Console.WriteLine(g_list.Peek());
        //                g_list.Pop();
        //            }

        //        }
        //        readerWriterLock.ExitReadLock();
        //    }
        //}


        //public void ProducerConsumerListWithSemaphore()
        //{
        //    Parallel.Invoke(ProduceToListWithSem, ConsumeFromListWithSem, ProduceToListWithSem,
        //       ProduceToListWithSem, ProduceToListWithSem, ConsumeFromListWithSem, ConsumeFromListWithSem, ConsumeFromListWithSem);
        //}

        //void ProduceToListWithSem()
        //{
        //    for (int i = 0; i < 100; i++)
        //    {
        //        lock (_lock)
        //        {
        //            g_list.Push(i);
        //        }

        //        consumerSem.Release();
        //    }
        //}

        //public void  ConsumeFromListWithSem()
        //{
        //    for (int i = 0; i < 100; i++)
        //    {
        //        if (consumerSem.Wait(1000))
        //        {
        //            lock (_lock)
        //            {
        //                Console.WriteLine(g_list.Peek());
        //                g_list.Pop();
        //            }
        //        }
        //    }
        //}

        private static int _rounds = 100;
        private static void TestIntContainer()
        {
            IntContainer container = new IntContainer();


            Action A1 = (() =>
            {
                for (int i = 0; i < _rounds; i++)
                {
                    container.Produce(i + 1);
                }
            });


            Action A2 = (() =>
            {
                for (int j = 0; j < _rounds; j++)
                {
                    Console.WriteLine(container.Consume());
                }
            });

            Parallel.Invoke(A1, A2);
           
        }

        private static void TestListNoSem()
        {
            ListContainer list = new ListContainer();

            Action A1 = (() =>
            {
                for (int i = 0; i < 10; i++)
                {
                    list.ProduceNoSemaphore();
                }
            });

            Action A2 = (() =>
            {
                for (int i = 0; i < 10; i++)
                {
                    Console.WriteLine(list.ConsumeNoSemaphore());
                }
            });


            for (int i = 0; i < 4; i++)
            {
                Parallel.Invoke(A1, A2);
            }
        }


        private static void TestListWithSem()
        {
            ListContainer list = new ListContainer();

            Action A1 = (() =>
            {
                for (int i = 0; i < 10; i++)
                {
                    list.ProduceWithSemaphore();
                }
            });

            Action A2 = (() =>
            {
                for (int i = 0; i < 10; i++)
                {
                    Console.WriteLine(list.ConsumeWithSemaphore());
                }
            });


            for (int i = 0; i < 4; i++)
            {
                Parallel.Invoke(A1, A2);
            }
        }



        public static void Main()
        {
            IntContainer intContainer = new IntContainer();
            ListContainer listContainer = new ListContainer();

            TestIntContainer();
            TestListNoSem();
            TestListWithSem();

            
        }
    }
}