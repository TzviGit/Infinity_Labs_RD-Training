using System.Collections;

namespace ilrd
{


    /// <summary>
    /// hello
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class GenericList<T> : IEnumerable
    {
        private class Node
        {
            public T Item { get; set; }

            public Node Next { get; set; }
            public Node Previous { get; set; }

            public Node(T data = default, Node next = default, Node prev = default)
            {
                Item = data;
                Next = next;
                Previous = prev;
            }

        }

        private Node m_root;
        private Node m_end;
        private ulong m_count;


        public GenericList()
        {
            Clear();
        }

        private Node First
        {
            get => m_root.Next;
        }

        public void Push(T data)
        {
            Node temp = m_end.Previous;
            Node node = new Node(data, m_end, temp);
            m_end.Previous = node;
            temp.Next = node;

            ++m_count;
        }

        public void Pop()
        {
            if (!IsEmpty())
            {
                Node temp = First.Next;
                temp.Previous = m_root;
                m_root.Next = temp;
                --m_count;
            }

        }

        public T Peek()
        {
            if (IsEmpty())
            {
                throw new AccessViolationException("list is empty!");
            }

            return First.Item;
        }

        public bool IsEmpty()
        {
            return m_count == 0;
        }

        public ulong Count()
        {
            return m_count;
        }

        public void Clear()
        {
            m_end = new Node();
            m_root = new Node(default, m_end);
            m_end.Previous = m_root;
            m_count = 0;
        }

        public IEnumerator GetEnumerator()
        {
            for (Node node = First; node != m_end; node = node.Next)
            {
                yield return node.Item;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }


    }
}
