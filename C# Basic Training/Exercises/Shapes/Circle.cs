namespace ilrd
{
    public class Circle : Shape
    {
        private double _radius;

        public double Radius
        {
            get => _radius;
            set => _radius = value;
        }

        public Circle(double radius_ = 1.0)
        : base()
        {
            Radius = radius_;
        }

        public Circle(string color_, bool isFull_, double radius_ = 1.0)
        : base(color_, isFull_)
        {
            Radius = radius_;
        }

        public override string ToString()
            => $"A {typeof(Circle)} with radius = {Radius}, which is a derived class of {base.ToString()}";

        public override double Area() => Math.Round((Math.PI * Math.Pow(Radius, 2)), 3);

        public override double Perimeter() => Math.Round((2 * Math.PI * Radius), 3);


    }
}
