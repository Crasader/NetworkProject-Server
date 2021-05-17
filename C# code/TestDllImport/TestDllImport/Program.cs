using System;
using System.Runtime.InteropServices;

namespace TestDllImport
{
    class Program
    {
        [DllImport("/Users/anhduy0911/Projects/School/NetworkProgramming/midProject/C# code/TestDllImport/TestDllImport/obj/Debug/netcoreapp3.1/dll.dylib", EntryPoint = "addInC")]
        static extern float addInC(float a, float b);

        [DllImport("/Users/anhduy0911/Projects/School/NetworkProgramming/midProject/C# code/TestDllImport/TestDllImport/obj/Debug/netcoreapp3.1/dll.dylib", EntryPoint = "instance_create")]
        static extern IntPtr instance_create();

        [DllImport("/Users/anhduy0911/Projects/School/NetworkProgramming/midProject/C# code/TestDllImport/TestDllImport/obj/Debug/netcoreapp3.1/dll.dylib", EntryPoint = "add")]
        static extern float add(IntPtr tl, float a, float b);

        [DllImport("/Users/anhduy0911/Projects/School/NetworkProgramming/midProject/C# code/TestDllImport/TestDllImport/obj/Debug/netcoreapp3.1/dll.dylib", EntryPoint = "substract")]
        static extern float substract(IntPtr tl, float a, float b);

        static void Main(string[] args)
        {
            float a = addInC(5.5f, 3.3f);

            IntPtr tl = instance_create();
            float b = add(tl, 5.5f, 3.3f);
            float c = substract(tl, 5.5f, 3.3f);

            Console.WriteLine("Hey, this is result from c++ :" + a + " " + b+ " " + c);
        }
    }
}
