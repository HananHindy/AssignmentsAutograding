using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FastFib
{
    // *****************************************
    // DON"T CHANGE CLASS NAME OR FUNCTION NAME
    // *****************************************
    public static class FastBoundedFib
    {
        //====================
        //Your Code is Here:
        //===================
        /// <summary>
        /// Write efficient algorithm to the bounded-Fibonacci of index N using the matrix equation
        /// </summary>
        /// <param name="N">fibonacci index</param>
        /// <returns>fibonacci value</returns>
        public static long FastBoundedFibonacci(long N)
        {
            return 0;
        }

        #region Sorting function - if you need it
        public static void Sorting(long[] collection, long N)
        {
            QSort(0, N - 1, collection, N);
        }

        public static void Swap(long i, long j, long[] collection)
        {
            long tmp = collection[i];
            collection[i] = collection[j];
            collection[j] = tmp;
        }

        public static void QSort(long startIndex, long finalIndex, long[] collection, long N)
        {
            if (startIndex >= finalIndex) return;
            long i = startIndex, j = finalIndex;
            while (i <= j)
            {
                while (i < N && collection[startIndex] >= collection[i]) i++;
                while (j > -1 && collection[startIndex] < collection[j]) j--;
                if (i <= j)
                {
                    Swap(i, j, collection);
                }
            }
            Swap(startIndex, j, collection);
            QSort(startIndex, j - 1, collection, N);
            QSort(i, finalIndex, collection, N);
        }
        #endregion
    }
}
