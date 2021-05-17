[DllImport("dll.dylib", EntryPoint = "addInC")]
static extern float addInC(float a, float b);

[DllImport("dll.dylib", EntryPoint = "add")]
static extern float add(float a, float b);

[DllImport("libtestmachw.dylib", EntryPoint = "subtract")]
static extern float subtract(float a, float b);


static void Main(string[] args)
{
    float a = addInC(5.5, 3.3);
    float b = add(5.5, 3.3);
    float c = subtract(5.5, 3.3);
    Console.WriteLine("Hey, this is result from c++ :" + a + " " + b + " " + c);
}