namespace ilrd
{
    public abstract class Shape
    {
        private string _color;
        private bool _isFull;

        public string Color
        {
            get => _color;
            set => _color = value;
        }

        public bool IsFull => _isFull;

        public Shape(string color_ = "green", bool isFull_ = true)
        {
            _color = color_;
            _isFull = isFull_;
        }

        public abstract double Area();
        public abstract double Perimeter();

        public override string ToString()
            => $"Shape with color = {Color} and IsFilled = {IsFull}";

    }
}