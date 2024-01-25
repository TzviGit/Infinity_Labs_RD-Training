namespace ilrd
{
    public class CircleBuffer<T> : IDisposable
    {
        private readonly T[] _internal;
        private readonly object _writeLock = new object();
        private readonly object _readLock = new object();

        private int _readIndex = 0, _writeIndex = 0;
        SemaphoreSlim _consumeSem = new SemaphoreSlim(0);
        SemaphoreSlim _productionSem;

        private int _count = 0;
        public CircleBuffer(int capacity)
        {
            _internal = new T[capacity];
            _productionSem = new SemaphoreSlim(capacity);
        }


        public bool TryPush(T data)
        {
            bool success = false;   

            if (_productionSem.Wait(millisecondsTimeout: 0))
            {
                lock (_writeLock)
                {
                    _internal[_writeIndex++] = data;
                    NormalizeIndex(ref _writeIndex);

                }

                success = true;
                _consumeSem.Release();
            }
            
            return success;
        }

        public bool TryPeek(out T? data)
        {
            data = default;

            bool success = false;
            if (_consumeSem.CurrentCount != 0)
            {
                lock (_readLock)
                {
                    data = _internal[_readIndex];
                }
                success = true;
            }

            return success;
            
        }


        public bool TryPop(out T? outParam)
        {
            outParam = default;

            bool success = false;
            if (_consumeSem.Wait(millisecondsTimeout: 0))
            {
                lock (_readLock)
                {
                    outParam = _internal[_readIndex++];
                    NormalizeIndex(ref _readIndex);
                }

                success = true;
                _productionSem.Release();
            }

            return success;
        }

        private void NormalizeIndex(ref int index)
        {
            index %= _internal.Length;
        }

        public void Dispose()
        {
           _productionSem.Dispose();
           _consumeSem.Dispose();
            GC.SuppressFinalize(this);
        }
    }
}