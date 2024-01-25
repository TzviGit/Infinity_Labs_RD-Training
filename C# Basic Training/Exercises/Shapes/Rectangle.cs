namespace ilrd
{
    public class Rectangle : Shape
    {
        public double Width { get; set; }
        public double Height { get; set; }

        public Rectangle(double width_ = 1.0, double height_ = 1.0)
        : base()
        {
            Width = width_;
            Height = height_;
        }

        public Rectangle(string color_, bool isFull_, double width_ = 1.0, double height_ = 1.0)
        : base(color_, isFull_)
        {
            Width = width_;
            Height = height_;
        }
        static Rectangle()//static contructor
        {
            Console.WriteLine("Rectangle class initialized, current time is: " + DateTime.Now.ToLongTimeString());
        }

        public override double Area() => Math.Round((Width * Height), 3);

        public override double Perimeter() => Math.Round((2 * (Width + Height)), 3);

        public override string ToString()
            => $"A {typeof(Rectangle)} with width = {Width} and hight = {Height}, which is a derived class of {base.ToString()}";


    }
}
