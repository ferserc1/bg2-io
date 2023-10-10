using System;
using System.IO;
using bg2io;

namespace HelloWorld
{
    class Program
    {
        static void Main(string [] args)
        {
            Bg2FileReader fileReader = new Bg2FileReader();

            string filePath = Directory.GetCurrentDirectory() + "/../../resources/cubes.bg2";

            try {
                Bg2File bg2File = fileReader.open(filePath);    
                Console.WriteLine("Version: " + bg2File.GetVersion());
            }
            catch (Exception e) {
                Console.WriteLine(e.Message + "\n Cannot read from file");
                return;
            }
        }
    }
}
