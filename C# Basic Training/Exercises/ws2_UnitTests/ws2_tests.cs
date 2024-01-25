// See https://aka.ms/new-console-template for more information


using ilrd;

Console.WriteLine("Hello, World!");



ObjectsStack stack = new();

/*stack.StackPush(7);
stack.StackPush("first base");
stack.StackPush(4.35D);*/
foreach (object item in stack.Internal)
{
    stack.Push(55);
}
try
{


    stack.Push(22);
}
catch
{
    Console.WriteLine("hgfhfhf:");
}
foreach (object item in stack.Internal)
{
    Console.WriteLine(item);
}
Console.WriteLine(stack.Peek());
stack.Pop();



Console.WriteLine("NOW TESTING GENERIC LIST\n\n");

GenericList<int> list = new GenericList<int>();

int max = 10;
for (int i = 0; i < max; i++)
{
    list.Push(i);
}


foreach (int i in list)
{

    Console.WriteLine(i);
}


Console.WriteLine("NOW TESTING Shapes\n\n");

Rectangle s1 = new Rectangle("blue", false, 32, 34);
Console.WriteLine(s1.ToString());

ComplexShapes compl = new ComplexShapes();

max = 15;
for (int i = 0; i < max; i++)
{
    // compl.Add(new Circle(i + 1));
    compl.Add(new Rectangle(2, 3));//(i + 1, i + 3));
}

Console.WriteLine(compl.Area() / 15);


Console.WriteLine(compl.Perimeter() / 15);



