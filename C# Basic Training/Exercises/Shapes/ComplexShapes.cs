namespace ilrd
{

    public class ComplexShapes
    {
        public ComplexShapes() //Ctor
        {
            _shapes = new List<Shape>();
        }


        private readonly List<Shape> _shapes;

        public Shape this[int index_] => _shapes[index_]; //no setter only get




        public void Add(Shape shape_)
        {
            _shapes.Add(shape_);
        }

        public double Area()
        {
            double totalArea = 0;

            foreach (Shape shape in _shapes)
            {
                totalArea += shape.Area();
            }

            return Math.Round(totalArea, 3);
        }

        public double Perimeter()
        {
            double totalPerimeter = 0;

            foreach (Shape shape in _shapes)
            {
                totalPerimeter += shape.Perimeter();
            }

            return Math.Round(totalPerimeter, 3);
        }

    }
}
