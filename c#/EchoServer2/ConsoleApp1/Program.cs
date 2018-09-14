using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime;
using System.Threading;


namespace EchoServer
{
    class Program
    {
        static void Main(string[] args)
        {
            if (GCSettings.IsServerGC == true)
                Console.WriteLine("ServerMODE");

            Server<UserSession> server = new Server<UserSession>();
            server.Listen();

            while(true)
            {
                Thread.Sleep(1);
            }
        }
    }
}
