using System;

namespace ChilliEngine
{
    public class Main
    {
        public Main()
        {
            Console.WriteLine("Main Constructor");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello from C#");
        }

        public void PrintInt(int number)
        {
            Console.WriteLine($"C# says: {number}");
        }

        public void Add(int number, int number2)
        {
            Console.WriteLine($"C# says: {number+number2}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }

           

        

    }
}
