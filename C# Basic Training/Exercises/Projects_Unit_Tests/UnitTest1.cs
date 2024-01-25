using ilrd;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

namespace ShapesUnitTest
{
    [TestClass]
    public class CircleUnitTest
    {
        public Func<double, double, bool> WithinRange = (double d1, double d2) => (Math.Abs(d2 - d1) < 0.01);

        [TestMethod]
        public void DefaultCircleTest()
        {
            Circle c1 = new Circle();
            Circle c2 = new Circle();

            //Assert.IsTrue(c1.Equals(c2));
            Assert.AreEqual(c1.Radius, 1);
            Assert.AreEqual(c1.Color, "green");
            Assert.AreEqual(c1.IsFull, true);
            Assert.AreEqual(c1.Area(), Math.Round((Math.PI), 3));
            Assert.AreEqual(c1.Perimeter(), Math.Round((Math.PI * 2), 3));
        }

        [TestMethod]
        public void NotDefaultCircleTest()
        {
            Circle c1 = new Circle("red", false, 2);

            Assert.AreEqual(c1.Radius, 2);
            Assert.AreEqual(c1.Color, "red");
            Assert.AreEqual(c1.IsFull, false);
            Assert.AreEqual(c1.Area(), Math.Round((Math.PI * 4), 3));
            Assert.AreEqual(c1.Perimeter(), Math.Round((Math.PI * 4), 3));
        }
    }

    [TestClass]
    public class RectangleUnitTest
    {
        public Func<double, double, bool> WithinRange = (double d1, double d2) => (Math.Abs(d2 - d1) < 0.01);

        [TestMethod]
        public void RectangleTest()
        {
            Rectangle r1 = new();
            Assert.AreEqual(r1.Width, 1);
            Assert.AreEqual(r1.Height, 1);
            Assert.AreEqual(r1.Color, "green");
            Assert.AreEqual(r1.IsFull, true);
            Assert.AreEqual(r1.Area(), 1);
            Assert.AreEqual(r1.Perimeter(), 4);

            Rectangle r2 = new("black", false, 2, 3);
            Assert.AreEqual(r2.Width, 2);
            Assert.AreEqual(r2.Height, 3);
            Assert.AreEqual(r2.Color, "black");
            Assert.AreEqual(r2.IsFull, false);
            Assert.AreEqual(r2.Area(), 6);
            Assert.AreEqual(r2.Perimeter(), 10);
        }
    }

    [TestClass]
    public class ComplexShapeUnitTest
    {
        public Func<double, double, bool> WithinRange = (double d1, double d2) => (Math.Abs(d2 - d1) < 0.01);

        [TestMethod]
        public void ComplexShapeTest()
        {
            ComplexShapes cs = new ComplexShapes();
            cs.Add(new Circle());
            cs.Add(new Circle());
            cs.Add(new Rectangle());
            cs.Add(new Rectangle());

            Assert.IsTrue(WithinRange(cs.Area(), (Math.PI * 2 + 2)));
            Assert.IsTrue(WithinRange(cs.Perimeter(), (Math.PI * 2 * 2 + 4 * 2)));
        }
    }
}