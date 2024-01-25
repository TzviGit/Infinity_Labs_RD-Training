/*
 * Description: 
 * Auther: Tzvi Moskowitz4
 * 
 * 
 * 
 * 
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ilrd
{
    public class IntContainer
    {
        private int _container = 0;

        private  bool _consume = false;


        public void Produce(int value)
        {
            while (_consume) { }

            Interlocked.MemoryBarrierProcessWide();
            Interlocked.Exchange(ref _container, value);
            Interlocked.MemoryBarrierProcessWide();

            _consume = true;
        }

        public int Consume()
        {
            while (!_consume) { }

            Interlocked.MemoryBarrierProcessWide();
            int value  = _container;
            Interlocked.MemoryBarrierProcessWide();

            _consume = false;

            return value;
        }

    }

    public class ListContainer
    {
        private GenericList<int> _list = new GenericList<int>();

        private int _currVal = 0;

        private object _lock = new object();

        private SemaphoreSlim _consumeSem = new SemaphoreSlim(0);

        private ReaderWriterLockSlim _R_W_lock = new ReaderWriterLockSlim();


        public void ProduceNoSemaphore()
        {
            _R_W_lock.EnterWriteLock();
            _list.Push(_currVal++);
            _R_W_lock.ExitWriteLock();
        }

        public int ConsumeNoSemaphore()
        {
            int value = int.MinValue;
            _R_W_lock.EnterReadLock();

            if (!_list.IsEmpty())
            {
                value = _list.Peek();
                _list.Pop();

             }

            _R_W_lock.ExitReadLock();
            return value;
        }

        public void ProduceWithSemaphore()
        {
            lock (_lock)
            {
                _list.Push(_currVal++);
                Interlocked.MemoryBarrierProcessWide();
            }

            _consumeSem.Release();
        }

        public int ConsumeWithSemaphore()
        {
            int value = -1;

           if( _consumeSem.Wait(100))
            {
                lock (_lock)
                {
                    value = _list.Peek();
                    _list.Pop();
                }
            }

           return value;
            
        }

    }


}
