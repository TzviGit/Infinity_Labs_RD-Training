using System.IO.Pipes;

namespace ilrd
{
    public class NamedPipeCommunicator : IDisposable
    {
        private bool _wasDisposed;


        readonly NamedPipeServerStream _pipeStream;
        readonly StreamReader _reader;
        readonly StreamWriter _writer;

        public string PipeName { get; private set; }

        public NamedPipeCommunicator(string pipeName_, PipeDirection pipeDirection)
        {
            _wasDisposed = false;
            PipeName = pipeName_;
            _pipeStream = new NamedPipeServerStream(pipeName_, pipeDirection);
            _reader = new StreamReader(_pipeStream);
            _writer = new StreamWriter(_pipeStream);
            _writer.AutoFlush = true;
        }


        public string ReceiveMessage()
        {
            return _reader.ReadToEnd();
        }

        public void SendMessage(string message)
        {
            _writer.WriteLine(message);
        }

        public void WaitForConection()
        {
            _pipeStream.WaitForConnection();
        }
        protected virtual void Dispose(bool disposing)
        {
            if (!_wasDisposed)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects)
                    _pipeStream.Dispose();
                    _reader.Dispose();
                    _writer.Dispose();

                }

                // TODO: free unmanaged resources (unmanaged objects) and override finalizer
                // TODO: set large fields to null
                _wasDisposed = true;
            }
        }

        // todo: override finalizer only if 'dispose(bool disposing)' has code to free unmanaged resources
        ~NamedPipeCommunicator()
        {
            // do not change this code. put cleanup code in 'dispose(bool disposing)' method
            Dispose(disposing: true);
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
}
