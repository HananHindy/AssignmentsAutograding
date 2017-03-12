using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace FastFib
{
    class Program
    {
        #region I/O
        static void Main(string[] args)
        {
            long nCases;
            long result, actualResult;
            StreamReader sr;
            TextReader origConsole = Console.In;
            Console.WriteLine("Fast Fib Problem:\n[1] Sample test cases\n[2] Complete testing");
            Console.Write("\nEnter your choice [1-2]: ");
           // char choice = (char)Console.ReadLine()[0];
          //  switch (choice)
           // {
             //   case '1':
                    sr = new StreamReader("samples.txt");
                    Console.SetIn(sr);

                    nCases = long.Parse(Console.ReadLine());

                    for (long i = 0; i < nCases; i++)
                    {
                        long N = long.Parse(Console.ReadLine());

                        Console.Write("Case " + (i + 1).ToString() + ": ");
                        Console.WriteLine("N = " + N);

                        long timeBefore = System.Environment.TickCount;
                        result = FastBoundedFib.FastBoundedFibonacci(N);
                        long timeAfter = System.Environment.TickCount;
                        if (timeAfter - timeBefore > 30)
                        {
                            Console.WriteLine("[#EVAL]Time limit exceed: case # " + (i + 1));
                            sr.Close();
                            return;
                        }
                        actualResult = long.Parse(Console.ReadLine());
                        if (actualResult != result)
                        {
                            Console.WriteLine("[#EVAL]Wrong Answer!" + " your answer = "
                                + result + ", correct answer = " + actualResult);
                            sr.Close();
                            return;
                        }
                        else
                        {
                            Console.WriteLine("Succeed");
                        }
                    }
                    sr.Close();

                    Console.SetIn(origConsole);

                    Console.WriteLine("Sample cases run successfully. you should run Complete Testing... ");
                    Console.Write("Do you want to run Complete Testing now (y/n)? ");
                    //choice = (char)Console.Read();
                    //if (choice == 'n' || choice == 'N')
                    //    break;
                    //else if (choice == 'y' || choice == 'Y')
                    //    goto CompleteTest;
                    //else
                    //{
                    //    Console.WriteLine("Invalid Choice!");
                    //    break;
                    //}

                //case '2':
                //CompleteTest:
                    Console.WriteLine("Complete Testing is running now...");
                    sr = new StreamReader("input.txt");
                    Console.SetIn(sr);

                    nCases = long.Parse(Console.ReadLine());

                    for (long i = 0; i < nCases; i++)
                    {
                        long N = long.Parse(Console.ReadLine());

                        long timeBefore = System.Environment.TickCount;
                        result = FastBoundedFib.FastBoundedFibonacci(N);
                        long timeAfter = System.Environment.TickCount;
                        if (timeAfter - timeBefore > 50)
                        {
                            Console.WriteLine("[#EVAL]Time limit exceed: case # " + (i + 1));
                            sr.Close();
                            return;
                        }
                        actualResult = long.Parse(Console.ReadLine());
                        if (actualResult != result)
                        {
                            Console.WriteLine("[#EVAL]Wrong Answer: case # " + (i + 1) + ": your answer = "
                                + result + ", correct answer = " + actualResult);
                            sr.Close();
                            return;
                        }
                    }
                    sr.Close();
                    Console.WriteLine("\n[#EVAL]Congratulations... your program runs successfully");
                   // break;

            //}
        }
        #endregion

    }
}
