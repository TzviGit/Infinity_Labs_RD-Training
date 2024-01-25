namespace ilrd
{
    public class ObjectsStack
    {
        private readonly object[] m_internal;
        private ulong m_index;
        private readonly ulong m_capacity;
        public ObjectsStack(ulong capacity = 10)
        {
            m_internal = new object[capacity];
            m_index = 0;
            m_capacity = capacity;
        }

        public bool IsEmpty()
        {
            return m_index == 0;  // O(1) time complexity
        }

        public void Push(object o) //will throw exception if full
        {
            if (m_index == m_capacity)
            {
                throw new IndexOutOfRangeException("capacity is full!");
            }

            m_internal[m_index++] = o;
        }

        public void Pop()
        {
            if (m_index != 0)
            {
                --m_index;
            }

        }

        public object Peek()
        {
            if (IsEmpty())
            {
                throw new AccessViolationException("stack is empty!");
            }

            return m_internal[m_index - 1];
        }
        public object[] Internal
        {
            get { return m_internal; }
        }

        public void Clear()
        {
            m_index = 0;
        }
    }
}
